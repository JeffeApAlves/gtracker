#!/usr/bin/env python

"""

@file    vanet.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1


Gerenciador do projeto

"""

import os
import os.path
import locale
import sys
from project import *
from vanet import *

locale.setlocale(locale.LC_ALL, '')


@click.group()
@click.option('--debug/--no-debug', default=False)
@click.pass_context
def cli(ctx, debug):
    ctx.obj['DEBUG'] = debug

@cli.command()
def run():

    '''Executa a simulação com a configuração previamente feita'''
    
    SUMO.run()
  
@cli.command()
@click.option('--cfg', default=None)
@click.option('--bbox', default=SUMO.BBOX)
@click.option('--seed', default=SUMO.SEED)
@click.option('--name', default=SUMO.NAME)
@click.option('--types', default=SUMO.TYPES)
@click.option('--out', default=PROJECT.VANETDIR)
@click.pass_context
def create(cfg,bbox,seed,name,types,out):

    '''Gerencia as simulaçoes de transito'''
    
    config(cfg,bbox,seed,name,types,out)
    
    SUMO.create_simulation()

@cli.command()
@click.option('--cfg', default=None)
@click.option('--bbox', default=SUMO.BBOX)
@click.option('--seed', default=SUMO.SEED)
@click.option('--name', default=SUMO.NAME)
@click.option('--types', default=SUMO.TYPES)
@click.option('--out', default=PROJECT.VANETDIR)
def config(cfg,bbox,seed,name,types,out):
    
    '''Configurações referente a simulação de tráfego que serão salvas na configuração do projeto'''
    pass

if __name__ == '__main__':
    cli(obj={})


