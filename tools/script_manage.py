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
import locale
import tarfile
import sys
import ast
import getpass
import subprocess
import click
import shlex
import nmap
import wget
import distutils
import xml.etree.ElementTree as ET
from distutils import *
from dialog import Dialog

SUMO_HOME = os.environ.get('SUMO_HOME',
                           os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
sys.path.append(os.path.join(SUMO_HOME, 'tools'))

locale.setlocale(locale.LC_ALL, '')


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
    # diretorio das ferramentas do SUMO
    TOOLDIR = HOME + '/tools'
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
        SUMO.download_map()
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

        bashh_cmd = 'python %s/osmGet.py --bbox "%s" --prefix "%s" --output-dir "%s" 2>&1' % (SUMO.TOOLDIR,SUMO.BBOX,SUMO.PREFIX_FILE,SUMO.OUTPUTDIR)
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

                bash_cmd = '%s/randomTrips.py --seed %d  --pedestrians --max-distance 3000 --fringe-factor %d  --prefix %s -n %s -r %s -p %d -e %d -o %s' % (SUMO.TOOLDIR,SUMO.SEED,fringe_factor,pre_fix,net_file,route_file,period,end_time,trip_file)

            else :

                bash_cmd = '%s/randomTrips.py --seed %d  --vehicle-class %s --min-distance 1000 --fringe-factor %d  --prefix %s -n %s -r %s -p %d -e %d -o %s'  % (SUMO.TOOLDIR,SUMO.SEED,type_class,fringe_factor,pre_fix,net_file,route_file,period,end_time,trip_file)
            
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


# Dummy context manager to make sure the debug file is closed on exit, be it
# normal or abnormal, and to avoid having two code paths, one for normal mode
# and one for debug mode.
class DummyContextManager:
    def __enter__(self):
        return self

    def __exit__(self, *exc):
        return False


class ESP32:

    # nome do proojeto
    PROJECT_NAME = os.getenv('ESP32_PROJECT_NAME','esp32-mqtt')
    # diretorio do projeto
    HOMEDIR = PROJECT.PLATFORMDIR + '/' + PROJECT_NAME
    # porta seral
    SERIAL_PORT = os.getenv('ESP32_SERIAL_PORT','/dev/ttyUSB0')
    # local do toolchain para o ESP
    IDF_PATH = os.getenv("IDF_PATH","/home/%s/esp-idf" % USER.NAME)


    @staticmethod
    def compile_all():

        os.chdir(ESP32.HOMEDIR)
        subprocess.call(shlex.split("make clean"),stdout = subprocess.DEVNULL , stderr=subprocess.STDOUT)
        bash.execute("make all","Compilando all")

    @staticmethod
    def compile_app():
        
        os.chdir(ESP32.HOMEDIR)
        bash.execute("make app","Compilando app")

    @staticmethod
    def create_project():

        '''Cria um projeto para ESP32 baseado no codigo demo blink''' 

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        code, value = d.inputbox(text = "Informe a frequencia do jtag em [KHz]")
     
        if code == d.OK:

            PROJECT.NAME = value

            code,path = d.dselect("./",title="Seleção do local do projeto",height=30,width=100)

            if code == d.OK:

                PROJECT.HOMEDIR = path

                copy_tree(ESP32.PROJECT_TEMPLATE,PROJECT.HOMEDIR)
                
                # renomeia o direotiro blink para o nome do projeto
                move_tree(PROJECT.HOMEDIR+'/blinck',PROJECT.HOMEDIR+PROJECT.NAME)

                subprocess.call("sed -e '/PROJECT_NAME/D' '$filepath/$new_project_name/Makefile' > '/tmp/Makefile'")
                subprocess.call("echo  'PROJECT_NAME := '$new_project_name >> '/tmp/Makefile'")
  
                copy_file("/tmp/Makefile","%s/%s" % (PROJECT.HOMEDIR,PROJECT.NAME))


    def serial_permission():

        try:
            subprocess.call(shlex.split("sudo chmod 777 %s" % ESP32.SERIAL_PORT),stdout = subprocess.DEVNULL , stderr=subprocess.STDOUT)
        except:
            click.echo("Serial não disponivel")


    def flash():

        ESP32.serial_permission()
        os.chdir(ESP32.HOMEDIR)
        bash.execute("make flash","Compilando flash")


    def monitor():

        ESP32.serial_permission()
        os.chdir(ESP32.HOMEDIR)
        bash.execute("make monitor","Monitor uart")


    def config():
        
        os.chdir(ESP32.HOMEDIR)
        subprocess.call(shlex.split("make menuconfig"))


    def manage_sdk():

        '''Atualiza ou clona o repositorio IDF'''

        '''
        if [ -n $IDF_PATH ]; then

            idf_path="$IDF_PATH"
        fi

        select_path "Repositório IDF" "$idf_path"
        local RET=$?

        if [ $RET -eq 0 ]; then

            idf_path=$filepath

            if [[ -d "$idf_path" || $(mkdir $idf_path) -eq 0 ]]; then

                {
                    update_repositorio "$idf_path"

                } || {

                    clone_repositorio "$idf_path_orin" "$idf_path"

                } || {

                    show_msgbox "ERRO!" "Não foi possivel clonar/atualizar o SDK"
                }
            fi

            update_paths
        fi
        '''

    def manage_toolchain():

        choices = [ ("Versão 64 bits","",True)
                    ("Versão 32 bits","",False)
        ]

        code, tag = d.radiolist("Selecione o IP do Host GDB",choices=choices)

        if code ==d.OK:

            code,path = d.dselect("./",title="Seleção do local do toolchain",height=30,width=100)

            if code == d.OK:
                
                ESP32.IDF_PATH = path
                 
                file_name = ""

                if tag == "Versão 64 bits":
                    file_name = wget.download(ESP32.url_espressif_toolchain64,bar=bar_thermometer)
                else:
                    file_name = wget.download(ESP32.url_espressif_toolchain32,bar=bar_thermometer)


                tar = tarfile.open(file_name)
                tar.extractall(ESP32.IDF_PATH)
                tar.close()

    def manage_openocd():
        pass

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

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        file = "%s/*%s" %(GDB.program_path,GDB.ext_program)

        code,path = d.fselect(file,title="Seleção do programa",
                                height=30,width=100)

        if code == d.OK :
            GDB.program = path

        return code 
      

    @staticmethod
    def select_interface() :

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        file = "%s/*%s" %(GDB.interface_path,GDB.ext_config)

        code,path = d.fselect(file,title="Seleção da interface",
                                height=30,width=100)

        if code == d.OK:
            GDB.interface = path 

        return code 

    @staticmethod
    def select_target():

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        file = "%s/*%s" %(GDB.target_path,GDB.ext_config)

        code,path = d.fselect(file,title="Seleção do target",
                                height=30,width=100)

        if code == d.OK :
            GDB.target = path 
        
        return code     

    @staticmethod
    def start_gdb(file_init = gdbinit, program = program):

        click.echo(file_init)
        click.echo(program)
        sys.exit(0)

        if os.path.isfile(file_init):

            if os.path.isfile(program) is True:

                bash_cmd = 'xtensa-esp32-elf-gdb -x %s %s' % (file_init,program)

                args = shlex.split(bash_cmd)

                process = subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE)

                stdout, erro = process.communicate()

            else:
                d = Dialog(dialog="dialog")
                d.set_background_title(Menu.background_title)
                d.msgbox('Não foi possivel abrir o arquivo %s' % program,title="ERRO")
        else:
            d = Dialog(dialog="dialog")
            d.set_background_title(Menu.background_title)
            d.msgbox('Não foi possivel abrir o arquivo %s' % file_init,title="ERRO")


    @staticmethod
    def start_debug_server():

        bash_cmd = "'sudo -Sv && bash -s' -- < $openocdfile start -i %s -t %s" % (GDB.interface,GDB.target)
        bash.execute_remote(bash_cmd,USER.NAME,GDB.host_gdb)


    @staticmethod
    def stop_debug_server():

        bash_cmd = "'sudo -Sv && bash -s' -- < $openocdfile stop"
        bash.execute_remote(bash_cmd,USER.NAME,GDB.host_gdb)
      

    @staticmethod
    def start_debug_section():

        code = GDB.select_program()

        if code == Dialog.OK :

            GDB.start_gdb()
    
        
    @staticmethod
    def config_jtag():

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        code, value = d.inputbox(text = "Informe a frequencia do jtag em [KHz]")
     
        if code == d.OK :
            GDB.freq_jtag = value
            bash_cmd = "'sudo -Sv && bash -s' -- < $openocdfile start -f %s" % (GDB.freq_jtag)
            bash.execute_remote(bash_cmd,USER.NAME,GDB.host_gdb)


    @staticmethod
    def shutdown_interface():

        bash_cmd = "'sudo -Sv && bash -s' -- < $openocdfile shutdown"
        bash.execute_remote(bash_cmd,USER.NAME,GDB.host_gdb)


    @staticmethod
    def reset_target():

        bash_cmd = "'sudo -Sv && bash -s' -- < $openocdfile reset"
        bash.execute_remote(bash_cmd,USER.NAME,GDB.host_gdb)
       

    @staticmethod
    def scan_gdbserver():

        #stdout, erro = subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()
        #bash_cmd = "nmap -n -sn 192.168.42.0/24 -oG - | awk '/Up$/{print $2}'"
        #args = shlex.split(bash_cmd)

        nm = nmap.PortScanner() 
        nm.scan('192.168.42.0/24', '22-443')      # scan host 127.0.0.1, ports from 22 to 443

        choices = []

        for l in nm.all_hosts():
            choices.append((l,"",False))

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)
        code, tag = d.radiolist("Selecione o IP do Host GDB",choices=choices)

        if code == d.OK:
            GDB.host_gdb = tag


