var path = require('path');
var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var fs = require('fs');

// Initlizes the 2D array to store power consumption data
var sckt_data = new Array(3);
sckt_data[0] = [];
sckt_data[1] = [];
sckt_data[2] = [];

var numMinutes = 0;

// Delivers the index.html file when the server receives a GET request
app.get('/', function(req,res){
  res.sendFile(__dirname + '/index.html');
});

// Points the program to the directory where it can find static files
app.use(express.static('files'));

	// Writes the header of the .csv files. writeFile overwrites exsisting files
	fs.writeFile("files/data/dat" + 0 + ".csv", "time,power\n", function(err) {
		if(err) {
			return console.log(err);
		}
	});
	fs.writeFile("files/data/dat" + 1 + ".csv", "time,power\n", function(err) {
		if(err) {
			return console.log(err);
		}
	});
	fs.writeFile("files/data/dat" + 2 + ".csv", "time,power\n", function(err) {
		if(err) {
			return console.log(err);
		}
	});

io.on('connection', function(socket){
	socket.on('command', function(msg){
		io.emit('command', msg);
	});
	
	// Calls the function logData every 600000 ms
	var logTimer = setInterval(logData, 600000);
	
	socket.on('sckt_1', function(msg){
		io.emit('sckt_1', msg);
		sckt_data[0].push(msg);
	});
	socket.on('sckt_2', function(msg){
		io.emit('sckt_2', msg);
		sckt_data[1].push(msg);
	});
	socket.on('sckt_3', function(msg){
		io.emit('sckt_3', msg);
		sckt_data[2].push(msg);
	});
	
	socket.on('sckt_ctl', function(msg){
		io.emit('sckt_ctl', msg);
	});
});

// Funtion to append power consumption data to the .csv file
function logData() {
	numMinutes++;
	var sum;
	for( var n = 0; n < 3; n++ ){
		if(sckt_data[n].length == 0) {
			avg = 0;
		} else {
			sum = 0;
			for( var i = 0; i < sckt_data[n].length; i++ ){
				sum += parseInt( sckt_data[n][i], 10 );
			}
			var avg = sum / sckt_data[n].length;
			avg = Math.floor(avg);
		}
		fs.appendFile("files/data/dat" + n + ".csv", numMinutes + "," + avg + "\n", function(err) {
			if(err) {
				return console.log(err);
			}
		});
		sckt_data[n] = [];
	}
	
}

// Listens on port 80 for HTTP request
http.listen(80, function(){
	console.log('listening on *:80');
});
