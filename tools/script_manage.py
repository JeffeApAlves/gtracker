#!/usr/bin/env python

"""

@file    gtracker.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1


Gerenciador do projeto

"""

import os
import sys
import ast
import getpass
import subprocess
import click

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
    PLATFORMDIR = HOMEDIR + '/platform

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

    # nome da simulação
    NAME = os.getenv('SUMO_NAME','osm')

    # diretorio de trabalho
    OUTPUTDIR = os.getenv('SUMO_OUTPUT', PROJECT.VANETDIR+ '/' + NAME)
    
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

    INPUT_OSM_FILE='./' + OSM_FILE

    # Coordenadas que define a região do mapa para simulação
    BBOX = os.getenv('SUMO_BBOX','-23.6469,-46.7429,-23.6371,-46.7260')

    ## Informações para geração da frota/tráfico da simulação

    # Classes/tipos que serão criadas
    TYPES = os.getenv('SUMO_TYPES','passenger').split(' ')

    # Fim do periodo de criação de trips
    ENDTIME = ast.literal_eval(os.getenv('SUMO_ENDTIME','{"passenger" : "3600"}'))
    
    # Periodo do ciclo de criação das trips (ex: a cada x criar n trips)
    PERIOD = ast.literal_eval(os.getenv('SUMO_PERIOD','{"passenger" : "3"}'))
    
    # Influencia na estatistica onde será iniciado a trips
    FRINGE_FACTOR  = ast.literal_eval(os.getenv('{SUMO_FRINGEFACTOR','{"passenger" : "5"}'))
    
    # Seed para referencia na geração randomica da simulação
    SEED = os.getenv('SUMO_SEEDSEED',42)

    ## Arquivos de saida
    NET_FILE = PREFIX_FILE+'.net.xml'
    POLY_FILE = PREFIX_FILE+'.poly.xml'
    GUI_FILE = PREFIX_FILE+'.view.xml'
    LAUNCH_FILE = PREFIX_FILE+'.launch.xml'
    ADD_FILE = PREFIX_FILE+'.type.add.xml'
    
class USER:
    NAME = getpass.getuser()


class ESP32:

    PROJECT_NAME = os.getenv('ESP32_PROJECT_NAME','esp32-mqtt')

    HOMEDIR = PLATFORMDIR + '/' + PROJECT_NAME


class GDB:
    
    HOMEDIR = ESP32.HOMEDIR

    openocdfile = PROJECT.TOOLDIR + "/openocd.sh"

    gdbinit = HOMEDIR + '/gdbinit'

    url_openocd ="https://github.com/espressif/openocd-esp32.git"

    interface_path ="/usr/local/share/openocd/scripts/interface"

    target_path = "/usr/local/share/openocd/scripts/target"

    target = target_path + '/' + os.getenv('GDB_TARGET','esp32.cfg')
 
    interface = interface_path + '/' + os.getenv('GDB_INTERFACE','raspberrypi-native.cfg')
    
    program = HOMEDIR + "/build/"+os.getenv('GDB_PROGRAM_ELF','program.elf')
    
    host_gdb = os.getenv('GDB_HOST','192.168.42.1') 
    
    ext_program = 'elf'
 
    ext_config  = 'cfg'
 
    freq_jtag = os.getenv('ESP32_FREQ_JTAG',4000)

    def __init__(self):
        pass

    def select_program(self):

        bashCommand = ""
        process = subprocess.Popen(bashCommand.split(),stdout=subprocess.PIPE)
        stdout=process.communicate()

        pass

    def select_interface(self):
        pass

    def select_target(self):
        pass

    def start_gdb(self):
        pass

    def start_debug_server(self):
        pass

    def stop_debug_server(self):
        pass

    def start_debug_section(self):
        pass

    def config_jtag(self):
        pass

    def shutdown_interface(self):
        pass

    def reset_target(self):
        pass

    def scan_gdbserver(self):
        pass

sys.path.append(os.path.join(PROJECT.HOMEDIR, 'tools'))

@click.group()
@click.option('--debug/--no-debug', default=False)
@click.pass_context
def cli(ctx, debug):
    ctx.obj['DEBUG'] = debug

@cli.command()
@click.pass_context
def update(ctx):

    '''Atualiza os pacotes do ambiente Python'''

    # arquivo  temporario com a lista  dos pacotes python 
    
    packges='/tmp/env_packages.txt'

    os.system('pip freeze --local > {0} && pip install -U -r {0}'.format(packges))
    os.system('pip freeze --local > {0}'.format(PROJECT.REQUERIMENTS_FILE))    


@cli.command()
def install():

    '''Instala os pacotes do ambiente Python'''

    os.system('pip install -r {0}'.format(PROJECT.REQUERIMENTS_FILE))   


@cli.command()
def deploy():

    '''Copia os arquivos para o servidor'''

    os.system('rsync -avz {1} {0}@{2}'.format(USER.NAME,PROJECT.TOOLDIR,PROJECT.DEPLOYDIR))
    os.system('rsync -avz {1} {0}@{2}'.format(USER.NAME,PROJECT.WEBDIR,PROJECT.DEPLOYDIR))


@cli.command()
def run():

    ''''Executa a aplicação django'''

    os.system('python {0}/manage.py runserver  {1}:{2}'.format(PROJECT.WEBDIR,WEBSERVER.IP,WEBSERVER.PORT))  

@cli.command()
def runworker():

    '''Executa a aplicação django em modo produção'''

    os.system('python {0}/manage.py runworker'.format(WEBDIR))  


@click.command()
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
        
        os.system("sumo-gui -c %s " % cfg)

    elif bbox != None  :

        cmd = ''

        types_str = ''.join(str(e + ' ') for e in types)

        opts = []

        if name != None :
            opts.append(' %s' % name)

        if bbox != None :
            opts.append(' -b %s' % bbox)

        if seed != None :
            opts.append(' -s %s' % seed)

        if types != None :
            opts.append(' -t "%s"' % types_str)

        opts.append(' -d %s' % out)

        cmd = PROJECT.TOOLDIR + '/osmCreate.sh' + ''.join(opts)

        click.echo(cmd)
        os.system(cmd)

@cli.command()
@click.pass_context
def esp32(ctx):

    '''Gerencia p projeto do ESP32'''

    os.system(PROJECT.TOOLDIR + '/config.sh')

if __name__ == '__main__':
    cli(obj={})