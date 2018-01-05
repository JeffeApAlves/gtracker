var samples = [];

// Chart.js scripts
// -- Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily = '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#292b2c';

function factoryDataChar(){

    var dataChart = {
        labels: [],
        datasets: [{
            label: "Sessions",
            lineTension: 0.3,
            backgroundColor: "rgba(2,117,216,0.2)",
            borderColor: "rgba(2,117,216,1)",
            pointRadius: 5,
            pointBackgroundColor: "rgba(2,117,216,1)",
            pointBorderColor: "rgba(255,255,255,0.8)",
            pointHoverRadius: 5,
            pointHoverBackgroundColor: "rgba(2,117,216,1)",
            pointHitRadius: 20,
            pointBorderWidth: 2,
            data: [],
        }],
    }

    var configChart = {
        type: 'line',
        data: dataChart,
        options: {
        scales: {
            xAxes: [{
            time: {
                unit: 'date'
            },
            gridLines: {
                display: false
            },
            ticks: {
                maxTicksLimit: 7
            }
            }],
            yAxes: [{
            ticks: {
                min: -2.0000,
                max: 2.0000,
                maxTicksLimit: 5
            },
            gridLines: {
                color: "rgba(0, 0, 0, .125)",
            }
            }],
        },
        legend: {
            display: false
        }
        }
    };

    return configChart;
}

var canvas_X = document.getElementById("chart_X");
var chart_X = new Chart(canvas_X,factoryDataChar());

var canvas_Y = document.getElementById("chart_Y");
var chart_Y = new Chart(canvas_Y,factoryDataChar());

var canvas_Z = document.getElementById("chart_Z");
var chart_Z = new Chart(canvas_Z,factoryDataChar());

$(document).ready(function () {
    
      // Decide entre ws:// and wss://
      var ws_scheme   = window.location.protocol == "https:" ? "wss" : "ws";
      var ws_path     = ws_scheme + '://' + window.location.host + "/tracker/stream/";
      console.log("Conecatando em " + ws_path);
      var socket = new ReconnectingWebSocket(ws_path);
      
      socket.onopen = function () {
  
          console.log("Conectado no ws: " + ws_path);
      };
  
      socket.onclose = function () {
  
          console.log("Desconectado do ws: "+ ws_path);
      };
  
      // Hook para processa menssagem recebidoas pelo server
      socket.onmessage = function (message) {
  
          console.log("Got ws message " + message.data);
     
          // Decode the JSON
          var data = JSON.parse(message.data);
     
          // Handle errors
          if (data.error) {
  
              alert(data.error);
              return;
          }
          
          if (data.telemetry) {
  
              handle_tlm(data);
  
          } else {
  
              console.log("Ops !!! Não foi possivel manusear a mensagem enviada pelor servidor !");
          }
      };
/*  
      window.setInterval(function() {
       
      }, 2000);
 */     
  
      $('#bt_connection').click(function(ev){
  
          console.log("Valor: " + $('#txt_nr_tracker').val());
  
          socket.send(JSON.stringify({
              
              "command": "tracker_connect",
              
              "nr_tracker": $('#txt_nr_tracker').val(),
          }));
   
      });
  });
  
function handle_tlm(data){

    x =  parseFloat(data.telemetry.acce_G.X);
    y = parseFloat(data.telemetry.acce_G.Y);
    z =  parseFloat(data.telemetry.acce_G.Z);
    label =  data.telemetry.timestamp_tlm;

    var sample = {

        x: x,
        y: y,
        z: z,
        label: label,
    };

    samples.push(sample);
    samples = samples.slice(-15);

    for (var i = 0; i < samples.length; i++){

        chart_X.data.datasets[0].data[i] = samples[i].x;
        chart_Y.data.datasets[0].data[i] = samples[i].y;
        chart_Z.data.datasets[0].data[i] = samples[i].z;
        chart_X.data.labels[i] = samples[i].label;
        chart_Y.data.labels[i] = samples[i].label;
        chart_Z.data.labels[i] = samples[i].label;
    }

    chart_X.update();
    chart_Y.update();
    chart_Z.update();
}