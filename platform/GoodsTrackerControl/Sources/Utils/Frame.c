/*
 * Frame.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "string.h"
#include "Frame.h"

inline unsigned int putDataArray(Frame *frame, char data) {

	if(frame!=NULL){
		frame->Data[frame->Length++] = data;
		frame->Length%=LEN_FRAME;
	}
}
//----------------------------------------------------------------------------------

/**
 *
 * Calcula o checkSum da string buff
 *
 */
unsigned int calcChecksum(const char *buff, size_t sz) {

	int i;
	unsigned char chk	= 0;

	if (buff) {

		for (i = 0; i < sz; i++){
			chk ^= buff[i];
		}
	}

	return chk;
}
//------------------------------------------------------------------------

void AppendFrame(Frame* frame,const char* data){

	strcat(frame->Data,data);
	frame->Length = strlen(frame->Data);
}
//------------------------------------------------------------------------

uint16_t getNumField(char* str,const char a_delim) {

	uint16_t i,f;

	i		= 0;
	f		= 0;

	while((str[i]!='\0') && (i<LEN_FRAME)){

		if(str[i++]==a_delim){

			f++;
		}
	}

	return f+1;
}
//------------------------------------------------------------------------

void getField(char* str, char* out,uint16_t num_field,const char a_delim) {

	uint16_t i,start,end,index_field;

	index_field	= 0;		// ja aponta para o field 0
	start		= 0;		// ques inicia na posicao 0
	end			= 0;
	out[0]		= '\0';

	for(i=0;i<LEN_FRAME;i++){

		if(str[i]=='\0'){

			end = i;
			break;
		}

		if(str[i]==a_delim){

			// Proximo campo
			index_field++;

			if(index_field==num_field){

				start = i+1;
			}

			if(index_field==(num_field+1)){

				end = i;
				break;
			}
		}
	}

	if((end-start)>0){

		strncpy(out,str+start,end-start);
		out[end-start] = '\0';
	}
}
//------------------------------------------------------------------------
