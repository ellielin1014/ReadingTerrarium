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


function takeValues(){

  var temp_min = document.forms["setRange"]["temp_min"].value;
  var temp_max = document.forms["setRange"]["temp_max"].value;
  var moisture_min = document.forms["setRange"]["moisture_min"].value;
  var moisture_max = document.forms["setRange"]["moisture_max"].value;

if ((temp_min || temp_max || moisture_min || moisture_max) === ''){
  alert("Please setup the range");
} else {
  SubForm();
  }
}

function SubForm(){
  //e.preventDefault();
  var temp_min = document.forms["setRange"]["temp_min"].value;
  var temp_max = document.forms["setRange"]["temp_max"].value;
  var moisture_min = document.forms["setRange"]["moisture_min"].value;
  var moisture_max = document.forms["setRange"]["moisture_max"].value;

  var  data=$(document.getElementsByTagName("form")[0]).closest('form').serialize();

  console.log(data);
  $.ajax({
        url:'#',
        type:'POST',
        data: data,
        success:function(){
  terrariumRange_temp_min.innerHTML = temp_min;
  terrariumRange_temp_max.innerHTML = temp_max;
  terrariumRange_moisture_min.innerHTML = moisture_min;
  terrariumRange_moisture_max.innerHTML = moisture_max;
  document.forms["setRange"]["temp_min"].value = '';
  document.forms["setRange"]["temp_max"].value = '';
  document.forms["setRange"]["moisture_min"].value = '';
  document.forms["setRange"]["moisture_max"].value = '';
       }
   });
}

function getStatus(endpoint) {
  fetch(endpoint)
}

function getRange(endpoint) {
  fetch(endpoint)
}
