# iot-garden-demo

This is the source-code for an IoT garden project. It allows for two-way communication between a NodeMCU and a web server via an MQTT broker. The NodeMCU has a moisture sensor, which gets read and periodically transmitted to the server. The server exposes an endpoint that allows the user to trigger a pump to water a plant for a requested number of seconds.

Here is the Hackster project: <insert link here>

# Pre-requisites:
- Download Node.js: https://nodejs.org/en/
- Download Postman: https://www.getpostman.com/
- Sign up for CloudMQTT and start an instance of an MQTT broker. There is a free tier you can use: https://www.cloudmqtt.com/
- Follow the instructions on the Hackster project to set up the NodeMCU and the other devices on a breadboard

# Steps:
1. Clone the repo and cd into the folder
2. Upload the code in the .ino file onto the NodeMCU, which will be connected to the moisture sensor and water pump
3. Use `npm start` to run the web server
4. While the NodeMCU is running, you should see periodic console logs of the moisture value on the web server
5. Make a POST request to http://localhost:5000/water with the body { "seconds": 5 }, to trigger the pump to run for 5 seconds

