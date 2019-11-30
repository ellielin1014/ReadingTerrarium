var url = 'https://reading-terrarium.herokuapp.com';

function setup(){
  noCanvas();
}


function getStatus() {
  httpDo(url+'/temp', 'GET', function (res){
  terrariumStatus_temp.innerHTML = res;});

  httpDo(url+'/moisture', 'GET', function (res){
  terrariumStatus_moisture.innerHTML = res;});

  httpDo(url+'/status', 'GET', function (res){
  terrariumStatus_status.innerHTML = res;});

  httpDo(url+'/sound', 'GET', function (res){
  terrariumStatus_sound.innerHTML = res;});

  updateButtonState();
}


function getRange() {
  httpDo(url+'/setpoint_temp_min', 'GET', function (res){
  terrariumRange_temp_min.innerHTML = res;});

  httpDo(url+'/setpoint_temp_max', 'GET', function (res){
  terrariumRange_temp_max.innerHTML = res;});

  httpDo(url+'/setpoint_moisture_min', 'GET', function (res){
  terrariumRange_moisture_min.innerHTML = res;});

  httpDo(url+'/setpoint_moisture_max', 'GET', function (res){
  terrariumRange_moisture_max.innerHTML = res;
  });
}



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

  var data=$(document.getElementsByTagName("form")[0]).closest('form').serialize();

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
  getStatus();
       }
   });
}

function updateButtonState(){
  //e.preventDefault();
  var buttonState = 'buttonState=1';
  var data = buttonState;

  $.ajax({
        url:'playPause',
        type:'POST',
        data: data,
        success:function(){
            buttonState = 'buttonState=0';
       }
   });
}
