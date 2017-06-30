/*
 * Array.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */


#include "string.h"
#include "Array.h"

inline unsigned int putDataArray(ArrayFrame *frame, char data) {

	if(frame!=NULL){
		frame->Data[frame->Count++] = data;
		frame->Count%=SIZE_FRAME;
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

void AppendPayLoad(ArrayPayLoad* payload,const char* data){

	strcat(payload->Data,data);
	payload->Count = strlen(payload->Data);
}
//------------------------------------------------------------------------

void AppendFrame(ArrayFrame* frame,const char* data){

	strcat(frame->Data,data);
	frame->Count = strlen(frame->Data);
}
//------------------------------------------------------------------------
