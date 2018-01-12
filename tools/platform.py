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
from project import *
from cl_bash import *

@click.group()
@click.option('--debug/--no-debug', default=False)
@click.pass_context
def cli(ctx, debug):
    ctx.obj['DEBUG'] = debug


class ESP32:

    # nome do proojeto
    PROJECT_NAME = PROJECT.configuration["platform"]["esp32"]["name"]
    # diretorio do projeto
    HOMEDIR = PROJECT.PLATFORMDIR + '/' + PROJECT_NAME
    # porta seral
    SERIAL_PORT = PROJECT.configuration["platform"]["esp32"]["serial_port"]
    # local do toolchain para o ESP
    IDF_PATH = os.getenv("IDF_PATH","/home/%s/esp-idf" % getpass.getuser())


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
    target = "%s/%s"  % (target_path,PROJECT.configuration["platform"]["esp32"]["gdb"]["target"])
    # interface selecionada
    interface = "%s/%s" % (interface_path, PROJECT.configuration["platform"]["esp32"]["gdb"]["interface"])
    # programa para debug
    program = "%s/build/%s" % (HOMEDIR, PROJECT.configuration["platform"]["esp32"]["gdb"]["program"])
    # ip do hohst gdb
    host_gdb = "%s/%s" % (HOMEDIR, PROJECT.configuration["platform"]["esp32"]["gdb"]["host"])
    # extensão do arquivo de programa
    ext_program = 'elf'
    # extensão dos arquivos de configuração
    ext_config  = 'cfg'
    # frequencia de debug
    freq_jtag = PROJECT.configuration["platform"]["esp32"]["gdb"]["freq_jtag"]

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
        bash.execute_remote(bash_cmd,getpass.getuser(),GDB.host_gdb)


    @staticmethod
    def stop_debug_server():

        bash_cmd = "'sudo -Sv && bash -s' -- < $openocdfile stop"
        bash.execute_remote(bash_cmd,getpass.getuser(),GDB.host_gdb)
      

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
            bash.execute_remote(bash_cmd,getpass.getuser(),GDB.host_gdb)


    @staticmethod
    def shutdown_interface():

        bash_cmd = "'sudo -Sv && bash -s' -- < $openocdfile shutdown"
        bash.execute_remote(bash_cmd,getpass.getuser(),GDB.host_gdb)


    @staticmethod
    def reset_target():

        bash_cmd = "'sudo -Sv && bash -s' -- < $openocdfile reset"
        bash.execute_remote(bash_cmd,getpass.getuser(),GDB.host_gdb)
       

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
    '''Inicia o modo menu de configuração da plataform'''

    Menu.loop()


if __name__ == '__main__':
    cli(obj={})
