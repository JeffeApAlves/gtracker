#!/usr/bin/env python

"""

@file    gtracker.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1


Gerenciador do projeto

"""

import os
import os.path
import sys
import ast
import getpass
import subprocess
import click
import shlex
import xml.etree.ElementTree as ET
import importlib.util


SUMO_HOME = os.environ.get('SUMO_HOME',
                           os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
sys.path.append(os.path.join(SUMO_HOME, 'tools'))

#import randomTrips


@click.group()
@click.option('--debug/--no-debug', default=False)
@click.pass_context
def cli(ctx, debug):

    ctx.obj['DEBUG'] = debug
    

class USER:
    NAME = getpass.getuser()

class PROJECT:

    # nome do projeto
    NAME = os.getenv('PROJECT_NAME','solution')

    # Estrutura de diretorios da solucao

    # diretrio raiz da solução
    HOMEDIR = os.environ.get('PROJECT_HOME',os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
    # ferramnetas
    TOOLDIR=HOMEDIR + '/tools'
    # site
    WEBDIR = HOMEDIR + '/web' 
    # embarcado
    PLATFORMDIR = HOMEDIR + '/platform'
    # Diretorio da simulação
    VANETDIR=HOMEDIR + '/vanet' 
    # local para deploy da solução
    DEPLOYDIR=os.getenv('DEPLOYDIR','/var/wwww/' + NAME)
    # Arquivos
    REQUERIMENTS_FILE= WEBDIR + "/requirements.txt"

class WEBSERVER:

    # IP do webserver da solução
    IP = os.getenv('IP_WEBSERVER','localhost')
    # porta do webserver da solução
    PORT = os.getenv('PORT_WEBSERVER','8000')


class SUMO:

    # local onde esta instalado o SUMO
    HOME=os.getenv('SUMO_HOME','/home/%s/sumo'%USER.NAME)

    # nome da simulação
    NAME = os.getenv('SUMO_NAME','osm')
    # diretorio de trabalho
    OUTPUTDIR = PROJECT.VANETDIR + '/' + NAME
    # Prefixo dos arquivos do projeto
    PREFIX_FILE=NAME
    # Arquivo de configuração  usado na criação dos poligonos adicionais
    POLY_CONFIG=PREFIX_FILE+'.polycfg'
    # Arquivo de configuração  usado na criação das vias
    NET_CONFIG= PREFIX_FILE + '.netccfg'
    # Arquivo de configuração  usado na simulação
    SUMO_CONFIG=PREFIX_FILE + '.sumocfg'
    # Arquivo do mapa (entrada)
    OSM_FILE = PREFIX_FILE + '_bbox.osm.xml'
    # arquivo do mapa de entrada
    INPUT_OSM_FILE='./' + OSM_FILE
    # Coordenadas que define a região do mapa para simulação
    BBOX = os.getenv('SUMO_BBOX','-23.6469,-46.7429,-23.6371,-46.7260')

    ## Informações para geração da frota/tráfico da simulação

    # Classes/tipos que serão criadas
    TYPES = os.getenv('SUMO_TYPES','enger').split(' ')
    # Fim do periodo de criação de trips
    ENDTIME = ast.literal_eval(os.getenv('SUMO_ENDTIME','{"enger" : "3600"}'))
    # Periodo do ciclo de criação das trips (ex: a cada x criar n trips)
    PERIOD = ast.literal_eval(os.getenv('SUMO_PERIOD','{"enger" : "3"}'))
    # Influencia na estatistica onde será iniciado a trips
    FRINGE_FACTOR  = ast.literal_eval(os.getenv('SUMO_FRINGEFACTOR','{"enger" : "5"}'))
    # Seed para referencia na geração randomica da simulação
    SEED = os.getenv('SUMO_SEEDSEED',42)

    ## Arquivos de saida
    NET_FILE = PREFIX_FILE+'.net.xml'
    POLY_FILE = PREFIX_FILE+'.poly.xml'
    GUI_FILE = PREFIX_FILE+'.view.xml'
    LAUNCH_FILE = PREFIX_FILE+'.launch.xml'
    ADD_FILE = PREFIX_FILE+'.type.add.xml'

    @staticmethod
    def create_simulation():

        SUMO.update_all_paths()
        
        if not os.path.exists(SUMO.OUTPUTDIR):
            os.mkdir(SUMO.OUTPUTDIR)

        SUMO.create_guicfg()
        SUMO.create_polycfg()
        SUMO.create_netcfg()  
        #SUMO.download_map()
        SUMO.create_net()      
        SUMO.create_poly()     
        SUMO.create_all_trips()
        SUMO.create_sumocfg()
        SUMO.create_launch()
        

    @staticmethod
    def update_simulation():

        update_all_paths()

         # Classes/tipos que serão criadas
        TYPES = os.getenv('SUMO_TYPES','enger').split(' ')

        create_all_trips()

        

    @staticmethod
    def update_all_paths():

        # Atualiza nome de todos os arquivos conforme o nome do projeto
        PREFIX_FILE=SUMO.NAME
 
        # Arquivo de configuração  usado na criação dos poligonos adicionais
        POLY_CONFIG=SUMO.PREFIX_FILE+'.polycfg'
        # Arquivo de configuração  usado na criação das vias
        NET_CONFIG= SUMO.PREFIX_FILE + '.netccfg'
        # Arquivo de configuração  usado na simulação
        SUMO_CONFIG=SUMO.PREFIX_FILE + '.sumocfg'
        # Arquivo do mapa (entrada)
        OSM_FILE = SUMO.PREFIX_FILE + '_bbox.osm.xml'
    
        # Arquivos de saida
        NET_FILE = PREFIX_FILE+'.net.xml'
        POLY_FILE = PREFIX_FILE+'.poly.xml'
        GUI_FILE = PREFIX_FILE+'.view.xml'
        LAUNCH_FILE = PREFIX_FILE+'.launch.xml'
        ADD_FILE = PREFIX_FILE+'.type.add.xml'
 
        # diretorio de trabalho
        SUMO.OUTPUTDIR = PROJECT.VANETDIR + '/' + SUMO.NAME

        

    @staticmethod
    def create_guicfg():

        filename = SUMO.OUTPUTDIR + '/' + SUMO.GUI_FILE

        viewsettings = ET.Element("viewsettings")

        ET.SubElement(viewsettings, "scheme", name="real world")
        ET.SubElement(viewsettings, "delay", value="50")
        ET.SubElement(viewsettings, "viewport",zoom="2000",x="2500",y="6200")

        tree = ET.ElementTree(viewsettings)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')
        


    @staticmethod
    def create_polycfg():

        filename = SUMO.OUTPUTDIR + '/' + SUMO.POLY_CONFIG

        # TODO O atributo original deveria ser o da linha comentado (xsi)
        # config = ET.Element("configuration",xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance",xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")
        config = ET.Element("configuration",xmlns="http://www.w3.org/2001/XMLSchema-instance",noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")

        input = ET.SubElement(config, "input")
        ET.SubElement(input, "net-file" , value = SUMO.NET_FILE)
        ET.SubElement(input, "osm-files" , value = SUMO.OSM_FILE)
        ET.SubElement(input, "osm.keep-full-type" , value = "true")
        ET.SubElement(input, "type-file" , value = "%s/data/typemap/osmPolyconvert.typ.xml" % SUMO.HOME)

        output = ET.SubElement(config, "output")
        ET.SubElement(output, "output-file" , value = SUMO.POLY_FILE)

        report = ET.SubElement(config, "report")
        ET.SubElement(report, "verbose" , value = "true")

        tree = ET.ElementTree(config)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')
        

    @staticmethod
    def create_netcfg():

        filename = SUMO.OUTPUTDIR + '/' + SUMO.NET_CONFIG

        dir_typemap = '%s/data/typemap' % SUMO.HOME

        # TODO O atributo original deveria ser o da linha comentado (xsi)
        # config = ET.Element("configuration",xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance",xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")
        config = ET.Element("configuration",xmlns="http://www.w3.org/2001/XMLSchema-instance",noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")

        input = ET.SubElement(config, "input")
        ET.SubElement(input, "osm-files" , value = SUMO.OSM_FILE)
        ET.SubElement(input, "type-files" , value = "{0}/osmNetconvert.typ.xml,{0}/osmNetconvertUrbanBrSP.typ.xml,{0}/osmNetconvertPedestrians.typ.xml".format(dir_typemap))

        output = ET.SubElement(config, "output")
        ET.SubElement(output, "output-file" , value = SUMO.NET_FILE)
        ET.SubElement(output, "output.street-names" , value = "true")
        ET.SubElement(output, "output.original-names" , value = "true")

        tls_building = ET.SubElement(config, "processing")
        ET.SubElement(tls_building, "tls.discard-loaded" ,  value = "false")
        ET.SubElement(tls_building, "tls.discard-simple" ,  value = "false")
        ET.SubElement(tls_building, "tls.join" ,            value = "true")
        ET.SubElement(tls_building, "tls.join-dist" ,       value = "40")
        ET.SubElement(tls_building, "tls.guess-signals" ,   value = "false")

        ramp_guessing = ET.SubElement(config, "ramp_guessing")
        ET.SubElement(ramp_guessing, "ramps.guess" ,  value = "true")

        processing = ET.SubElement(config, "processing")
        ET.SubElement(processing, "geometry.remove", value="true")
        ET.SubElement(processing, "roundabouts.guess", value="false")
        ET.SubElement(processing, "junctions.join", value="true")
        ET.SubElement(processing, "junctions.join-dist", value="5")
        ET.SubElement(processing, "junctions.corner-detail", value="5")
        ET.SubElement(processing, "crossings.guess", value="true")
        ET.SubElement(processing, "sidewalks.guess", value="false")

        building = ET.SubElement(config, "building")
        ET.SubElement(building, "default.junctions.radius" , value = "1")

        report = ET.SubElement(config, "report")
        ET.SubElement(report, "verbose" , value = "true")

        tree = ET.ElementTree(config)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')

    @staticmethod
    def create_poly():

        bash_cmd = 'polyconvert --configuration-file %s/%s' % (SUMO.OUTPUTDIR,SUMO.POLY_CONFIG)
        args = shlex.split(bash_cmd)
        subprocess.call(args)
        

    @staticmethod
    def download_map():

        bashh_cmd = 'python %s/tools/osmGet.py --bbox "%s" --prefix "%s" --output-dir "%s" 2>&1' % (SUMO.HOME,SUMO.BBOX,SUMO.PREFIX_FILE,SUMO.OUTPUTDIR)
        args = shlex.split(bash_cmd)
        subprocess.call(args)
        

    @staticmethod
    def create_net():

        bash_cmd = 'netconvert  --configuration-file %s/%s' % (SUMO.OUTPUTDIR,SUMO.NET_CONFIG)
        args = shlex.split(bash_cmd)
        subprocess.call(args)
        


    @staticmethod
    def create_all_trips():

        for type_class in SUMO.TYPES :
            SUMO.create_trips(type_class)
        

    @staticmethod
    def create_trips(type_class):

        pre_fix = type_class[:3]
        route_file = '%s/%s.%s.rou.xml' % (SUMO.OUTPUTDIR,SUMO.PREFIX_FILE,type_class)
        trip_file = '%s/%s.%s.trips.xml' % (SUMO.OUTPUTDIR,SUMO.PREFIX_FILE,type_class)
        net_file = '%s/%s' % (SUMO.OUTPUTDIR,SUMO.NET_FILE)
        fringe_factor = int(SUMO.FRINGE_FACTOR[type_class])
        end_time = int(SUMO.ENDTIME[type_class])
        period = int(SUMO.PERIOD[type_class])

        if os.path.isfile(net_file) :

            if type_class ==  "pedestrian" :

                bash_cmd = '%s/tools/randomTrips.py --seed %d  --pedestrians --max-distance 3000 --fringe-factor %d  --prefix %s -n %s -r %s -p %d -e %d -o %s' % (SUMO.HOME,SUMO.SEED,fringe_factor,pre_fix,net_file,route_file,period,end_time,trip_file)

            else :

                bash_cmd = '%s/tools/randomTrips.py --seed %d  --vehicle-class %s --min-distance 1000 --fringe-factor %d  --prefix %s -n %s -r %s -p %d -e %d -o %s'  % (SUMO.HOME,SUMO.SEED,type_class,fringe_factor,pre_fix,net_file,route_file,period,end_time,trip_file)
            
            args = shlex.split(bash_cmd)

            process = subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
            
            stdout, erro = process.communicate()

            if stdout :
                click.echo("Rotas[%s]: %s %s) - done" % (type_class,route_file,trip_file))

            if erro :
                click.echo("ERROR: Falha na criação das rotas para %s / arquivo  %s " % (type_class,route_file))

        else:
            click.echo("ERROR: Não encontrado o arquivo $NET_FILE para criação das rotas")

        
       
    @staticmethod
    def create_launch():

    	# Cria o arquivo xml que omnet utilizara para iniciar o sumo

        filename = SUMO.OUTPUTDIR + '/' + SUMO.LAUNCH_FILE

        launch = ET.Element("launch")

        ET.SubElement(launch, "copy", file=SUMO.NET_FILE)

        for t in SUMO.TYPES:
            ET.SubElement(launch, "copy", file="%s.%s.rou.xml" % (SUMO.PREFIX_FILE,t) )

        ET.SubElement(launch, "copy", file=SUMO.POLY_FILE)
        ET.SubElement(launch, "copy", file=SUMO.GUI_FILE)
        ET.SubElement(launch, "copy", file=SUMO.SUMO_CONFIG , type= "config")

        tree = ET.ElementTree(launch)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')

        


    @staticmethod
    def create_sumocfg():

        # Cria arquivo xml com a configuração para ser usada na simulação

        route_files = ','.join(str(SUMO.PREFIX_FILE + '.' + e + '.rou.xml') for e in SUMO.TYPES)

        filename = SUMO.OUTPUTDIR + '/' + SUMO.SUMO_CONFIG

        # TODO O atributo original deveria ser o da linha comentado (xsi)
        # config = ET.Element("configuration",xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance",xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")
        config = ET.Element("configuration",xmlns="http://www.w3.org/2001/XMLSchema-instance",noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")


        input = ET.SubElement(config, "input")
        ET.SubElement(input, "net-file" , value = SUMO.NET_FILE)
        ET.SubElement(input, "route-files" , value = route_files)
        ET.SubElement(input, "additional-files" , value = SUMO.POLY_FILE)

        processing = ET.SubElement(config, "processing")
        ET.SubElement(processing, "ignore-route-errors" , value = "true")

        routing = ET.SubElement(config, "processing")
        ET.SubElement(routing, "device.rerouting.adaptation-steps" , value = "100")

        report = ET.SubElement(config, "report")
        ET.SubElement(report, "verbose" , value = "true")
        ET.SubElement(report, "duration-log.statistics" , value = "true")
        ET.SubElement(report, "no-step-log" , value = "true")

        gui = ET.SubElement(config, "gui")
        ET.SubElement(gui, "gui-settings-file" , value = SUMO.GUI_FILE)

        tree = ET.ElementTree(config)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')

        


class ESP32:

    # nome do proojeto
    PROJECT_NAME = os.getenv('ESP32_PROJECT_NAME','esp32-mqtt')
    # diretorio do projeto
    HOMEDIR = PROJECT.PLATFORMDIR + '/' + PROJECT_NAME


class GDB:

    # diretŕorio de trabalho     
    HOMEDIR = ESP32.HOMEDIR
    # script para execução remota no adaptador
    openocdfile = PROJECT.TOOLDIR + "/openocd.sh"
    # arquivo de inicialização
    gdbinit = HOMEDIR + '/gdbinit'
    # repositório do openocd
    url_openocd ="https://github.com/espressif/openocd-esp32.git"
    # diretorório com os arquivos de cfg das interfaces
    interface_path ="/usr/local/share/openocd/scripts/interface"
    # diretorório com os arquivos de cfg dos tragets
    target_path = "/usr/local/share/openocd/scripts/target"
    # diretorório onde está o executavel para debug
    program_path = HOMEDIR + '/build'
    # target selecionando
    target = target_path + '/' + os.getenv('GDB_TARGET','esp32.cfg')
    # interface selecionada
    interface = interface_path + '/' + os.getenv('GDB_INTERFACE','raspberrypi-native.cfg')
    # programa para debug
    program = HOMEDIR + "/build/"+os.getenv('GDB_PROGRAM_ELF','program.elf')
    # ip do hohst gdb
    host_gdb = os.getenv('GDB_HOST','192.168.42.21') 
    # extensão do arquivo de programa
    ext_program = 'elf'
    # extensão dos arquivos de configuração
    ext_config  = 'cfg'
    # frequencia de debug
    freq_jtag = os.getenv('ESP32_FREQ_JTAG',4000)

    def __init__(self):
        pass

    @staticmethod
    def select_program():

        ret = TUI.select_file('Selecione o programa',program_path,ext_config,remote=False) 
        if ret != None :
            program = ret 

        

    @staticmethod
    def select_interface() :

        ret = TUI.select_file('Selecione a insterface',interface_path,ext_config,remote=True) 
        if ret != None :
            interface = ret 
        
        

    @staticmethod
    def select_target(self):

        ret = TUI.select_file('Selecione o target',target_path,ext_config,remote=True) 
        if ret != None :
            target = ret 
        
        

    @staticmethod
    def start_gdb(file_init,program):

        if os.path.isfile(file_init):

            if os.path.isfile(program) is True:

                bash_cmd = 'xtensa-esp32-elf-gdb -x %s %s' % (file_init,program)

                process = subprocess.Popen(bash_cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE)

                stdout, erro = process.communicate()

            else:
                TUI.show_msgbox('EERO','Não foi possivel abrir o arquivo %s'  % program)
        else:
            TUI.show_msgbox('EERO','Não foi possivel abrir o arquivo %s'  % file_init)


        

    @staticmethod
    def start_debug_server():

        '''
            ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "start"  -i "$interface" -t "$target" &> /tmp/ssh.log &

            dialog \
                --no-shadow \
                --title "$USER iniciando debug no host IP:$host_gdb" \
                --tailbox /tmp/ssh.log 40 100
        '''

        

    @staticmethod
    def stop_debug_server():

        '''
            ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "stop"  &> /tmp/ssh.log &

            dialog \
                --no-shadow \
                --title "$USER iniciando debug no host IP:$host_gdb" \
                --tailbox /tmp/ssh.log 40 100
        '''
        

    @staticmethod
    def start_debug_section():

        ret = GDB.select_program()

        if ret :

            start_gdb(gdbinit,program)
    
        

    @staticmethod
    def config_jtag():
        '''
                freq_jtag=$(dialog --stdout \
                            --title "Nome do projeto" \
                            --backtitle "$app_title" \
                            --inputbox "Informe a frequencia do jtag em [KHz]" 10 100 $freq_jtag \
                        )

                local RET=$?

                if [ $RET -ne 1 ]; then

                    ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "config" -i "$interface" -f "$freq_jtag" &> /dev/null

            #        dialog \
            #            --no-shadow \
            #            --title "$USER iniciando debug no host IP:$host_gdb" \
            #            --tailbox /tmp/ssh.log 40 100
                fi
        '''
        

    @staticmethod
    def shutdown_interface():
        '''
                ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "shutdown" &> /tmp/ssh.log  &

                dialog \
                    --no-shadow \
                    --title "$USER iniciando debug no host IP:$host_gdb" \
                    --tailbox /tmp/ssh.log 40 100
        '''
        

    @staticmethod
    def reset_target():
        '''
                ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "reset" &> /tmp/ssh.log  &

                dialog \
                    --no-shadow \
                    --title "$USER iniciando debug no host IP:$host_gdb" \
                    --tailbox /tmp/ssh.log 40 100
        '''
        

    @staticmethod
    def scan_gdbserver():
        '''
        host_gdb=$(sudo nmap -sn 192.168.0.0/24 | awk '/^Nmap/{ip=$NF}/B8:27:EB/{print ip}')

        '''

        TUI.show_description(host_gdb)
        

class TUI:

    @staticmethod

    def show_description(text):
        clic.echo(text)

        

    @staticmethod
    def show_msgbox(title,text):
        clic.echo(title + ':' + text)

        

    @staticmethod
    def show_info(text):

        #splash 3 segundos
        clic.echo(Informações + ':' + text)

        

    @staticmethod
    def select_file(title , path , ext , remote=False):

        bashCommand = ['bash','c','source %s/misc.sh' % PROJECT.TOOLDIR]
        process = subprocess.Popen(bashCommand,stdout=subprocess.PIPE,stderr=subprocess.PIPE)

        if remote :
            r = 'remote'
        else :
            r = ''

        bashCommand = 'select_file "Selecione a interface(adaptador)" %s %s %s' % (path,ext,r)
        process = subprocess.Popen(bashCommand,stdout=subprocess.PIPE)

        stdout = process.communicate()

        

    @staticmethod
    def select_path():
        pass


sys.path.append(os.path.join(PROJECT.HOMEDIR, 'tools'))

@cli.command()
@click.pass_context
def update(ctx):

    '''Atualiza os pacotes do ambiente Python'''

    # arquivo  temporario com a lista  dos pacotes python 
    
    packges='/tmp/env_packages.txt'

    bash_cmd = 'pip freeze --local > {0} && pip install -U -r {0}'.format(packges)
    
    args = shlex.split(bash_cmd)
    subprocess.call(args)

    bash_cmd = 'pip freeze --local > {0}'.format(PROJECT.REQUERIMENTS_FILE)    
    args = shlex.split(bash_cmd)
    subprocess.call(args)



@cli.command()
def install():

    '''Instala os pacotes do ambiente Python'''
   
    bash_cmd = 'pip install -r %s' % (PROJECT.REQUERIMENTS_FILE)

    args = shlex.split(bash_cmd)
    subprocess.call(args)


@cli.command()
def deploy():

    '''Copia os arquivos para o servidor'''

    bash_cmd = 'rsync -avz {1} {0}@{2}' % (USER.NAME,PROJECT.TOOLDIR,PROJECT.DEPLOYDIR)
    args = shlex.split(bash_cmd)
    subprocess.call(args)
    
    bash_cmd = 'rsync -avz {1} {0}@{2}' % (USER.NAME,PROJECT.WEBDIR,PROJECT.DEPLOYDIR)
    args = shlex.split(bash_cmd)
    subprocess.call(args)
    


@cli.command()
def run():

    ''''Executa a aplicação django'''

    bash_cmd = 'python %s/manage.py runserver  %s:%s' % (PROJECT.WEBDIR,WEBSERVER.IP,WEBSERVER.PORT)  
    args = shlex.split(bash_cmd)
    subprocess.call(args)


@cli.command()
def runworker():

    '''Executa a aplicação django em modo produção'''

    bash_cmd = 'python %s/manage.py runworker' % PROJECT.WEBDIR  
    args = shlex.split(bash_cmd)
    subprocess.call(args)
    


@cli.command()
@click.option('--cfg', default=None)
@click.option('--bbox', default=SUMO.BBOX)
@click.option('--seed', default=SUMO.SEED)
@click.option('--name', default=SUMO.NAME)
@click.option('--types', default=SUMO.TYPES)
@click.option('--out', default=PROJECT.VANETDIR)
@click.pass_context
def sumo(ctx,cfg,bbox,seed,name,types,out):

    '''Gerencia as simulaçoes de transito'''

    if cfg != None :
        
        bash_cmd = 'sumo-gui -c %s/%s' % (SUMO.OUTPUTDIR,cfg)
        args = shlex.split(bash_cmd)
        subprocess.call(args)

    elif bbox != None  :

        SUMO.create_simulation()
    

@cli.command()
@click.pass_context
@click.option('--interface', default=GDB.program)
def esp32(ctx,interface):

    '''Gerencia p projeto do ESP32'''

    if interface != None :
        GDB.select_interface()

    bash_cmd =  '%s/config.sh' % PROJECT.TOOLDIR
    args = shlex.split(bash_cmd)
    subprocess.call(args)
    

if __name__ == '__main__':
    cli(obj={})
