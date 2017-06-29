/*
 * DataFrame.h
 *
 *  Created on: 27/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_DATAFRAME_H_
#define SOURCES_DATAFRAME_H_

#include "Array.h"

#define LEN_ADDRESS		5
#define LEN_ORIGEM		5
#define LEN_COUNT		5
#define LEN_OPERATION	2
#define LEN_RESOURCE	3
#define LEN_SIZE_PL		3
#define LEN_CHECKSUM	2

#define SIZE_HEADER			(LEN_ADDRESS+LEN_ORIGEM+LEN_OPERATION+LEN_RESOURCE+LEN_SIZE_PL+4)	// 4 separadores do cabecalho
#define SIZE_MIN_FRAME		(SIZE_HEADER+2)														// 2 separador do payload vazio

/*
 * Estrutura de dados do frame
 *
 */
typedef struct{

	int		address;
	int		dest;
	int		countFrame;
	char	operacao[LEN_OPERATION + 1];
	char	resource[LEN_RESOURCE + 1];

	ArrayPayLoad	PayLoad;

} DataCom;


#define clearData(f) memset((void*)f,0,sizeof(DataCom));

#endif /* SOURCES_DATAFRAME_H_ */
