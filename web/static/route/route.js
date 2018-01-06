var map;
var socket;
var directionsService;
var directionsDisplay;
var geocoder;

const STATUS_GUI = {
    INIT:0,
    INIT_MAP_OK:1,
    EMPTY_ROUTE:2,
    NEW_FENCE:3,
    ADD_POINTS:4,
    CONFIRM_FENCE:5,
    START_POINT_OK:6,
    END_POINT_OK:7,
};

var statusGUI = STATUS_GUI.INIT; 

$(document).ready(function () {

    var ws_scheme = window.location.protocol == "https:" ? "wss" : "ws";
    var ws_path     = ws_scheme + '://' + window.location.host + "/ws/tracker/";
    console.log("Conecatando em " + ws_path);
    socket = new ReconnectingWebSocket(ws_path);
 
    socket.onopen = function () {

        console.log("Conectado no ws: " + ws_path);
    };
        
    socket.onclose = function () {

        console.log("Desconectado do ws: " + ws_path);
    };

    // Hook para processa menssagem recebidoas pelo server
    socket.onmessage = function (message) {
        
        $('#log').append('<p class="list-group-item">' + message.data + '</p>');

        console.log("Got websocket message " + message.data);
    
        // Decode the JSON
        var data = JSON.parse(message.data);
    
        // Handle errors
        if (data.error) {

            alert(data.error);
            return;
        }
        
        //Hook de processamento das menssagens
        if (data.telemetry) {

            handle_tlm(data.telemetry);

        }  else {

            console.log("Ops !!! Não foi possivel manusear o tipo de mensagem recebida !");
        }
    };

    document.getElementById('submit').addEventListener('click', function() {
 
        calculateAndDisplayRoute(directionsService, directionsDisplay);
    });

    statusGUI = STATUS_GUI.EMPTY_ROUTE;
});

// Evento pra tratar o envio das informações de telemetria enviada pelo servidor.
// Sera invocada sempre que o servidor enviar dados
function handle_tlm(telemetry) {

    updatePosition(telemetry);
    updateProgressBar('#acce_X',telemetry.acce_G.X,0.0,4.0);
    updateProgressBar('#acce_Y',telemetry.acce_G.Y,0.0,4.0);
    updateProgressBar('#acce_Z',telemetry.acce_G.Z,0.0,4.0);
    updateProgressBar('#vel',telemetry.speed,0.0,300);
}    

function updatePosition(telemetry){

    var pos = {
        lat: parseFloat(telemetry.lat),
        lng: parseFloat(telemetry.lng),
    };

    var marker = new google.maps.Marker({
        
        position:   pos,
        map:        map,
        title:      '',
    });

    map.setCenter(pos);
}

function updateProgressBar(bar,val_str, min,max){

    var val = parseFloat(val_str);

    var r = Math.round((val*100)/max);
    var r_str = r.toString();

    $(bar).attr('aria-valuenow', r_str).css('width',r_str+"%");

}

// Inicializa a entidade map (Google)
function initMap() {

    //Seviços de direncionamento
    directionsService       = new google.maps.DirectionsService();
    
    // Renderização ddos objetos visuais relacionado a direção
    directionsDisplay       = new google.maps.DirectionsRenderer();

    //Geolocalização
    geocoder                = new google.maps.Geocoder();
    
    //Coordenada SENAi Anchieta
    const SENAI_ANCHIETA    = new google.maps.LatLng(-23.591387, -46.645126);

    //Entidade mapa
    map = new google.maps.Map(document.getElementById('map'), {
        center: SENAI_ANCHIETA,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        zoom: 15
    });
    
    //Configura saídas da direção
    directionsDisplay.setMap(map);
    directionsDisplay.setPanel(document.getElementById('directions-panel'));
/*
    // Tenta localizar a geolocalização do navagador.
    if (navigator.geolocation) {

        navigator.geolocation.getCurrentPosition(function(position) {
        
            var pos = {
                lat: position.coords.latitude,
                lng: position.coords.longitude
            };

            map.setCenter(pos);

        }, function() {
            handleLocationError(true, map.getCenter());
        });
    } else {
        
        // Browser não suporta geolocalização
        handleLocationError(false,  map.getCenter());
    }

    var marker = new google.maps.Marker({
    
        position:   SENAI_ANCHIETA,
        map:        map,
        title:      ''
    });
*/
    // Evento de alteração do centro
//    map.addListener('center_changed', function() {
//
//        window.setTimeout(function() {
//          map.panTo(marker.getPosition());
//        }, 2000);
//    });


    // Evento on click do mapa 
    google.maps.event.addListener(map, 'click', function(event) {

        registerRoute(event.latLng);
    });

    // Indica que foi inicializado
    statusGUI = STATUS_GUI.INIT_MAP_OK;
}

/*
 * Calcula e mostra a rota
 */
