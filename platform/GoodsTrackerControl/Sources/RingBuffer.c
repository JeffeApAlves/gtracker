/*
 * CircularBuffer.c
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#include "RingBuffer.h"

bool putData(RingBuffer* buffer,char ch){

	bool flag = FALSE;

	EnterCritical();

	if(!isFull(buffer)) {

		buffer->data[buffer->index_producer++] = ch;
		buffer->index_producer %= BUFFER_SIZE;
		buffer->count++;

		flag = TRUE;
	}

	ExitCritical();

	return flag;
}
//------------------------------------------------------------------------

bool getData(RingBuffer* buffer,char* ch){

	bool flag = FALSE;

	EnterCritical();

	if(hasData(buffer)){

		*ch = buffer->data[buffer->index_consumer++];
		buffer->index_consumer %= BUFFER_SIZE;
		buffer->count--;

		flag = TRUE;
	}

	ExitCritical();

	return flag;
}
//------------------------------------------------------------------------

unsigned int getCount(RingBuffer* buffer){

	unsigned int r = 0;

	if(buffer!=NULL){

		r = buffer->count;
	}

	return r;
}
//------------------------------------------------------------------------

bool isFull(RingBuffer* buffer){

	return getCount(buffer)>= BUFFER_SIZE;
}
//------------------------------------------------------------------------

bool hasData(RingBuffer* buffer){

	return getCount(buffer)>0;
}
//------------------------------------------------------------------------
