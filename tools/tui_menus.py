"""

@file    tui_menus.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1


Menus gráficos(TUI) para o gerenciamento do projeto do esp32

"""

import os
import locale
import sys
import subprocess
from distutils import *
from dialog import Dialog
from gdb import *
from esp32 import *

locale.setlocale(locale.LC_ALL, '')


class Menu(object):

    background_title = ""

    active_scrren = None
    before_scrren = None

    @staticmethod
    def show(screen):
        Menu.before_screen = Menu.active_scrren
        Menu.active_scrren = screen

    @staticmethod
    def loop():

        Menu.show(Main_Screen.main_screen)

        while True :

            code = Menu.active_scrren()

            # ESC na tela principal entao sai do script
            # processa eventos com hanldes comunus
            if code in (Dialog.CANCEL,Dialog.ESC):

                if Menu.active_scrren == Main_Screen.main_screen:

                    subprocess.call('clear')
                    sys.exit(0)
                    break
                else:
                    Menu.show(Menu.before_screen)


class Project_Screen(Menu):

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

class Enviroment_Screen(Menu):

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

class ESP32_Screen(Menu):

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

class Main_Screen(Menu):

    @staticmethod
    def main_screen():

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        choices = [ ("1","Debug"),
                    ("2","Projeto"),
                    ("3","ESP32"),
                    ("4","Configuração"),
        ]

        options = { "1": GDB_Screen.debug_screen,
                    "2": Project_Screen.project_screen,
                    "3": ESP32_Screen.esp32_screen,
                    "4": ESP32.config,
        }

        code, tag = d.menu("Selecione uma das opções abaixo\n\nInterface:%s\nTarget:%s" % (GDB.interface,GDB.target),
                height=30,width=100,
                choices=choices)

        if code == d.OK:
            Menu.show(options[tag])

        return code


class GDB_Screen(Menu):


    @staticmethod
    def config_jtag():

        d = Dialog(dialog="dialog")
        #d.set_background_title(Menu.background_title)

        code, value = d.inputbox(text = "Informe a frequencia do jtag em [KHz]")
     
        if code == d.OK :
            GDB.config_jtag(value)
 

    @staticmethod
    def start_debug_section():

        d = Dialog(dialog="dialog")
        #d.set_background_title(Menu.background_title)

        file = "%s/*%s" %(GDB.program_path,GDB.ext_program)

        code,program = d.fselect(file,title="Seleção do programa",
                                height=30,width=100)

        if code == d.OK :
            GDB.start_debug_section(program)

        return code


    @staticmethod
    def select_interface() :

        d = Dialog(dialog="dialog")
        #d.set_background_title(Menu.background_title)

        file = "%s/*%s" %(GDB.interface_path,GDB.ext_config)

        code,path = d.fselect(file,title="Seleção da interface",
                                height=30,width=100)

        if code == d.OK:
            GDB.interface = path 

        return code 

    @staticmethod
    def select_target():

        d = Dialog(dialog="dialog")
        #d.set_background_title(Menu.background_title)
        
        file = "%s/*%s" %(GDB.target_path,GDB.ext_config)

        code,path = d.fselect(file,title="Seleção do target",
                                height=30,width=100)

        if code == d.OK :
            GDB.target = path 
        
        return code     


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

        options = { "1": GDB_Screen.start_debug_section,
                    "2": GDB.start_debug_server,
                    "3": GDB.stop_debug_server,
                    "4": GDB_Screen.select_interface,
                    "5": GDB_Screen.select_target,
                    "6": GDB_Screen.config_jtag,
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
