// require npm package
const mqtt = require('mqtt');

// declare client
const client = mqtt.connect('mqtt://vic4key:zLjTIze0eHqlK5YQ@vic4key.cloud.shiftr.io', {
  clientId: 'javascript'
});

// register "connect" callback
client.on('connect', function() {
  console.log('connected!');

  // subscribe to topic
  client.subscribe('topic/test');
});

// register "message" callback
client.on('message', function(topic, message) {
  console.log(topic + ': ' + message.toString());
});