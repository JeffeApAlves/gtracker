#! /bin/bash
#
# Gerenciador de projeto sumo
#
# ./osmCreate [nome do projeto] [opções]
#
# [nome do projeto]
#  nome do projeto
#
# [opções]
#
# -o arquivo osm
# -t tipos de veiculos utilizado na simulação
# -d Diretorio de saida
# -s seed
#

# nome do projeto
PROJECT_NAME=osm

# Diretorio de saida
OUTPUT_DIR=$HOME

#diretorio de trabalho
WORK_DIR=$OUTPUT_DIR/$PROJECT_NAME

# Prefixo dos arquivos do projeto
PREFIX_FILE=$PROJECT_NAME

# Arquivo de configuração  usado na criação dos poligonos adicionais
POLY_CONFIG=${PREFIX_FILE}.polycfg
# Arquivo de configuração  usado na criação das vias
NET_CONFIG=${PREFIX_FILE}.netccfg
# Arquivo de configuração  usado na simulação
SUMO_CONFIG=${PREFIX_FILE}.sumocfg

# Arquivo do mapa (entrada)
OSM_FILE=${PREFIX_FILE}_bbox.osm.xml
INPUT_OSM_FILE=./$OSM_FILE
BBOX="-23.6469,-46.7429,-23.6371,-46.7260"
SOURCE_OSM=0

# Arquivos de saida
NET_FILE=${PREFIX_FILE}.net.xml
POLY_FILE=${PREFIX_FILE}.poly.xml
GUI_FILE=${PREFIX_FILE}.view.xml
LAUNCH_FILE=${PREFIX_FILE}.launch.xml
ADD_FILE=${PREFIX_FILE}.type.add.xml

declare -A  END_TIME
declare -A  PERIOD
declare -A  FRINGE_FACTOR
declare -a  TYPES

# Classes/tipos que serão criadas
TYPES=(bus passenger truck pedestrian motorcycle)

### Informações para geração da frota/tráfico ###

# Fim do periodo de criação de trips
END_TIME=( [passenger]=3600 [bus]=3600 [truck]=3600 [pedestrian]=3600 [bicycle]=3600 [motorcycle]=3600)
# Periodo do ciclo de criação das trips (ex: a cada x criar n trips)
PERIOD=( [passenger]=3 [bus]=10 [truck]=7 [pedestrian]=1 [bicycle]=100 [motorcycle]=4)
# Influencia na estatistica onde será iniciado a trips
FRINGE_FACTOR=( [passenger]=5 [bus]=5 [truck]=5 [pedestrian]=1 [bicycle]=2 [motorcycle]=3)
# Seed para referencia da simulação
SEED=42

CMD="update"

function create_trips(){

	local type=$1
	local pre_fix=${type:0:3}
	local route_file=$WORK_DIR/${PREFIX_FILE}.$type.rou.xml
	local trip_file=$WORK_DIR/${PREFIX_FILE}.$type.trips.xml
	local net_file=$WORK_DIR/$NET_FILE
	local fringe_factor=${FRINGE_FACTOR[$type]}
	local end_time=${END_TIME[$type]}
	local period=${PERIOD[$type]}

	if [[ -f "$net_file" ]]; then

		if [ "$type" = "pedestrian" ]; then

			RET=$(python $SUMO_HOME/tools/randomTrips.py --seed $SEED  --pedestrians --max-distance 3000 --fringe-factor $fringe_factor  --prefix $pre_fix -n $net_file -r $route_file -p $period -e $end_time -o $trip_file) 

		elif [ "$type" = "passenger" ]; then

			RET=$(python $SUMO_HOME/tools/randomTrips.py --seed $SEED --min-distance 1000 --fringe-factor $fringe_factor  --prefix $pre_fix --vehicle-class $type -n $net_file -r $route_file -p $period -e $end_time -o $trip_file) 
		else
			RET=$(python $SUMO_HOME/tools/randomTrips.py --seed $SEED --fringe-factor $fringe_factor  --prefix $pre_fix --vehicle-class $type -n $net_file -r $route_file -p $period -e $end_time -o $trip_file) 
		fi

		# verifica via retorno no console
		echo $RET | grep 'Success' &> /dev/null

		if [ $? == 0 ]; then

			if [[ -f "$route_file" ]]; then

		                echo "Rotas[$type]: $(filename $route_file) - done"

			else
				echo "ERROR: Falha na criação das rotas $route_file para o tipo $type"
				exit -1
			fi
		else
			echo "ERROR: Execução do randomTrips para o tipo $type !"
			echo $RET
			exit -1
		fi
	else
		echo "ERROR: Não encontrado o arquivo $NET_FILE para criação das rotas"
		exit -2
	fi
}

