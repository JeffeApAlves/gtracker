/*
 * utils.c
 *
 *  Created on: 07/06/2017
 *      Author: Jefferson
 */

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "utils.h"

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
 *  Use strpbrk() for multiple delimiters.
 *
 **/
buffer_t memtok(const void *s, size_t length, const char *delim, buffer_t *save_ptr){

    const unsigned char *stream,
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

void AsString(char* out,List *list,int index){

	if(index<list->count && out!=NULL){

		strcpy(out, list->itens[index]);
	}
}
//------------------------------------------------------------------------

void AsInteger(int* out,List *list,int index){

	if(index<list->count && out!=NULL){

		*out = atoi(list->itens[index]);
	}
}
//------------------------------------------------------------------------

void AsHex(int* out,List *list,int index){

	if(index<list->count && out!=NULL){

		*out = strtol(list->itens[index], NULL, 16);
	}
}
//------------------------------------------------------------------------

void AsFloat(float* out,List *list,int index){

	if(index<list->count && out!=NULL){

		*out = atof(list->itens[index]);
	}
}
//------------------------------------------------------------------------

void AsChar(char* out,List *list,int index){

	if(index<list->count && out!=NULL){

		*out = list->itens[index][0];
	}
}
//------------------------------------------------------------------------
