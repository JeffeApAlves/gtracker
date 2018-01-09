#!/usr/bin/env python

"""

@file    gtracker.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1


Gerenciador do projeto gtracker

"""

import os
import sys
import argparse
import ast
import getpass
#import subprocess
import click

class PROJECT:

    # nome do projeto
    NAME = os.getenv('PROJECT_NAME','solution')
    
    # Estrutura de diretorios da solucao

    # diretrio raiz da solução
    HOMEDIR = os.environ.get('GTRACKER_HOME',os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
    
    # ferramnetas
    TOOLDIR=HOMEDIR + '/tools'
    
    # site
    WEBDIR = HOMEDIR + '/web' 

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
    
    click.echo(OUTPUTDIR)
    
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
    

sys.path.append(os.path.join(PROJECT.HOMEDIR, 'tools'))

USER=getpass.getuser()


@click.group()
@click.option('--debug/--no-debug', default=False)
@click.pass_context
def cli(ctx, debug):
    ctx.obj['DEBUG'] = debug

@click.command(help='Atualiza os pacotes do ambiente Python')
@click.pass_context
def update(ctx):

    # arquivo  temporario com a lista  dos pacotes python 
    
    packges='/tmp/env_packages.txt'

    os.system('pip freeze --local > {0} && pip install -U -r {0}'.format(packges))
    os.system('pip freeze --local > {0}'.format(PROJECT.REQUERIMENTS_FILE))    


@click.command(help='Instala os pacotes do ambiente Python')
def install():

    os.system('pip install -r {0}'.format(PROJECT.REQUERIMENTS_FILE))   


@click.command(help='Copia os arquivos para o servidor')
def deploy():

    os.system('rsync -avz {1} {0}@{2}'.format(USER,PROJECT.TOOLDIR,DEPLOY_PATH))
    os.system('rsync -avz {1} {0}@{2}'.format(USER,WEBDIR,DEPLOY_PATH))


@click.command(help='Executa a aplicação django')
def run():
    os.system('python {0}/manage.py runserver  {1}:{2}'.format(WEBDIR,IP_WEBSERVER,PORT_WEBSERVER))  

@click.command(help='Executa a aplicação django em modo produção')
def runworker():

    os.system('python {0}/manage.py runworker'.format(WEBDIR))  


@click.command(help='Gerencia as simulaçoes de transito')
@click.option('--cfg', default=None)
@click.option('--bbox', default=SUMO.BBOX)
@click.option('--seed', default=SUMO.SEED)
@click.option('--name', default=SUMO.NAME)
@click.option('--types', default=SUMO.TYPES)
@click.option('--out', default=PROJECT.VANETDIR)
@click.pass_context
def sumo(ctx,cfg,bbox,seed,name,types,out):


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

@click.command(help='Gerencia p projeto do ESP32')
@click.pass_context
def esp32(ctx):
    os.system(PROJECT.TOOLDIR + '/config.sh')

cli.add_command(install)
cli.add_command(update)
cli.add_command(deploy)
cli.add_command(run)
cli.add_command(runworker)
cli.add_command(sumo)
cli.add_command(esp32)

if __name__ == '__main__':
    cli(obj={})