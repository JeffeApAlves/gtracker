/*
 * utils.h
 *
 *  Created on: 07/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_UTILS_H_
#define SOURCES_UTILS_H_

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "Cmd.h"

typedef enum {

	INTEGER,
	STRING,
	FLOAT,
	HEX,


}TYPE_INFO;

//#ifdef PEX_RTOS_START
#define _malloc(size)	pvPortMalloc(size)
#define _free(ptr)		vPortFree(ptr);ptr=NULL
//#else
//#define _malloc(size)	malloc(size)
//#define _free(ptr)		free(ptr)
//#endif

typedef struct {
    const char*	text;
    size_t len;
} buffer_t;

typedef	char** Itens;

typedef struct{

	Itens	itens;
	int		count;

} List;

buffer_t memtok(const void *s, size_t length, const char *delim, buffer_t *save_ptr);
void str_split(List* result,char* a_str, const char a_delim);
void str_append(char subject[], char insert[], int pos);
void removeList(List* list);

bool AsInteger(int* out,char *str,uint16_t index,const char a_delim);
bool AsString(char* out,char *str,uint16_t index,const char a_delim);
bool AsHex(uint16_t* out,char *str,uint16_t index,const char a_delim);
bool AsChar(char* out,char *str,uint16_t index,const char a_delim);
bool AsFloat(float* out,char *str,uint16_t index,const char a_delim);
bool AsResource(Resource* out,char *str,uint16_t index,const char a_delim);

#endif /* SOURCES_UTILS_H_ */
