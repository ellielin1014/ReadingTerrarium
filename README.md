Reading Terrarium API
=================

### Intro

According to research by the Royal Horticultural Society, talking to your plants can help them grow faster.
This is an API for monitoring your terrarium's enivronment (temperature & soil moisture level).
If your terrarium is in an healthy environment, it will sing to you. If it is not, you can read to it to help it grow.

### Methods

Copy the module and start to use the following functions:

##### The following methods are used to check and update the terrarium's environment conditions #####

1. Get terrarium's status, returns "healthy" or "unhealthy"
```json
GET /thermostat/status
```

2. Check healthy temperature range set for terrarium
```json
GET /range/setpoint_temp_max
GET /range/setpoint_temp_min
```

3. Set/adjust optiminal temperaute range for terrarium
```json
POST /range/setpoint_temp_max/:setpoint_temp_max
POST /range/setpoint_temp_min/:setpoint_temp_min
```

4. Check healthy soil moisture range set for terrarium
```json
GET /range/setpoint_moisture_max
GET /range/setpoint_moisture_min
```

3. Set/adjust optiminal soil moisture range for terrarium
```json
POST /range/setpoint_moisture_max/:setpoint_moisture_max
POST /range/setpoint_moisture_min/:setpoint_moisture_min

```
##### The following method pertains feedback by/to the terrarium #####

1. Check the current song or story that will be played either by the terrarium or you
```json
GET /thermostat/song
GET /thermostat/story
```

### JSON format
1. Set the range of optimal temperature and soil mositure, data will return as:
```javascript
range = {
   'setpoint_temp_min': 10.0,       
   'setpoint_temp_max': 30.0,
   'setpoint_moisture_min': 20.0,
   'setpoint_moisture_max': 40.0
}
```
2. Check the current terrarium environment, data will return as:
```javascript
thermostat = {
   'temp': 14.0,            
   'moisture': 30.0,
   'status': 'healthy', //healthy or unhealthy
   'sound': ' ' //return either story or song in a string
}
```
### Arduino
1. After setting the range of optimal temperature and soil moisture level (do once/whenever you want to adjust the level), you can click on the "Get Status" button at any time to see if your terrarium is in a healthy environment. The server will send "1 song" if the terrarium is healthy, and send "1 story" if the terrarium is unhealthy. The background color of the web interface will also change: it will be lighter if your terarrium is healthy, and darker if it is unhealthy. 
