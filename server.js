/*
   REST Test
   A minimal server to test a RESTful API.
   Modify to match your API.

   The RESTful API in this case is for a networked thermostat.
   The endpoints are:
   /temp              - temperature, a float
   /moisture          - moisture, a float
   /setpoint_temp_min - the temperature setpoint minimum
   /setpoint_temp_max - the temperature setpoint maximum
   /setpoint_moisture_min - the moisture setpoint miniumn
   /setpoint_moisture_max - the moisture setpoint maximum
   /status            - healthy, unhealthy
   /song              - the name of the song, a string
   /story             - the name of the story, a string

   You can GET the value of any of the endpoints, or
   you can POST to them.
*/

var express = require('express');
var https = require('https');
var http = require('http');
const server = express();
var fs = require('fs');
var bodyParser = require('body-parser');

// var options = {
//   key: fs.readFileSync('./keys/domain.key'),
//   cert: fs.readFileSync('./keys/domain.crt')
// };

//server.use('*', httpRedirect);
server.use('/', express.static('public'));

// function httpRedirect(request, response, next) {
//   if (!request.secure) {
//     console.log("redirecting http request to https");
//     response.redirect('https://' + request.hostname + request.url);
//   } else {
//     next();
//   }
// }

http.createServer(server).listen(process.env.PORT || 443);
// http.createServer(server).listen(8080);
//https.createServer(options, server).listen(443);


server.get("/", function(request, response) {
  response.sendFile(__dirname + "/index.html");
});

server.use(bodyParser.json()); 						  // for  application/json
server.use(bodyParser.urlencoded({extended: false}));

// make a data object representing all your parameters:
let range = {
   'setpoint_temp_min': 10.0,        // the temperature minimum setpoint
   'setpoint_temp_max': 30.0,        // the temperature maximum setpoint
   'setpoint_moisture_min': 20.0,    // the moisture maximum setpoint
   'setpoint_moisture_max': 40.0     // the moisture minimum setpoint
}

let thermostat = {
   'temp': 120.0,            // current temperature
   'moisture': 30.0,         // current moisture
   'status': 'healthy',      // current status, healthy or unhealthy
   'sound': ''               // the name of the sound, song or story, string
}

let buttonState = {
   'buttonState': 0          // current temperature
}


//--------------------------------------------------------------------------
//handle GET requests
function handleGetRequest(request, response) {
  thermostat.status = (thermostat.temp > range.setpoint_temp_min && thermostat.temp < range.setpoint_temp_max && thermostat.moisture > range.setpoint_moisture_min && thermostat.moisture < range.setpoint_moisture_max) ? 'healthy' : 'unhealthy';
  thermostat.sound = (thermostat.temp > range.setpoint_temp_min && thermostat.temp < range.setpoint_temp_max && thermostat.moisture > range.setpoint_moisture_min && thermostat.moisture < range.setpoint_moisture_max) ? 'song' : 'story';
  let currentStat = '';
  switch (request.path) {
    case '/temp':
      currentStat = thermostat.temp;
      break;
    case '/moisture':
      currentStat = thermostat.moisture;
      break;
    case '/status':
      currentStat = thermostat.status;
      break;
    case '/sound':
      currentStat = thermostat.sound;
      break;
  }
  response.send(currentStat.toString());
};

function handleGetRangeRequest(request, response) {
  let setRange = '';
  switch (request.path) {
    case '/setpoint_temp_min':
      setRange = range.setpoint_temp_min;
      break;
    case '/setpoint_temp_max':
      setRange = range.setpoint_temp_max;
      break;
    case '/setpoint_moisture_min':
      setRange = range.setpoint_moisture_min;
      break;
    case '/setpoint_moisture_max':
      setRange = range.setpoint_moisture_max;
      break;
  }
  response.send(setRange.toString());
};


function handlePlayRequest(request, response) {
  let buttonString = '';
  if(request.path == '/playPause') {
    buttonString = buttonState.buttonState;
  }
  response.send(buttonString.toString());

};




//--------------------------------------------------------------------------
//handle POST requests

// 1. handling request from the Arduino client
server.post("/thermostat", function (request, response) {
  var sensorReading = '';
  request.on('data', function(chunk){
    sensorReading += chunk;
        for (let i = 0; i<2; i++){
      var allData = sensorReading.split("&");
      var oneData = allData[i].split("=");

      switch(i){
        case 0:
          thermostat.temp = oneData[1];
          break;

        case 1:
          thermostat.moisture = oneData[1];
          break;
      }
    }
  });

  request.on('end', function(){
    response.writeHead(200);
    response.end();
  });
});


// 2. handling request from the web client
server.post("/", function(request, response){
  var body = '';
  request.on('data', function(chunk){
    body += chunk;

    for (let i = 0; i<4; i++){
      var allData = body.split("&");
      var oneData = allData[i].split("=");

      switch(i){
        case 0:
          range.setpoint_temp_min = oneData[1];
          break;

        case 1:
          range.setpoint_temp_max = oneData[1];
          break;

        case 2:
          range.setpoint_moisture_min = oneData[1];
          break;

        case 3:
          range.setpoint_moisture_max = oneData[1];
          break;
      }
    }

  });

  request.on('end', function(){
    response.writeHead(200);
    response.end();
  });
});


// 3. handling buttonState from the web client
server.post("/playPause", function (request, response) {
  var comingData = '';
  request.on('data', function(chunk){
    comingData += chunk;



        for (let i = 0; i<1; i++){

      var oneData = comingData.split("=");

      switch(i){
        case 0:
          buttonState.buttonState = oneData[1];
          break;
      }
    }
  });

  request.on('end', function(){
    response.writeHead(200);

    response.end();
  });
});



server.get('/temp', handleGetRequest);
server.get('/moisture', handleGetRequest);
server.get('/status', handleGetRequest);
server.get('/sound', handleGetRequest);
server.get('/setpoint_temp_min', handleGetRangeRequest);
server.get('/setpoint_temp_max', handleGetRangeRequest);
server.get('/setpoint_moisture_min', handleGetRangeRequest);
server.get('/setpoint_moisture_max', handleGetRangeRequest);
server.get('/playPause', handlePlayRequest);
