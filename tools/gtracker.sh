#! /bin/bash
#
#  Gerenciador do projeto
#
#  <comando>    config: menu de configuração
#               update: atualiza
#
#
#  Steps:
#
#   1  Configurar a variavel de ambiente no  PROJECT_HOME=<path da solução> em gtracker.sh
#   
#   2. Incluir no .bashrc o script principal <gtracker.sh>
#
#   1. Conectar ambos (raspibarry+computador) em uma mesma rede com acesso a internet
#
#   3. Criar o mesmo usuario em ambos(raspiberry+computador)
#    3.1 adduser -m nome_usuario
#
#   4. Providenciar, para o usuaro criado sudo e bypass de senha para sudo atraves
#    4.1 visudo
#    4.2 <usuario> ALL=NOPASSWD: ALL
#
#   5. Providenciar RSA do seu usario
#    5.1 ssh-keygen -t rsa
#    5.2 ssh-copy-id user@ip_machine
#

#
# Local do projeto
#
export GTRACKER_HOME=$1

TOOLDIR="$GTRACKER_HOME/tools"

source $TOOLDIR/def.sh

#
# comandos gtracker
#
alias gtracker='$TOOLDIR/gtracker.py'
alias gtracker-home='cd $GTRACKER_HOME'
alias gtracker-deploy='gtracker deploy'
alias gtracker-update='gtracker update'
alias gtracker-install='gtracker install'
alias gtracker-run='gtracker run'
alias gtracker-worker='gtracker runworker'

#
# Configuração do ambiente de desenvolvimento para esp32
#
alias esp32-create='gtracker esp32'

# comandos sumo
#
alias sumo-run='gtracker sumo --cfg $SUMO_SIMULATION'
alias sumo-create='gtracker sumo --name "$SUMO_NAME" --seed "$SEED" --bbox "$SUMO_BBOX" --types "$SUMO_TYPES" --out "$SUMO_OUTPUT"'

#
# comandos Daphne
#
#alias daphne-r='daphne -b 127.0.0.1 -p 8020 --ws-protocol "graphql-ws" --proxy-headers GoodsTracker.asgi:channel_layer'
alias daphne-r='daphne -b 127.0.0.1 -p 8020 GoodsTracker.asgi:channel_layer --access-log=/var/www/gtracker/logs/daphne.log'

#
# comandos postgresql
#
alias pg-adm='python $WORKON_HOME/gtracker/lib/$PYTHON_VERSION/site-packages/pgadmin4/pgAdmin4.py'

#
# comandos ntop
#
alias ntop='sudo ntopng'

#