class bash:

    @staticmethod
    def execute_remote(command_line,user,machine):

        bash_cmd = "ssh %s@%s %s" % (user,machine,command_line)
        execute(bash_cmd)

        
    @staticmethod
    def execute(command_line,text=""):

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        try:
            devnull = subprocess.DEVNULL
        except AttributeError: # Python < 3.3
            devnull_context = devnull = open(os.devnull, "wb")
        else:
            devnull_context = DummyContextManager()

        args = shlex.split(command_line)

        with devnull_context:
            p = subprocess.Popen(args, stdout=subprocess.PIPE,
                                    stderr=devnull, close_fds=True)
            # One could use title=... instead of text=... to put the text
            # in the title bar.
            d.programbox(fd=p.stdout.fileno(),text=text,height=30,width=150)
            retcode = p.wait()

        # Context manager support for subprocess.Popen objects requires
        # Python 3.2 or later.
        p.stdout.close()

class Menu:

    background_title = "Gerenciador do %s" % PROJECT.NAME

    active_scrren = None
    before_scrren = None

    @staticmethod
    def debug_screen():

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        choices = [ ("1","Iniciar debug (prompt)"),
                    ("2","Iniciar server(openocd)"),
                    ("3","Parar server(openocd)"),
                    ("4","Interface (adaptador)"),
                    ("5","Target (device)"),
                    ("6","JTAG"),
                    ("7","Reset target"),
                    ("8","Scan server"),
                    ("9","Desligar interface")
        ]

        options = { "1": GDB.start_debug_section,
                    "2": GDB.start_debug_server,
                    "3": GDB.stop_debug_server,
                    "4": GDB.select_interface,
                    "5": GDB.select_target,
                    "6": GDB.config_jtag,
                    "7": GDB.reset_target,
                    "8": GDB.scan_gdbserver,
                    "9": GDB.shutdown_interface,
        }

        code, tag = d.menu("Selecione uma das opções abaixo\n\nInterface:%s\nTarget:%s" % (GDB.interface,GDB.target),
                height=30,width=100,
                choices=choices)

        if code == d.OK:
            options[tag]()


        return code

    @staticmethod
    def project_screen():

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        choices = [ ("1","Compilar all"),
                    ("2","Compilar app"),
                    ("3","Criar projeto"),
        ]

        options = { "1": ESP32.compile_all,
                    "2": ESP32.compile_app,
                    "3": ESP32.create_project,
        }

        code, tag = d.menu("Selecione uma das opções abaixo",
                height=30,width=100,
                choices=choices)

        if code == d.OK:
            options[tag]()
            pass

        return code

    @staticmethod
    def enviroment_screen():

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        choices = [ ("1","Instalar/atualizar ESP-IDF"),
                    ("2","Instalar/atualizar openocd"),
                    ("3","Instalar/atualizar toolchain"),
        ]

        options = { "1": ESP32.manage_sdk,
                    "2": ESP32.manage_openocd,
                    "3": ESP32.manage_toolchain,
        }

        code, tag = d.menu("Ambiente de desenvolvimento",
                height=30,width=100,
                choices=choices)

        if code == d.OK:
            options[tag]()

        return code

    @staticmethod
    def esp32_screen():

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        choices = [ ("1","Gravar"),
                    ("2","Monitor"),
                    ("3","Configuração"),
        ]

        options = { "1": ESP32.flash,
                    "2": ESP32.monitor,
                    "3": ESP32.config,
        }

        code, tag = d.menu("Ambiente de desenvolvimento",
                height=30,width=100,
                choices=choices)

        if code == d.OK:
            options[tag]()

        return code

    @staticmethod
    def main_screen():

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        choices = [ ("1","Debug"),
                    ("2","Projeto"),
                    ("3","ESP32"),
                    ("4","Configuração"),
        ]

        options = { "1": Menu.debug_screen,
                    "2": Menu.project_screen,
                    "3": Menu.esp32_screen,
                    "4": Menu.esp32_screen,
        }

        code, tag = d.menu("Selecione uma das opções abaixo\n\nInterface:%s\nTarget:%s" % (GDB.interface,GDB.target),
                height=30,width=100,
                choices=choices)

        if code == d.OK:
            Menu.show(options[tag])

        return code

    @staticmethod
    def show(screen):
        Menu.before_screen = Menu.active_scrren
        Menu.active_scrren = screen

 
    def loop():

        Menu.show(Menu.main_screen)

        while True :

            code = Menu.active_scrren()

            # ESC na tela principal entao sai do script
            # processa eventos com hanldes comunus
            if code in (Dialog.CANCEL,Dialog.ESC):

                if Menu.active_scrren == Menu.main_screen:

                    subprocess.call('clear')
                    sys.exit(0)
                    break
                else:
                    Menu.show(Menu.before_screen)


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

    bash_cmd = 'pip freeze --local > %s' % (PROJECT.REQUERIMENTS_FILE)    
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

    bash_cmd = 'rsync -avz %s %s@%s' % (PROJECT.TOOLDIR,USER.NAME,PROJECT.DEPLOYDIR)
    args = shlex.split(bash_cmd)
    subprocess.call(args)
    
    bash_cmd = 'rsync -avz %s %s@%s' % (PROJECT.WEBDIR,USER.NAME,PROJECT.DEPLOYDIR)
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
@click.option('--interface', default = None)
def esp32(ctx,interface):

    '''Gerencia o projeto do ESP32'''

    if interface != None :
        GDB.select_interface()


@cli.command()
@click.pass_context
def menu(ctx):
    '''Inicia o modo menu de configuração'''

    Menu.loop()



if __name__ == '__main__':
    cli(obj={})
