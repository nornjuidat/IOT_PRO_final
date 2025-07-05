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

      else if (data.command === "cancel") {
        console.log("Cancel command from browser");

        if (espSocket && espSocket.readyState === WebSocket.OPEN) {
          espSocket.send(JSON.stringify({ command: "cancel" }));
        }
      }

    } catch (err) {
      console.error("Invalid message:", message);
    }
  });
  ws.on("close", () => {
    if (ws === espSocket) {
      console.log("ESP disconnected");
      espSocket = null;
    } else if (ws === browserSocket) {
      console.log("Browser disconnected");
      browserSocket = null;
    }
  });

  if (!browserSocket) {
    browserSocket = ws;
    console.log("Browser connected");
  }
});

console.log(`WebSocket server is running on ws://localhost:${PORT}`);

