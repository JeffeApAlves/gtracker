#ifndef SOURCES_COMMUNICATION_FRAME_H_
#define SOURCES_COMMUNICATION_FRAME_H_

/**
 *
 * Frame Coomunication
 * [ End. de orig[5] : End dest[5] :  TIME STAMP[10] :Operacao[2] : Recurso[3] : SizePayload[3] : payload[ 0 ~ 255] : CheckSum[2] ] \r\n
 *
 * End. de orig:
 * Range: 00000~65535 (00000) Broadcast
 *
 * End. de dest:
 * Range: 00000~65535 (00000) Broadcast
 *
 * Operacao:
 * Possiveis:
 * RD = READ
 * WR = WRITE
 * AN + ANSWER
 *
 * Recurso:
 * Range: A-Z a-z 0~9
 *
 * SizePayload:
 * Range: 0~255
 *
 * Payload:
 * Informações para a camda aplicação
 * Observacao: '[' ']' sao caracteres especiais entao usar \] e \[
 *
 * CheckSum
 * Somatoria
 */

#include <stdint.h>

#include "Cmd.h"

#define LEN_ADDRESS		5
#define LEN_ORIGEM		5
#define LEN_TIME_STAMP	10
#define LEN_OPERATION	2
#define LEN_RESOURCE	3
#define LEN_SIZE_PL		3
#define LEN_CHECKSUM	2
#define LEN_PAYLOAD		256

#define SIZE_HEADER		(LEN_ADDRESS + LEN_ORIGEM + LEN_TIME_STAMP + LEN_OPERATION + LEN_RESOURCE + LEN_SIZE_PL+6)	// 5 separadores do cabecalho
#define SIZE_MIN_FRAME	(SIZE_HEADER+2)																				// 2 separador do payload vazio

#define CHAR_START		'['
#define CHAR_END		']'
#define CHAR_SEPARATOR	':'
#define CHAR_CR			'\r'
#define CHAR_LF			'\n'
#define CHAR_STR_END	'\0'

typedef struct{

	int			address;
	int			dest;
	int32_t		time_stamp;
	char		operacao[LEN_OPERATION + 1];
	Resource	resource;
	int			lengthPayLoad;
} Header;

typedef struct{

	int		Length;
	char	Data[LEN_PAYLOAD];

} PayLoad;

/*
 * Estrutura do pacote de comunicação
 *
 */
typedef struct{

	Header		Header;
	PayLoad		PayLoad;

} CommunicationPackage;

#define clearPayLoad(f)	memset((void*)f,0,sizeof(PayLoad));
#define clearPackage(f) memset((void*)f,0,sizeof(CommunicationPackage));
#define clearHeader(f)	memset((void*)f,0,sizeof(Header));

void AppendPayLoad(PayLoad* payload,const char* data);


#endif /* SOURCES_DATAFRAME_H_ */
