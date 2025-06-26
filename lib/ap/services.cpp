#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include <services.hpp>
#include <uart1.hpp>

WiFiClient client;
WebServer server(80);

const char* control_page_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>电机转速控制</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f9;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            background: #ffffff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            text-align: center;
            width: 300px;
        }
        h1 {
            font-size: 1.5em;
            color: #333;
        }
        label {
            display: block;
            margin: 10px 0 5px;
            font-weight: bold;
        }
        input[type="number"] {
            width: 100%;
            padding: 8px;
            margin-bottom: 10px;
            border: 1px solid #ccc;
            border-radius: 4px;
            font-size: 1em;
        }
        button {
            background-color: #007bff;
            color: white;
            border: none;
            padding: 10px 15px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 1em;
        }
        button:hover {
            background-color: #0056b3;
        }
        .error {
            color: red;
            font-size: 0.9em;
            margin-top: 10px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>电机转速控制</h1>
        <form id="speedForm">
            <label for="speed">设置转速 (0 - 5):</label>
            <input type="number" id="speed" name="speed" step="0.1" min="0" max="5" required>
            <button type="submit">提交</button>
            <div class="error" id="error"></div>
        </form>
    </div>

    <script>
        document.getElementById('speedForm').addEventListener('submit', function(event) {
            event.preventDefault();
            const speedInput = document.getElementById('speed');
            const errorDiv = document.getElementById('error');
            const speedValue = parseFloat(speedInput.value);

            // 修改前端JavaScript，添加发送数据到服务器的逻辑
            if (isNaN(speedValue) || speedValue < 0 || speedValue > 5) {
                errorDiv.textContent = "请输入 0 到 5 之间的有效浮点数！";
            } else {
                errorDiv.textContent = "";
                // 发送数据到服务器
                fetch(`/set_speed?speed=${speedValue}`)
                    .then(response => response.text())
                    .then(data => {
                        alert(`转速已设置为: ${speedValue}`);
                        console.log(data);
                    })
                    .catch(error => {
                        errorDiv.textContent = "发送失败，请重试！";
                        console.error(error);
                    });
            }
        });
    </script>
</body>
</html>
)rawliteral";

bool ap_init() {
    Serial.println("Starting Access Point...");

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
    if (!WiFi.softAP("output_kontrol", "35086020")) {
        Serial.println("Failed to start Access Point");
        return false;
    }

    Serial.println("Access Point started with SSID: output_kontrol and Password: 35086020");
    return true;
}

void service_init() {

    server.begin();

    server.on("/", []() {
        server.send(200, "text/plain", "Hello from Access Point!");
    });
    
    server.on("/status", []() {
        server.send(200, "application/json", "{\"status\":\"running\"}");
    });

    server.onNotFound([]() {
        server.send(404, "text/plain", "Not Found");
    });

    server.on("/control", []() {
        server.send(200, "text/html", control_page_html);
    });

    server.on("/set_speed", []() {
        if (server.hasArg("speed")) {
            String speed = server.arg("speed");
            strncpy(send_buffer, speed.c_str(), sizeof(send_buffer) - 1); // Update the send_buffer with the new speed value
            send_buffer[sizeof(send_buffer) - 1] = '\0'; // Ensure null-termination
            server.send(200, "text/plain", "Speed set to: " + speed);
        } else {
            server.send(400, "text/plain", "Speed not provided");
        }
    });

    
}


void ap_service_task(void* args) {
    while (1) {
        server.handleClient(); // Handle incoming client requests
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay to prevent busy-waiting
    }

}