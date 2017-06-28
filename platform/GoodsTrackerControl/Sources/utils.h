/*
 * utils.h
 *
 *  Created on: 07/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_UTILS_H_
#define SOURCES_UTILS_H_

#include "stdlib.h"

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
    const unsigned char *data;
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
void AsString(char* out,List *list,int index);
void AsInteger(int* out,List *list,int index);
void AsHex(int* out,List *list,int index);
void AsFloat(float* out,List *list,int index);
void AsChar(char* out,List *list,int index);

unsigned int calcChecksum(const char *buff, size_t sz);
void Convert(void* out,char* info,TYPE_INFO type);

#endif /* SOURCES_UTILS_H_ */
