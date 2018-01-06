$(document).ready(function () {
  
    // Correctly decide between ws:// and wss://
    var ws_scheme = window.location.protocol == "https:" ? "wss" : "ws";
    var ws_path = ws_scheme + '://' + window.location.host + "/ws/chat/";
    console.log("Connecting to " + ws_path);
    var socket = new ReconnectingWebSocket(ws_path);

    // Debug
    socket.onopen = function () {
        console.log("Conectado no WebSocket chat");
    };

    // Debug
    socket.onclose = function () {
        console.log("Desconectado do websocket chat");
    };

    // Processa menssagem
    socket.onmessage = function (message) {
        // Debug
        console.log("Got websocket message " + message.data);
   
        // Decode the JSON
        var data = JSON.parse(message.data);
   
        // Handle errors
        if (data.error) {
            alert(data.error);
            return;
        }
        
        if (data.join) {

            handle_join(data);

        } else if (data.leave) {

            handle_leave(data);

    
        } else if (data.message) {

            handle_message(data);

        } else {

            console.log("Cannot handle message!");
        }
    };

    function handle_join(data) {

        console.log("Joining room " + data.join);
        //$('#log').append('<p>Participando do grupo ' + data.join + '</p>');
        $('#log').append('<li class="list-group-item">Entrou do grupo: ' + data.join + '</li>');
    }

    function handle_leave(data) {

        console.log("Leaving room " + data.leave);
        $('#log').append('<li class="list-group-item">Saiu do grupo: ' + data.leave + '</li>');
    }

    function handle_message(data) {

        console.log("Receive message: " + data.message);
        $('#log').append('<li class="list-group-item">Mensagem recebida: ' + data.message.chat_msg + '</li>');
    }

    $("form#join button").click(function(ev){

        ev.preventDefault();  // cancel form submission
        
        var room_name  = $('#room_name').val();

        if($(this).attr("value")=="join-room"){
    
            socket.send(JSON.stringify({
            
                "room": room_name,
                "command": "join", 
            
            }));

        } else if($(this).attr("value")=="leave-room"){

            socket.send(JSON.stringify({
            
                "room": room_name,
                "command": "leave", 
            
            }));
            
        } else if($(this).attr("value")=="close-room"){

            socket.send(JSON.stringify({
            
                "command": "close_room", 
                "room": room_name,
            
            }));
        }

//        $("form#join").submit();

        return false;
    });

   $("form#send button").click(function(ev){
        
        ev.preventDefault();    // cancel form submission
        
        var room_name   = $('#room_name').val();
        var chat_msg    =  $('#chat_msg').val();

        if($(this).attr("value")=="echo"){

            socket.send(JSON.stringify({
            
                "room": room_name,
                "command": "send_echo",
                "chat_msg": chat_msg, 
            
            }));
 
        } else if($(this).attr("value")=="to-room"){

            socket.send(JSON.stringify({
            
                "room": room_name,
                "command": "send_room",
                "chat_msg": chat_msg
            }));
            
        }else if($(this).attr("value")=="to-all"){

            socket.send(JSON.stringify({
            
                "room": room_name,
                "command": "send_all",
                "chat_msg": chat_msg, 
            
            }));
        }

//        $("form#send").submit();

        return false;
    });


    $('form#disconnect').on('submit',function(event) {

        var room_name  = $('#room_name').val();

        socket.send(JSON.stringify({
        
            "room": room_name,
            "command": "disconnect", 
        
        }));
        
        return false;
    });
  
    $('form#join').submit(function(event) {

        //Debug
        console.log("submit");
        return false;
    });

});