function calculateAndDisplayRoute(directionsService, directionsDisplay) {

    directionsService.route({
        origin: start_position,
        destination: end_position,
        optimizeWaypoints: false,
        travelMode: 'DRIVING'
    }, function(response, status) {

        if (status === 'OK') {

            directionsDisplay.setDirections(response);
            
            var route = response.routes[0];
            
            //var summaryPanel = document.getElementById('summary-directions');
            //summaryPanel.innerHTML = '';
            
            // Resumo de informações para cada rota.
            for (var i = 0; i < route.legs.length; i++) {
//                var routeSegment = i + 1;
//                summaryPanel.innerHTML += '<b>Rota: ' + routeSegment +'</b><br>';
//                summaryPanel.innerHTML += 'Inicial(lat):' + route.legs[i].start_location.lat() + '<br>';
//                summaryPanel.innerHTML += 'Inicial(lng):' + route.legs[i].start_location.lng() + '<br>';
//                summaryPanel.innerHTML += 'Final(lat):'   + route.legs[i].end_location.lat() + '<br>' ;
//                summaryPanel.innerHTML += 'Final(lat):'   + route.legs[i].end_location.lng() + '<br>';
  
                $('#lat_inicial').text(parseFloat(route.legs[i].start_location.lat().toFixed(5)));
                $('#lng_inicial').text(parseFloat(route.legs[i].start_location.lng().toFixed(5)));
                $('#lat_final').text(parseFloat(route.legs[i].end_location.lat().toFixed(5)));
                $('#lng_final').text(parseFloat(route.legs[i].end_location.lng().toFixed(5)));                
 
                socket.send(JSON.stringify({
                    
                    "command":"route",
                    "route": route,
                    "nr_tracker": $('#txt_nr_tracker').val(),
                }));
            }
        } else {
            
            window.alert('Falha no pedido das direções: ' + status);
        }
    });
}

// Usada para definir os pontos de origeme e destino da rota
function registerRoute(latlng){


    if(statusGUI==STATUS_GUI.EMPTY_ROUTE){

        createMarker("Origem",latlng);

        setStartPosition(latlng);

    }else if(statusGUI==STATUS_GUI.START_POINT_OK){

        createMarker("Destino",latlng);
        
        setEndPosition(latlng);
    }
}

// Cria um infowindow com a coordenada e o endereço correspondente
function createInfo(marker){

    var content = 
        '<div>'+
            '<h1>'+marker.title+'</h1>'+
            '<div>'+
                '<p>Latitude:  ' + marker.position.lat() + '</p>'+
                '<p>Longitude: ' + marker.position.lng() + '</p>'+
                '<p>Endereço: '  + marker.address + '</p>'+
            '</div>'+
        '</div>';

    var infowindow = new google.maps.InfoWindow({
        content: content
    });

    infowindow.open(map, marker);

    // Evento para fechar automaticamente o infowindow
    google.maps.event.addListener(infowindow, 'domready', function(){

        window.setTimeout(function() {
            infowindow.close();
        }, 5000);
    });
}

function createMarker(title,latlng){

    var icon = 'https://cdn2.iconfinder.com/data/icons/snipicons/500/map-marker-32.png';
    
    var marker = new google.maps.Marker({

        position: latlng, 
        map: map,
        draggable: true,
        icon: icon,
        title: title,
        typeMarker: title,
        address: '',
    });

    geocodeLatLng(marker);

//    marker.addListener('drag', onDragEvent);
    marker.addListener('dragend', onDragEvent);
    marker.addListener('click', function() {
        
        map.setCenter(marker.getPosition());
    
        createInfo(marker);        
    });
}

// Define o ponto de origem da rota e atualiza o painel
function setStartPosition(position){

    start_position = position;

    $('#start_lat').val(position.lat());
    $('#start_lng').val(position.lng());

    statusGUI = STATUS_GUI.START_POINT_OK;
}

// Define o ponto de destino da rota e atualiza o painel
function setEndPosition(position){

    end_position =position;

    $('#end_lat').val(position.lat());
    $('#end_lng').val(position.lng());
    
    statusGUI = STATUS_GUI.END_POINT_OK;
}

// Evento drag drop do marcador
function onDragEvent(event) {

    if(this.typeMarker=="Origem"){

        setStartPosition(this.position);
        geocodeLatLng(this);
    }
 
    if(this.typeMarker=="Destino"){
        
        setStartPosition(this.position);
        geocodeLatLng(this);
    }
}

// Chamada quando algum erro acorreu durante a localização do navegador
function handleLocationError(browserHasGeolocation, pos) {

    var infoWindow  = new google.maps.InfoWindow({map: map});
    
    infoWindow.setPosition(pos);
    infoWindow.setContent(browserHasGeolocation ?
                                'Ops! Não foi possivel obter a localização do navegador.' :
                                'Error: Your browser doesn\'t support geolocation.');
}

// Executa a geolocalização inversa e atualiza o painel de informações
function geocodeLatLng(marker) {
    
    geocoder.geocode({'location': marker.position}, function(results, status) {

        if (status === 'OK') {

            if (results[1]) {

                // Preenche o endereço correspondente da coordenada
                marker.address = results[1].formatted_address;

                createInfo(marker);
            
            } else {

                window.alert('Não encontrado resultado para pesquisa');
            }
            
        } else {

            window.alert('Falha no Geocoder: ' + status);
        }
    });
}