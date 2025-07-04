const WebSocket = require("ws");

const PORT = 3000;
const wss = new WebSocket.Server({ port: PORT });

let espSocket = null;
let browserSocket = null;


wss.on("connection", (ws, req) => {
  console.log("New client connected");

  ws.on("message", (message) => {
    try {
      const data = JSON.parse(message);

      if (data.device_id) {
        console.log("Message from ESP:", data);
        espSocket = ws;

        if (browserSocket && browserSocket.readyState === WebSocket.OPEN) {
          browserSocket.send(JSON.stringify(data));
        }
      }