function add_file(){

        local type=${2:-""}
        local file=$(filename $1)

	echo '   <copy file="'$file'"' $type '/>' >> $WORK_DIR/$LAUNCH_FILE
}

function add_line(){

	echo $1 >> $WORK_DIR/$LAUNCH_FILE
}

function create_sumocfg(){

	# Cria arquivo xml com a configuração para ser usada na simulação

	local route_files=""

	# Cria lista dos tipos
        for i in ${TYPES[@]}; do

                local file=${PREFIX_FILE}.${i}.rou.xml

                if [[ -f "$WORK_DIR/$file" ]]; then

 			if [ -n "$route_files" ]; then
                              route_files=$file","$route_files
                        else
                              route_files=$file
                        fi
		else
 			echo "ERROR: Arquivo $file não encontrado durante a criação do cfg"
			exit -1
		fi
	done

cat >$WORK_DIR/$SUMO_CONFIG <<EOL
<?xml version="1.0" encoding="UTF-8"?>

<configuration xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd">

    <input>
        <net-file value="$NET_FILE"/>
        <route-files value="$route_files"/>
        <additional-files value="$POLY_FILE"/>
    </input>

    <processing>
        <ignore-route-errors value="true"/>
    </processing>

    <routing>
        <device.rerouting.adaptation-steps value="180"/>
    </routing>

    <report>
        <verbose value="true"/>
        <duration-log.statistics value="true"/>
        <no-step-log value="true"/>
    </report>

    <gui_only>
        <gui-settings-file value="$GUI_FILE"/>
    </gui_only>

</configuration>
EOL
}

function create_launch(){

	# Cria o arquivo xml que omnet utilizara para iniciar o sumo-gui

	> $WORK_DIR/$LAUNCH_FILE		&&
	add_line '<?xml version="1.0"?>'	&&
	add_line '<!-- debug config -->'	&&
	add_line '<launch>'			&&
	add_file $NET_FILE			&&

	for i in ${TYPES[@]}; do

		add_file ${PREFIX_FILE}.${i}.rou.xml
		if [ $? != 0 ]; then
			exit -1
		fi
	done

	add_file $POLY_FILE			&&
	add_file $GUI_FILE			&&
	add_file $SUMO_CONFIG  'type="config"'	&&
	add_line '</launch>'
}

