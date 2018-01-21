# GTracker

O objetivo desse projeto é desenvolver uma plataforma web que relaciona valores coletados de telemetria veicular (acelerômetros, GPS, ...) a eficiência enérgica do veículo com o modo de condução (acelerações e frenagens bruscas) e o tráfico das vias da região. 

### Objetivos adicionais
Comparar diferentes tipos de serialização de payload  (JSON, BJSON e CBOR) e seus impactos no consumo de banda e latência  

## Comunicação 
Foram implementados 2 tipos de comunicação serial (RS 232) e via broker de mensagens (rabbitmq). Para a camada de domínio a comunicação é
realizada por meio de um gerenciador de dispositivos que abstrai a uma mesma interface, sendo assim, é totalmente transparente a forma de comunicação para esse layer.

## Embarcado firmware ARM M0+:
Utilização do FreeRTOS com filas de mensagens para troca de informações entre as tasks e sinalizado através de TaskNotification. 
A comunicação via UART com o Host é totalmente assíncrona. O device possui tasks independentes para recepção e transmissão. Após recepção e validação do frame o payload é postado em uma fila de entrada e sinalizado para a task da camada de aplicação que fará o consumo e o processamento respectivo. Se algum processamento da camada de aplicação gerar alguma resposta que deve ser transmitida para o host. Essa resposta será colocada em uma fila de saída para a camada de comunicação fazer o empacotamento e envio. 
Por ser tratar de eventos assíncronos os frames possuem um timestamp para controle de sequência. Todo o firmware está orientado a eventos.
Segue o diagrama de tasks 

![Diagrama de tasks][tasks_diagram]

Abaixo podemos verificar como está o uso de memória e fazer uma comparação do impacto do uso do **TaskNotification**.


![Stack **sem** TaskNotification][task_w_eb.png]

![Stack **com** TaskNotification][task_w_tn.png]

![Heap **com** TaskNotification][tasks_w_tn]

### Relógio
O RTC é ajustado automaticamente através de informações do GPS. Alguns dos frames do Protocolo NMEA possuem a informação de horas no formato UTC.
Após receber essa informação o timestamp que o RTC incrementa localmente é setado para esse valor. Antes de mostrar para o usuário é chamado uma função que faz a conversão para o hora local. 

### Buffer circular
A recepção e envio ,de bytes nas portas seriais, são feitas utilizando interrupção e buffer circulare *ringbuffer*.
Como transceptor Wifi\BT\BLE está sendo utilizando o ESP32 que também possui um RTOS(FreeRTOS). Futuro será colocar um módulo GPRS para redes móveis e processar a pilha de conexão também no ESP32.
Apesar do ESP32 ser mais que um transceiver o custo permite utiliza-lo apenas para essa finalidade

## Embarcado firmware ESP32:
Basicamente o firmware do ESP32 é aprnas um bdrige UART<>WiFI. Não possui nenhuma logica de negócio.

## SW Web
Backend: Django com utilização de Channel , abstração de websocket , para atualizações, em tempo real do frontend (Bootstrap+JS+CSS+Chart.js).
Message Broker: RabbitMQ com o protocolo AMQP e MQTT
Google maps API


![Tela login][desktop_login]

Recepção do payload da telemetria via channels(sockets)
 
![Desktop Debug][desktop_debug]

![Mobile Debug][mobile_debug1]{:height="50%" width="50%"}![Mobile Debug][mobile_debug2]{:height="50%" width="50%"}

Gráficos do acelerometro utilizado para registros da telemetria

![Desktop Acelerometros][desktop_accelerometer]

![Mobile Acelerometros][mobile_accelerometer]{:height="50%" width="50%"}

Leitura da posição do GPS

![Desktop posição][desktop_position]


## SW Desktop
No diretório GoodsTracker é possível encontrar uma versão nativa do SW em linguagem C#.

Quando o usuario indica 2 pontos no mapa(clicando) uma rota é devolvida conforme mostra a figura abaixo.

![Rotas traçadas][desktop_route]

O usuário pode contruir uma cerca eletrôncia onde onde será sinalizado caso a posição que o GPS indique uma localização fora.
Pode-se contruir inúmeras cercas compostas por inúmeros pontos 

![Cercas traçadas][desktop_fence]

Todo o histórico da telemetria é mantido e mostrado para o usuário em forma de treeview

![Histórico da telemetria][desktop_behavior]

É possivel habilitar e desabilitar os layers e também configurar o tipos de comunicação

## Ambiente de desenvolvimento

### Editores 

VS Code for linux, nano

### IDEs

MCUXPresso,Eclipse, Visual Studio

### Monitors network

sniffer - wireshark

trafic - Ntopng

## Configuração

### Ports
      
| Port | Aplicação       | socket      | url             |
|------|-----------------|-------------|-----------------|
|15672 |RabbitMQ adm     |             
|1883  |MQTT             |
|3000  |ntopng           |
|8000  |gtracker.com     |              | gtracker.com   |
|8020  |Daphne           | daphne.sock  |                |
|8010  |uwsgi            | gtracker.sock|                |
|5432  |gtracker-data    |              |                |
|      |ldap adm         |              | /phpldapadmin/ |

[tasks_diagram]:architecture/tasks_diagram.png
[desktop_route]:images/sw/route.png
[desktop_fence]:images/sw/fence.png
[desktop_behavior]:images/sw/behavior.png
[desktop_login]:images/web/desktop_login.png
[desktop_debug]:images/web/desktop_debug.png
[mobile_debug1]:images/web/mobile_debug1.png
[mobile_debug2]:images/web/mobile_debug2.png
[desktop_accelerometer]:images/web/desktop_accelerometer.png
[desktop_position]:images/web/desktop_position.png
[task_w_eb.png]:images/fw/tasks_w_eventbit.png
[task_w_tn.png]:images/fw/tasks_w_tn.png
[tasks_w_tn]:images/fw/tasks_w_tn.png
[mobile_accelerometer]:images/web/mobile_accelerometer.png