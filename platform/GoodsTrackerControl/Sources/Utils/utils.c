/*
 * utils.c
 *
 *  Created on: 07/06/2017
 *      Author: Jefferson
 */

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "Frame.h"
#include "protocol.h"
#include "utils.h"

/**
 *
 * Cria um array de string
 *
 */
void str_split(List* result, char* str, const char a_delim) {

	char delim[2];
	delim[0]	= a_delim;
	delim[1]	= 0;
    char*	tmp	= str;
    int	count	= 0;

	while (*tmp){

		if (a_delim == *tmp){
			count++;
		}

		tmp++;
	}

	result->count	= count + 1;
	result->itens	= _malloc(sizeof(char*) * result->count);

	unsigned int len = strlen(str);

	buffer_t token, state;

	token = memtok(str, len, delim, &state);

    unsigned short idx = 0;

	while (token.text != NULL) {

		char* tmp = _malloc(sizeof(char) * (token.len + 1));

	   if(tmp!=NULL){

		   memcpy(tmp, token.text, token.len);

		   tmp[token.len] = '\0';

		   result->itens[idx++] = tmp;

		   token = memtok(NULL, 0, delim, &state);
	   }
	}
}
//------------------------------------------------------------------------

/*
 *
 *  Usa strpbrk() para multiplos delimitadores.
 *
 **/
buffer_t memtok(const void *s, size_t length, const char *delim, buffer_t *save_ptr){

    const char *stream,
                        *token;
    size_t len = 0;

    if (NULL == s) {
        stream = save_ptr->text;
    } else {
        stream = s;
        save_ptr->len = length;
    }

    token = stream;

    /* Advance until either a token is found or the stream exhausted. */
    while (save_ptr->len--) {
        if (memchr(delim, *stream, strlen(delim))) {
            /* Point save_ptr past the (non-existent) token. */
            save_ptr->text = stream + 1;
            return (buffer_t) { .text = token, .len = len };
        }

        ++len;
        ++stream;
    }

    /* State : done. */
    *save_ptr = (buffer_t) { .text = NULL, .len = 0 };

    /* Stream exhausted but no delimiters terminate it. */
    return (buffer_t){ .text = token, .len = len };
}
//-----------------------------------------------------------------------------------------

void str_append(char subject[], char insert[], int pos) {

	size_t size = (strlen(subject)-pos) + (strlen(insert) + 1);

	if(size>0){

		char* buf	= (char*) _malloc(sizeof(char)*size);

		if(buf!=NULL){

			memset((void*)buf,0,size);

			strcpy(buf, insert);
			strcpy(buf+strlen(insert), subject + pos);
			strcpy(subject + pos , buf);

			_free(buf);
		}
	}
}
//------------------------------------------------------------------------

void removeList(List* list){

	int i;

	// Exlcui cada item
	for(i=0 ; i<list->count ; i++){

		_free(list->itens[i]);
	}

	// Exclui o array dos itens
	_free(list->itens);
}
//------------------------------------------------------------------------

bool AsInteger(int* out,char *str,uint16_t index,const char a_delim){

	bool ret = false;

	char field[10];

	getField(field,str,index,a_delim);

	if(strlen(str)>0){

		*out = atoi(field);

		ret = true;
	}

	return ret;
}
//------------------------------------------------------------------------

bool AsString(char* out,char *str,uint16_t index,const char a_delim){

	getField(out,str,index,a_delim);

	return true;
}
//-----------------------------------------------------------------------

bool AsHex(uint16_t* out,char *str,uint16_t index,const char a_delim){

	bool ret = false;

	char field[5];

	getField(field,str,index,a_delim);

	if(strlen(str)>0){

		*out = strtol(field, NULL, 16);
		ret = true;
	}
	return ret;
}
//-----------------------------------------------------------------------

bool AsFloat(float* out,char *str,uint16_t index,const char a_delim){

	bool ret = false;

	char field[20];

	getField(field,str,index,a_delim);

	if(strlen(str)>0){

		*out = atof(field);
		ret = true;
	}

	return ret;
}
//------------------------------------------------------------------------

bool AsChar(char* out,char *str,uint16_t index,const char a_delim){

	bool ret = false;

	char field[2];

	getField(field,str,index,a_delim);

	if(strlen(str)>0){

		*out = field[0];
		ret = true;
	}

	return ret;
}
//------------------------------------------------------------------------

bool AsResource(Resource* out,char *str,uint16_t index,const char a_delim){

	bool ret = false;

	char field[4];

	getField(field,str,index,a_delim);

	if(strlen(str)>0){

		*out = getResource(field);
		ret = true;

	}else{
		*out = CMD_NONE;
	}

	return ret;
}
//------------------------------------------------------------------------
