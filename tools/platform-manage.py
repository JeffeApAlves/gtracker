#!/usr/bin/env python

"""

@file    platform.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1


Gerenciador do projeto

"""

import os
import os.path
import locale
import sys
import click
from project import *
from tui_menus import *
from esp32 import *
from gdb import *

@click.group()
@click.option('--debug/--no-debug', default=False)
@click.option('--menu/--no-menu',default=False,
                help="Inicia o modo menu de configuração da plataform")
@click.pass_context
def cli(ctx, debug,menu):
    ctx.obj['DEBUG'] = debug
    ctx.obj['MENU'] = menu

@cli.command()
def stop():

    '''Para o servidor de debug'''

    GDB.stop_debug_server()

@cli.command()
@click.option('--name',default=None)
def create(name):

    '''Cria um projeto'''

    ESP32.PROJECT_NAME = name
    ESP32.create_project()


@cli.command()
@click.pass_context
def start(ctx):

    '''Inicia o servidor de debug'''

    GDB.start_debug_server()

@cli.command()
@click.option('--elf',default=GDB.program)
@click.pass_context
def debug(ctx,elf):

    '''Inicia servidor de debug'''

    GDB.start_debug_section(program = elf)

@cli.command()
@click.pass_context
def config(ctx):

    '''Configurações referente a parte embarcada que serão salvas na configuração do projeto'''

    if ctx.obj['MENU']:
        Menu.background_title = "Gerenciador do %s" % PROJECT.NAME
        Menu.loop()

if __name__ == '__main__':
    cli(obj={})
