# GoodsTracker

Versão nativa do software WebAPP

## Comunição 

Foram implementadas 2 tipos de comunicação serial (RS 232) e via broker de mensagens (rabbitmq). Para a camada dominio a comunicação é
realizada por meio de gerenciador de dispositivos e uma interface, sendo assim, é totalmente transparente a forma de comunicação para 
o layer de negócio.  
