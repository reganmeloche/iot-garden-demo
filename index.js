'use strict';

// Importing libraries
const express = require('express');
const bodyParser = require('body-parser');
const mqtt = require('mqtt');
const config = require('./config');
const app = express();

// Setting up MQTT connection
const MQTT_URL = `mqtt://${config.MQTT_USER}:${config.MQTT_PASSWORD}@${config.MQTT_SERVER}:${config.MQTT_PORT}`;
const SUB_TOPIC = 'moisture';
const PUB_TOPIC = 'pump';
const client = mqtt.connect(MQTT_URL);

// Other app setup
app.set('port', (process.env.PORT || 5000));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Handle connecting to MQTT broker
client.on('connect', () => {
    console.log('Connected to MQTT');
    client.subscribe(`${SUB_TOPIC}`, (err) => {
        if (err) {
            console.log(`Subscription error: `, err.message);
        } else {
            console.log(`Subscribed to topic: ${SUB_TOPIC}`);
        }
    });
});

// Handle receiving a message
client.on('message', (topic, message) => {
    console.log(`Received message on topic ${topic}: ${message}`);
});

// Function to trigger the pump
app.post('/water', (req, res) => {
    const { seconds } = req.body;
    console.log(`Triggering pump for ${seconds}...`);
    const milliseconds = seconds * 1000;
    client.publish(`${PUB_TOPIC}`, `${milliseconds}`);
});

// Start the server
app.listen(app.get('port'), () => {
  console.log(`Listening on port: ${app.get('port')}`);
});
