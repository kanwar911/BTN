#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <string>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>

class Light;
class Thermostat;
class Camera;
extern std::string processCommand(const std::string& command);

std::string generateControlPanelHTML() {
    return R"(
<!DOCTYPE html>
<html>
<head>
    <title>Smart Home Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600&display=swap">
    <style>
        /* Modern, eye-calming color scheme */
        :root {
            --bg-color: #f8f9fa;
            --card-color: #ffffff;
            --text-color: #212529;
            --primary-color: #5469d4;
            --primary-light: #e6ebff;
            --secondary-color: #94a3b8;
            --success-color: #10b981;
            --success-light: #ecfdf5;
            --danger-color: #ef4444;
            --danger-light: #fee2e2;
            --shadow: 0 10px 15px -3px rgba(0, 0, 0, 0.1), 0 4px 6px -2px rgba(0, 0, 0, 0.05);
            --shadow-sm: 0 1px 3px rgba(0, 0, 0, 0.12), 0 1px 2px rgba(0, 0, 0, 0.04);
        }
        
        * {
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Poppins', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', sans-serif;
            background-color: var(--bg-color);
            color: var(--text-color);
            margin: 0;
            padding: 20px;
            line-height: 1.6;
            transition: background-color 0.3s ease;
        }
        
        .container {
            max-width: 800px;
            margin: 0 auto;
        }
        
        header {
            text-align: center;
            margin-bottom: 2rem;
        }
        
        h1 {
            color: var(--primary-color);
            font-weight: 600;
            font-size: 2rem;
            margin-bottom: 0.5rem;
        }
        
        .subtitle {
            color: var(--secondary-color);
            font-weight: 400;
            font-size: 1rem;
        }
        
        .card {
            background-color: var(--card-color);
            border-radius: 12px;
            box-shadow: var(--shadow);
            padding: 24px;
            margin-bottom: 24px;
            transition: all 0.3s ease;
            border: 1px solid rgba(0, 0, 0, 0.05);
        }
        
        .card:hover {
            transform: translateY(-4px);
            box-shadow: var(--shadow);
        }
        
        .card-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
            position: relative;
        }
        
        .card-title {
            font-size: 1.4rem;
            color: var(--text-color);
            font-weight: 500;
            margin: 0;
            display: flex;
            align-items: center;
        }
        
        .card-title svg {
            margin-right: 8px;
            fill: var(--primary-color);
        }
        
        .card-header .controls {
            display: flex;
            gap: 8px;
        }
        
        .btn {
            border: none;
            border-radius: 8px;
            padding: 10px 16px;
            font-size: 0.9rem;
            font-weight: 500;
            cursor: pointer;
            transition: all 0.2s ease;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 6px;
        }
        
        .btn svg {
            width: 16px;
            height: 16px;
        }
        
        .btn-primary {
            background-color: var(--primary-color);
            color: white;
        }
        
        .btn-primary:hover {
            background-color: #4559b3;
            transform: translateY(-2px);
        }
        
        .btn-outline {
            background-color: transparent;
            color: var(--primary-color);
            border: 1px solid var(--primary-color);
        }
        
        .btn-outline:hover {
            background-color: var(--primary-light);
        }
        
        .btn-success {
            background-color: var(--success-color);
            color: white;
        }
        
        .btn-success:hover {
            background-color: #0da271;
            transform: translateY(-2px);
        }
        
        .btn-danger {
            background-color: var(--danger-color);
            color: white;
        }
        
        .btn-danger:hover {
            background-color: #dc3535;
            transform: translateY(-2px);
        }
        
        .status {
            margin-top: 16px;
            font-size: 0.9rem;
            color: var(--secondary-color);
            display: flex;
            align-items: center;
            gap: 8px;
        }
        
        .status.active {
            color: var(--success-color);
        }
        
        .status-dot {
            display: inline-block;
            width: 8px;
            height: 8px;
            border-radius: 50%;
            background-color: var(--secondary-color);
        }
        
        .status-dot.active {
            background-color: var(--success-color);
        }
        
        /* Light visualization */
        .light-visualization {
            width: 100%;
            height: 100px;
            background-color: #f0f0f0;
            border-radius: 8px;
            position: relative;
            overflow: hidden;
            margin-bottom: 16px;
            transition: all 0.5s ease;
            border: 2px solid rgba(0, 0, 0, 0.05);
        }
        
        .light-visualization.on {
            background-color: #ffea8a;
            box-shadow: 0 0 30px rgba(255, 234, 138, 0.7);
        }
        
        .light-bulb {
            position: absolute;
            top: 20px;
            left: 50%;
            transform: translateX(-50%);
            width: 40px;
            height: 40px;
            transition: all 0.5s ease;
        }
        
        .light-bulb svg {
            fill: #d0d0d0;
            transition: all 0.5s ease;
        }
        
        .light-visualization.on .light-bulb svg {
            fill: #f59e0b;
        }
        
        /* Thermostat */
        .temp-control {
            display: flex;
            flex-direction: column;
            align-items: center;
            margin: 20px 0;
        }
        
        .temp-value {
            text-align: center;
            font-size: 3rem;
            font-weight: 300;
            color: var(--primary-color);
            margin: 10px 0;
            width: 100%;
            position: relative;
        }
        
        .temp-value::after {
            content: '°C';
            font-size: 1.5rem;
            position: absolute;
            right: 30%;
            top: 5px;
        }
        
        .slider-container {
            width: 80%;
            margin: 10px 0;
        }
        
        .slider {
            width: 100%;
            -webkit-appearance: none;
            appearance: none;
            height: 8px;
            background: #e2e8f0;
            border-radius: 10px;
            outline: none;
            margin: 20px 0;
        }
        
        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 24px;
            height: 24px;
            border-radius: 50%;
            background: var(--primary-color);
            cursor: pointer;
            transition: all 0.2s ease;
            box-shadow: 0 0 5px rgba(0, 0, 0, 0.2);
        }
        
        .slider::-webkit-slider-thumb:hover {
            transform: scale(1.2);
        }
        
        /* Camera */
        .camera-feed {
            border-radius: 8px;
            overflow: hidden;
            margin-bottom: 16px;
            position: relative;
            aspect-ratio: 16/9;
            background-color: #2c3e50;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        
        .camera-icon {
            color: white;
            font-size: 2rem;
            opacity: 0.7;
        }
        
        .camera-status {
            position: absolute;
            top: 10px;
            right: 10px;
            background-color: rgba(0, 0, 0, 0.5);
            color: white;
            padding: 4px 8px;
            border-radius: 4px;
            font-size: 0.8rem;
            display: flex;
            align-items: center;
            gap: 5px;
        }
        
        .camera-status-dot {
            width: 8px;
            height: 8px;
            border-radius: 50%;
            background-color: #10b981;
        }
        
        /* Loading animation */
        .loading {
            display: inline-block;
            width: 16px;
            height: 16px;
            border: 2px solid rgba(84, 105, 212, 0.3);
            border-radius: 50%;
            border-top-color: var(--primary-color);
            animation: spin 1s ease-in-out infinite;
        }
        
        @keyframes spin {
            to { transform: rotate(360deg); }
        }
        
        /* Notification */
        .notification {
            position: fixed;
            bottom: 24px;
            right: 24px;
            background-color: var(--primary-color);
            color: white;
            padding: 12px 20px;
            border-radius: 8px;
            box-shadow: var(--shadow);
            opacity: 0;
            transform: translateY(10px);
            transition: all 0.3s ease;
            z-index: 1000;
            max-width: 300px;
        }
        
        .notification.show {
            opacity: 1;
            transform: translateY(0);
        }
        
        /* Dark mode toggle */
        .mode-toggle {
            position: absolute;
            top: 20px;
            right: 20px;
            background: none;
            border: none;
            color: var(--secondary-color);
            cursor: pointer;
            font-size: 1.5rem;
        }
        
        /* Dark mode */
        body.dark-mode {
            --bg-color: #121212;
            --card-color: #1e1e1e;
            --text-color: #e5e5e5;
            --primary-light: #2d2d3a;
            --success-light: #0f2922;
            --danger-light: #3a1a1a;
            --shadow: 0 10px 15px -3px rgba(0, 0, 0, 0.3), 0 4px 6px -2px rgba(0, 0, 0, 0.2);
        }
        
        /* For small screens */
        @media (max-width: 768px) {
            .card-header {
                flex-direction: column;
                align-items: flex-start;
                gap: 10px;
            }
            
            .card-header .controls {
                width: 100%;
                justify-content: space-between;
            }
            
            .temp-value::after {
                right: 15%;
            }
        }
    </style>
</head>
<body>
    <button id="modeToggle" class="mode-toggle">🌙</button>
    
    <div class="container">
        <header>
            <h1>Smart Home Control</h1>
            <div class="subtitle">Control your home from anywhere</div>
        </header>
        
        <!-- Light Control -->
        <div class="card">
            <div class="card-header">
                <h2 class="card-title">
                    <svg width="24" height="24" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
                        <path d="M12 7C14.76 7 17 9.24 17 12C17 13.64 16.12 15.09 14.83 16H9.17C7.88 15.09 7 13.64 7 12C7 9.24 9.24 7 12 7ZM12 5C8.14 5 5 8.14 5 12C5 14.38 6.19 16.47 8 17.74V19C8 19.55 8.45 20 9 20H15C15.55 20 16 19.55 16 19V17.74C17.81 16.47 19 14.38 19 12C19 8.14 15.86 5 12 5ZM11 21V22H13V21H11Z"/>
                    </svg>
                    Lighting
                </h2>
                <div class="controls">
                    <button id="lightOn" class="btn btn-success">
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
                            <path d="M13 3H11V13H13V3Z"/>
                            <path d="M16.83 7.17L15.42 8.58C16.63 9.79 17 11.45 16.63 13.04C16.33 14.36 15.56 15.2 14.25 15.55C11.5 16.24 9 14.33 9 11.7C9 10.45 9.5 9.24 10.42 8.58L9 7.17C7.64 8.18 6.84 9.75 6.84 11.7C6.84 15.42 10.17 18.23 14 16.72C16 15.87 17.4 14.2 17.94 12.06C18.47 9.91 18 7.67 16.83 7.17Z"/>
                        </svg>
                        Turn On
                    </button>
                    <button id="lightOff" class="btn btn-danger">
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
                            <path d="M13 3H11V9H13V3Z"/>
                            <path d="M17.24 10.67L18.48 8.53C16.94 7.07 14.66 6.57 12.38 7.04C10.84 7.34 9.45 8.16 8.47 9.26L10.23 10.67C10.85 9.97 11.76 9.5 12.77 9.3C14.26 9.04 15.78 9.43 16.87 10.4C17 10.5 17.13 10.59 17.24 10.67Z"/>
                            <path d="M3.41 2.01L2 3.42L7.39 8.81C7.11 9.69 7.25 10.7 7.97 11.42L9.86 13.31C10.54 13.99 11.5 14.15 12.36 13.93L14.3 15.87L13.75 16.42C13.36 16.81 13.36 17.44 13.75 17.83C14.14 18.22 14.77 18.22 15.16 17.83L20.58 12.42L3.41 2.01ZM9.06 9.19L10.17 10.3C10.11 10.92 10.27 11.54 10.69 12L8.8 10.11L9.06 9.19Z"/>
                        </svg>
                        Turn Off
                    </button>
                </div>
            </div>
            
            <div id="lightVisualization" class="light-visualization">
                <div class="light-bulb">
                    <svg width="40" height="40" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
                        <path d="M12 2C8.14 2 5 5.14 5 9C5 11.38 6.19 13.47 8 14.74V17C8 17.55 8.45 18 9 18H15C15.55 18 16 17.55 16 17V14.74C17.81 13.47 19 11.38 19 9C19 5.14 15.86 2 12 2ZM9 21C9 21.55 9.45 22 10 22H14C14.55 22 15 21.55 15 21V20H9V21Z"/>
                    </svg>
                </div>
            </div>
            
            <div id="lightStatus" class="status">
                <span class="status-dot"></span>
                Checking status...
            </div>
        </div>
        
        <!-- Thermostat Control -->
        <div class="card">
            <div class="card-header">
                <h2 class="card-title">
                    <svg width="24" height="24" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
                        <path d="M15 13V5C15 3.34 13.66 2 12 2C10.34 2 9 3.34 9 5V13C7.79 13.91 7 15.37 7 17C7 19.76 9.24 22 12 22C14.76 22 17 19.76 17 17C17 15.37 16.21 13.91 15 13ZM12 4C12.55 4 13 4.45 13 5V8H11V5C11 4.45 11.45 4 12 4Z"/>
                    </svg>
                    Thermostat
                </h2>
            </div>
            
            <div class="temp-control">
                <div id="tempValue" class="temp-value">22</div>
                <div class="slider-container">
                    <input type="range" id="tempSlider" class="slider" min="16" max="30" value="22">
                </div>
                <button id="setTemp" class="btn btn-primary">
                    <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
                        <path d="M9 16.17L4.83 12L3.41 13.41L9 19L21 7L19.59 5.59L9 16.17Z"/>
                    </svg>
                    Set Temperature
                </button>
            </div>
            
            <div id="thermostatStatus" class="status">
                <span class="status-dot"></span>
                Checking status...
            </div>
        </div>
        
        <!-- Camera Control -->
        <div class="card">
            <div class="card-header">
                <h2 class="card-title">
                    <svg width="24" height="24" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
                        <path d="M17 10.5V7C17 6.45 16.55 6 16 6H4C3.45 6 3 6.45 3 7V17C3 17.55 3.45 18 4 18H16C16.55 18 17 17.55 17 17V13.5L21 17.5V6.5L17 10.5Z"/>
                    </svg>
                    Security Camera
                </h2>
                <button id="checkCamera" class="btn btn-outline">
                    <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
                        <path d="M12 4.5C7 4.5 2.73 7.61 1 12C2.73 16.39 7 19.5 12 19.5C17 19.5 21.27 16.39 23 12C21.27 7.61 17 4.5 12 4.5ZM12 17C9.24 17 7 14.76 7 12C7 9.24 9.24 7 12 7C14.76 7 17 9.24 17 12C17 14.76 14.76 17 12 17ZM12 9C10.34 9 9 10.34 9 12C9 13.66 10.34 15 12 15C13.66 15 15 13.66 15 12C15 10.34 13.66 9 12 9Z"/>
                    </svg>
                    Check Status
                </button>
            </div>
            
            <div class="camera-feed">
                <div class="camera-icon">📹</div>
                <div id="cameraStatusBadge" class="camera-status">
                    <span class="camera-status-dot"></span>
                    <span>ACTIVE</span>
                </div>
            </div>
            
            <div id="cameraStatus" class="status">
                <span class="status-dot"></span>
                Checking status...
            </div>
        </div>
    </div>
    
    <div id="notification" class="notification"></div>
    
    <script>
        // Dark mode toggle
        const body = document.body;
        const modeToggle = document.getElementById('modeToggle');
        
        modeToggle.addEventListener('click', () => {
            body.classList.toggle('dark-mode');
            modeToggle.textContent = body.classList.contains('dark-mode') ? '☀️' : '🌙';
        });
        
        // Function to send API requests
        async function sendCommand(command) {
            try {
                const response = await fetch(`/api/${command}`);
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                const text = await response.text();
                return text;
            } catch (error) {
                console.error('Error:', error);
                showNotification('Error communicating with server');
                return 'Error communicating with server';
            }
        }
        
        // Show notification
        function showNotification(message) {
            const notification = document.getElementById('notification');
            notification.textContent = message;
            notification.classList.add('show');
            
            setTimeout(() => {
                notification.classList.remove('show');
            }, 3000);
        }
        
        // Light control
        const lightVisualization = document.getElementById('lightVisualization');
        const lightStatus = document.getElementById('lightStatus');
        const lightStatusDot = lightStatus.querySelector('.status-dot');
        
        function updateLightUI(isOn) {
            if (isOn) {
                lightVisualization.classList.add('on');
                lightStatusDot.classList.add('active');
                lightStatus.classList.add('active');
            } else {
                lightVisualization.classList.remove('on');
                lightStatusDot.classList.remove('active');
                lightStatus.classList.remove('active');
            }
        }
        
        document.getElementById('lightOn').addEventListener('click', async () => {
            lightStatus.innerHTML = '<span class="loading"></span> Turning on...';
            const result = await sendCommand('light/on');
            lightStatus.innerHTML = '<span class="status-dot"></span> ' + result;
            updateLightUI(true);
            showNotification(result);
        });
        
        document.getElementById('lightOff').addEventListener('click', async () => {
            lightStatus.innerHTML = '<span class="loading"></span> Turning off...';
            const result = await sendCommand('light/off');
            lightStatus.innerHTML = '<span class="status-dot"></span> ' + result;
            updateLightUI(false);
            showNotification(result);
        });
        
        // Thermostat controls
        const slider = document.getElementById('tempSlider');
        const tempValue = document.getElementById('tempValue');
        
        slider.addEventListener('input', () => {
            tempValue.textContent = slider.value;
        });
        
        document.getElementById('setTemp').addEventListener('click', async () => {
            const temp = slider.value;
            const thermostatStatus = document.getElementById('thermostatStatus');
            thermostatStatus.innerHTML = '<span class="loading"></span> Setting temperature...';
            const result = await sendCommand(`thermostat/set/${temp}`);
            thermostatStatus.innerHTML = '<span class="status-dot"></span> ' + result;
            showNotification(result);
        });
        
        // Camera controls
        document.getElementById('checkCamera').addEventListener('click', async () => {
            const cameraStatus = document.getElementById('cameraStatus');
            const cameraStatusBadge = document.getElementById('cameraStatusBadge');
            cameraStatus.innerHTML = '<span class="loading"></span> Checking...';
            const result = await sendCommand('camera/status');
            cameraStatus.innerHTML = '<span class="status-dot"></span> ' + result;
            
            // Update camera status badge
            if (result.includes('ACTIVE')) {
                cameraStatusBadge.querySelector('span:last-child').textContent = 'ACTIVE';
                cameraStatusBadge.style.backgroundColor = 'rgba(16, 185, 129, 0.7)'; // Green
            } else {
                cameraStatusBadge.querySelector('span:last-child').textContent = 'OFFLINE';
                cameraStatusBadge.style.backgroundColor = 'rgba(239, 68, 68, 0.7)'; // Red
            }
            
            showNotification(result);
        });
        
        // Initialize status for all devices
        async function updateAllStatus() {
            // Light status
            const lightResult = await sendCommand('light/status');
            lightStatus.innerHTML = '<span class="status-dot"></span> ' + lightResult;
            updateLightUI(lightResult.includes('ON'));
            
            // Thermostat status
            const thermostatStatus = document.getElementById('thermostatStatus');
            const thermostatResult = await sendCommand('thermostat/status');
            thermostatStatus.innerHTML = '<span class="status-dot"></span> ' + thermostatResult;
            
            // Extract current temperature if available
            const tempMatch = thermostatResult.match(/(\d+)°C/);
            if (tempMatch && tempMatch[1]) {
                const currentTemp = parseInt(tempMatch[1]);
                slider.value = currentTemp;
                tempValue.textContent = currentTemp;
            }
            
            // Camera status
            const cameraStatus = document.getElementById('cameraStatus');
            const cameraStatusBadge = document.getElementById('cameraStatusBadge');
            const cameraResult = await sendCommand('camera/status');
            cameraStatus.innerHTML = '<span class="status-dot"></span> ' + cameraResult;
            
            // Update camera status badge
            if (cameraResult.includes('ACTIVE')) {
                cameraStatusBadge.querySelector('span:last-child').textContent = 'ACTIVE';
                cameraStatusBadge.style.backgroundColor = 'rgba(16, 185, 129, 0.7)'; // Green
            } else {
                cameraStatusBadge.querySelector('span:last-child').textContent = 'OFFLINE';
                cameraStatusBadge.style.backgroundColor = 'rgba(239, 68, 68, 0.7)'; // Red
            }
        }
        
        // Update status when page loads
        window.addEventListener('load', updateAllStatus);
    </script>
</body>
</html>
    )";
}

