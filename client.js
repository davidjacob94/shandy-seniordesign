console.log("Client Started...");
var socket = require('socket.io-client')('http://compeng16.ddns.net');
var readline = require('readline');

socket.on('connect', function () {
	console.log("Connected to Socket...\n");
});

socket.on('sckt_ctl', function(msg){
	console.log(msg);
});


// Creates an interface to read lines from the Arduino:
var lineReader = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  terminal: false
});

// Receives a line from arduino
lineReader.on('line', function (message) {
	var msg = message.split(" ");
	switch(msg[0]) {
		case 'sckt_1':
				socket.emit('sckt_1', msg[1]);
			break;
		case 'sckt_2':
				socket.emit('sckt_2', msg[1]);
			break;
		case 'sckt_3':
				socket.emit('sckt_3', msg[1]);
			break;
		default:
	}
});




