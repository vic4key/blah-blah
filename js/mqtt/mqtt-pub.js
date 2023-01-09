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
  // client.subscribe('hello');

  // publish message every second
  setInterval(function() {
    client.publish('topic/test', 'hello world');
  }, 1000);
});

// register "message" callback
client.on('message', function(topic, message) {
  console.log(topic + ': ' + message.toString());
});