std::string parseApiCommand(const char* request) {
    const char* apiPath = strstr(request, "GET /api/");
    if (!apiPath) return "";
    apiPath += 9;
    
    const char* pathEnd = strstr(apiPath, " ");
    if (!pathEnd) return "";
    
    std::string command(apiPath, pathEnd - apiPath);
    
    return "GET /" + command;
}

DWORD WINAPI handleWebClient(LPVOID lpParam) {
    SOCKET client_socket = (SOCKET)lpParam;
    char buffer[4096] = {0};
    
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        closesocket(client_socket);
        return 0;
    }
    
    buffer[bytesReceived] = '\0'; // Ensure null termination
    
    if (strstr(buffer, "GET / HTTP/1.1") || strstr(buffer, "GET /index.html HTTP/1.1")) {
        std::string html = generateControlPanelHTML();
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: " + 
                              std::to_string(html.length()) + "\r\n\r\n" + html;
        
        send(client_socket, response.c_str(), response.length(), 0);
    }
    else if (strstr(buffer, "GET /api/") != NULL) {
        std::string cmd = parseApiCommand(buffer);
        std::cout << "API Request: " << cmd << std::endl;
        
        std::string result = processCommand(cmd);
        
        size_t pos = result.find("┬░C");
        if (pos != std::string::npos) {
            result.replace(pos, 3, "°C");
        }
        
        std::cout << "API Response: " << result << std::endl; 
        
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=UTF-8\r\nContent-Length: " + 
                              std::to_string(result.length()) + "\r\nAccess-Control-Allow-Origin: *\r\n\r\n" + 
                              result;
        
        send(client_socket, response.c_str(), response.length(), 0);
    }
    else {
        std::string notFound = "Nah Bro, I don't know what you're looking for";
        std::cout << "404 Request: " << buffer << std::endl;
        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain; charset=UTF-8\r\nContent-Length: " + 
                              std::to_string(notFound.length()) + "\r\n\r\n" + notFound;
        
        send(client_socket, response.c_str(), response.length(), 0);
    }
    
    closesocket(client_socket);
    return 0;
}

DWORD WINAPI webServerThread(LPVOID lpParam) {
    SOCKET web_server_fd = (SOCKET)lpParam;
    sockaddr_in address{};
    int addrlen = sizeof(address);
    
    while (true) {
        SOCKET client_socket = accept(web_server_fd, (sockaddr*)&address, &addrlen);
        if (client_socket == INVALID_SOCKET) {
            continue;
        }
        CreateThread(NULL, 0, handleWebClient, (LPVOID)client_socket, 0, NULL);
    }
    return 0;
}

#endif // WEB_SERVER_H 