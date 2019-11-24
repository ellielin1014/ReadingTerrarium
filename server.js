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

let express = require('express');	         // include the express library
const app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);				         // create a server using express
let bodyParser = require('body-parser');	   // include body-parser

// server.listen(process.env.PORT);
server.listen(8080);           // start the server

app.use('/', express.static('public'));   // app static files from /public

app.get("/", function(request, response) {
  response.sendFile(__dirname + "/index.html");
});


// make a data object representing all your parameters:
let range = {
   'setpoint_temp_min': 10.0,        // the temperature setpoint
   'setpoint_temp_max': 30.0,
   'setpoint_moisture_min': 20.0,
   'setpoint_moisture_max': 40.0
}

let thermostat = {
   'temp': 120.0,            // temperature in degrees C, a float
   'moisture': 30.0,
   'status': 'healthy',      // healthy, unhealthy
   'song': 'A song',         // the name of the song, string
   'story': 'A story'        // the name of the story, string
}



//handle GET requests
app.get("/thermostat", function(request, response) {
  thermostat.status = (thermostat.temp > range.setpoint_temp_min && thermostat.temp < range.setpoint_temp_max && thermostat.moisture > range.setpoint_moisture_min && thermostat.moisture < range.setpoint_moisture_max) ? 'healthy' : 'unhealthy';
  io.emit('thermostat', thermostat);
});

app.get("/range", function(request, response) {
  io.emit('range', range);
});

//handle POST requests
app.post("/", function(request, response){
  var body = "";
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

    console.log(range);
  });



  request.on('end', function(){
    console.log('POSTed: ' + body);
    response.writeHead(200);
    response.end();
  });
});