function create_all_trips(){

	# Cria trips aleatorias para todas os tipos


	if [ ${#TYPES[@]} -gt 0 ]; then

		echo "Types: ${TYPES[*]}"

		for i in ${TYPES[@]}; do
        		create_trips  ${i}
                	if [ $? != 0 ]; then
                        	exit -1
                	fi
		done
	else
		echo "ERROR: Tipo de veículo não definido"
		exit -2
	fi
}

function create_net(){

	if [ $SOURCE_OSM -eq 1 ]; then

		RET=$(python $SUMO_HOME/tools/osmGet.py --bbox "$BBOX" --prefix ${PREFIX_FILE} --output-dir $WORK_DIR 2>&1)

               # verifica via retorno no console
                echo $RET | grep '200 OK' &> /dev/null

                if [ $? = 0 ]; then

			echo "Download da área ($BBOX) realizado com sucesso"
		else
                        echo "Warning: Não foi possível fazer o download da área"
		fi

	elif [ $SOURCE_OSM -eq 2 ]; then

		if [[ -f "$INPUT_OSM_FILE" ]]; then

			# Copia o mapa para o diretorio de trabalho
			cp $INPUT_OSM_FILE $WORK_DIR/$OSM_FILE

			if [ $? != 0 ]; then
				echo "ERROR: Não foi possível copiar o arquivo $INPUT_OSM_FILE"
				exit 1
			fi
		else
			echo "Warning: Não foi localizado o arquivo '$INPUT_OSM_FILE'"
		fi
	fi

	# Cria o arquivo net baseado nas configurações que estão no arquivo xml e+ o map osm.xml

	if [[ -f "$WORK_DIR/$OSM_FILE" ]]; then

		netconvert  --configuration-file $WORK_DIR/$NET_CONFIG
	else
		echo "ERROR: O mapa $WORK_DIR/$OSM_FILE não foi localizado"
		exit -2
	fi
}

function create_poly(){

	# Cria o arquivo de poligonos adicionais baseado nas configurações que estão no arquivo xml

        if [[ -f "$WORK_DIR/$POLY_CONFIG" ]]; then

		polyconvert --configuration-file $WORK_DIR/$POLY_CONFIG
	else

                echo "ERROR: Não localizado o arquivo de configuração $WORK_DIR/$POLY_CONFIG"
                exit -2
	fi
}

function create_netcfg(){

	# Cria o arquivo xml com a configuração que será usada na geração do arquivo net


	#opcao da sessao output para gerar arquivos node,edge,connection utilizados na construcao do net 
	#<plain-output-prefix value ="$PREFIX_FILE"/>
	

	#<node-files value="$WORK_DIR/$PREFIX_FILE.nod.xml"/>
	#<edge-files value="$WORK_DIR/$PREFIX_FILE.edg.xml"/>
	#<connection-files value="$WORK_DIR/$PREFIX_FILE.con.xml"/>

cat >$WORK_DIR/$NET_CONFIG <<EOL
<?xml version="1.0" encoding="UTF-8"?>

<configuration xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/netconvertConfiguration.xsd">

    <input>
        <osm-files value="$OSM_FILE"/>
        <type-files value="$SUMO_HOME/data/typemap/osmNetconvert.typ.xml,$SUMO_HOME/data/typemap/osmNetconvertUrbanBrSP.typ.xml,$SUMO_HOME/data/typemap/osmNetconvertPedestrians.typ.xml"/>
    </input>

    <output>
        <output-file value="$NET_FILE"/>
        <output.street-names value="true"/>
        <output.original-names value="true"/>

    </output>

    <tls_building>
		<tls.discard-loaded value="false"/>
        <tls.discard-simple value="false"/>
        <tls.join value="true"/>
		<tls.join-dist value="40"/>
        <tls.guess-signals value="false"/>
    </tls_building>

    <ramp_guessing>
        <ramps.guess value="true"/>
    </ramp_guessing>

    <processing>
        <geometry.remove value="true"/>
        <roundabouts.guess value="false"/>
        <junctions.join value="true"/>
		<junctions.join-dist value="5"/>
        <junctions.corner-detail value="5"/>
        <crossings.guess value="true"/>
		<sidewalks.guess value="false"/>
     </processing>

     <building>
        <default.junctions.radius value="1" />
     </building>

    <report>
        <verbose value="true"/>
    </report>

</configuration>
EOL

}

function create_polycfg(){

	# Cria o arquivo xml de configuração dos poligonos

cat >$WORK_DIR/$POLY_CONFIG <<EOL
<?xml version="1.0" encoding="UTF-8"?>

<configuration xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/polyconvertConfiguration.xsd">

    <input>
        <net-file value="$NET_FILE"/>
        <osm-files value="$OSM_FILE"/>
        <osm.keep-full-type value="true"/>
        <type-file value="$SUMO_HOME/data/typemap/osmPolyconvert.typ.xml"/>
    </input>

    <output>
        <output-file value="$POLY_FILE"/>
    </output>

    <report>
        <verbose value="true"/>
    </report>

</configuration>
EOL

}

function create_guicfg(){

	#cria o arquivo xml com as configurações para o GUI 

cat >$WORK_DIR/$GUI_FILE <<EOL
<viewsettings>
    <scheme name="real world"/>
    <delay value="50"/>
    <viewport zoom="2000" x="2500" y="6180"/>
</viewsettings>
EOL
}


function create_addfile(){

#TODO

cat >$WORK_DIR/$ADD_FILE <<EOL

<additional>

    <inductionLoop id="myLoop1" lane="foo_0" pos="42" freq="900" file="out.xml"/>
    <inductionLoop id="myLoop2" lane="foo_2" pos="42" freq="900" file="out.xml"/>

 
    <busStop id="station1" lane="foo_0" startPos="5" endPos="20"/>

    <vType id="bus" maxSpeed="20" length="12"/>

</additional>
EOL

}
function update_all_paths(){

        # Atualiza nome de todos os arquivos conforme o nome do projeto
        PREFIX_FILE=$PROJECT_NAME

	# Atualiza o nome do arquivo de entrada do mapa
        OSM_FILE=${PREFIX_FILE}_bbox.osm.xml

        POLY_CONFIG=${PREFIX_FILE}.polycfg
        NET_CONFIG=${PREFIX_FILE}.netccfg
        SUMO_CONFIG=${PREFIX_FILE}.sumocfg
        NET_FILE=${PREFIX_FILE}.net.xml
        POLY_FILE=${PREFIX_FILE}.poly.xml
        GUI_FILE=${PREFIX_FILE}.view.xml
	LAUNCH_FILE=${PREFIX_FILE}.launch.xml
	ADD_FILE=${PREFIX_FILE}.type.add.xml

        # Atualiza diretorio de trabalho
	WORK_DIR=$OUTPUT_DIR/$PROJECT_NAME 
}

function create_project(){

        update_all_paths

	# cria diretorio do projeto e seus respectivos arquivos

	if [[ ! -d "$WORK_DIR" ]]; then
		mkdir "$WORK_DIR"

		if [ $? != 0 ]; then
			exit 1
		fi
	fi

        create_guicfg           &&
        create_polycfg          &&
        create_netcfg           &&
        create_net              &&
        create_poly             &&
        create_all_trips        &&
        create_sumocfg          &&
        create_launch
}

function set_types(){

	unset TYPES
	local i=0

	for var in $@ ; do

		TYPES[$i]=$var
		i=$i+1
	done
}

function update_simulation(){

        update_all_paths

	route_files=$(xmllint --xpath "string(//configuration/input/route-files/@value)" $WORK_DIR/$SUMO_CONFIG)

	IFS=',' read -r -a array <<< "$route_files"

        unset  TYPES

	for index in "${!array[@]}" ; do

    		TYPES[$index]=$(echo "${array[index]}" | awk -F "." '{print $2}')
	done

        #create_net              &&
        #create_poly             &&
		create_all_trips 
}

##### Main - Entrada do script

# Nome do projeto
PROJECT_NAME=$1

# Pula o primeiro argumento (nome do projeto)
shift

while getopts "b:o:t:d:s:" opt; do

  case $opt in

    o)  INPUT_OSM_FILE=$OPTARG
	SOURCE_OSM=2
	CMD=create
	;;

    t)	set_types $OPTARG
	;;

    d)  OUTPUT_DIR=$OPTARG
	;;

    s)  SEED=$OPTARG
	;;

    b)	BBOX=$OPTARG
	SOURCE_OSM=1
        CMD=create
	;;

    \?)
	echo "Opção invalida: -$OPTARG" >&2
      ;;

    :)
	echo "Opção -$OPTARG requer um argumento." >&2
	exit 1
      ;;

  esac

done

if [ $CMD = "create" ]; then

	create_project
else
	update_simulation
fi
