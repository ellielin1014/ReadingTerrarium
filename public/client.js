var socket = io();
var terrariumData;

socket.on('thermostat', data => {
  terrariumData = data;
  terrariumStatus_temp.innerHTML = terrariumData.temp;
  terrariumStatus_moisture.innerHTML = terrariumData.moisture;
  terrariumStatus_status.innerHTML = terrariumData.status;

  if(terrariumData.status = 'healthy'){
  terrariumStatus_sound.innerHTML = terrariumData.song;
  } else{
    terrariumStatus_sound.innerHTML = terrariumData.story;
  }
});


socket.on('range', data => {
  terrariumData = data;
  terrariumRange_temp_min.innerHTML = terrariumData.setpoint_temp_min;
  terrariumRange_temp_max.innerHTML = terrariumData.setpoint_temp_max;
  terrariumRange_moisture_min.innerHTML = terrariumData.setpoint_moisture_min;
  terrariumRange_moisture_max.innerHTML = terrariumData.setpoint_moisture_max;
});




function getStatus(endpoint) {
  fetch(endpoint)
}

function getRange(endpoint) {
  fetch(endpoint)
}
