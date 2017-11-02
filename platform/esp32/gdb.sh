#! /bin/bash
#
# Inicia o debug de um program 
#
function start_gdb()
{
    if [ -f $program_name ] ; then
        xtensa-esp32-elf-gdb -x $gdb_init $program_name
    else
        echo "Não foi possivel localizar o arquivo de debug:$program_name"
    fi
}