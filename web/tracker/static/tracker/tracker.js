var count  = 0;
var ping_pong_times = [];

$(document).ready(function () {
  
    // Decide entre ws:// and wss://
    var ws_scheme   = window.location.protocol == "https:" ? "wss" : "ws";
    //var ws_path     = ws_scheme + '://' + window.location.host + "/tracker/stream/";
    //var ws_path     = ws_scheme + "://gtracker.com:8443/tracker/stream/";
    var ws_path     = ws_scheme + '://' + window.location.host + "/ws/tracker/";
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

        $('#log').append(
            '<a href="#" class="list-group-item list-group-item-action">'+
            '<div class="media">'+
                '<img class="d-flex mr-3 rounded-circle" src="http://placehold.it/45x45" alt="">'+
                '<div class="media-body">'+
                    message.data +
                    '<div class="text-muted smaller">Atualizado em: ' + 
                        ((new Date()).getTime()/1000.0).toString()+ 
                    '</div>'+
                '</div>'+
            '</div>'+
            '</a>'
        );

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

        } else if (data.pong) {

            handle_pingpong(data);
    
        } else {

            console.log("Ops !!! Não foi possivel manusear a mensagem enviada pelor servidor !");
        }
    };

    // Ping periodico (medição de latencia)
    // Função inicia o teste de latencia enviando o ping e zera o cronometro
    window.setInterval(function() {
        start_time = (new Date()).getTime();
        socket.send(JSON.stringify({
        
            "command": "ping", 
        }));

    }, 2000);
    

    $('#bt_connection').click(function(ev){

        console.log("Valor: " + $('#txt_nr_tracker').val());

        socket.send(JSON.stringify({
            
            "command": "tracker_connect",
            
            "nr_tracker": $('#txt_nr_tracker').val(),
        }));
 
    });
});

// Evento pra tratar o envio de dados feito pelo servidor atualizando a tabela.
// A callback sera invocada sempre que o server enviar dados para o clie
function handle_tlm(data) {
    
    count++;

    tlm = data.telemetry;

    $('#count').text(count);
    $('#lat').text(tlm.lat);
    $('#lng').text(tlm.lng);
    $('#acce_X').text(tlm.acce.X);
    $('#acce_Y').text(tlm.acce.Y);
    $('#acce_Z').text(tlm.acce.Z);
    $('#acce_XG').text(tlm.acce_G.X);
    $('#acce_YG').text(tlm.acce_G.Y);
    $('#acce_ZG').text(tlm.acce_G.Z);
    $('#lock').text(tlm.lock);
    $('#timestamp').text(tlm.timestamp_tlm);
    $('#tanque').text(tlm.level);
}
       
// Retorno para o teste de latencia
// Essa função será invocada quando o server enviar o pong como command. 
// O delta de tempo e computado e o resultado registrado em uma lista 
// para o calculo da media dos 10 ultimos valores
function handle_pingpong(data) {

    var latency = (new Date()).getTime() - start_time;
    ping_pong_times.push(latency);
    ping_pong_times = ping_pong_times.slice(-30); // keep last 30 samples
    var sum = 0;
    for (var i = 0; i < ping_pong_times.length; i++){
        sum += ping_pong_times[i];
    }
    $('#ping-pong').text(Math.round(10 * sum / ping_pong_times.length) / 10);
}
       