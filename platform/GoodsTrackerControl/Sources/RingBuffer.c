/*
 * CircularBuffer.c
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#include "RingBuffer.h"

bool putData(RingBuffer* buffer,char ch){

	bool flag = FALSE;

//	EnterCritical();
	vPortEnterCritical();

	if(!isFull(buffer)) {

		buffer->data[buffer->index_producer++] = ch;
		buffer->index_producer %= BUFFER_SIZE;
		if(buffer->count<BUFFER_SIZE){
			buffer->count++;
		}

		flag = TRUE;
	}

//	ExitCritical();
	vPortExitCritical();

	return flag;
}
//------------------------------------------------------------------------

bool getData(RingBuffer* buffer,char* ch){

	bool flag = FALSE;

//	EnterCritical();
	vPortEnterCritical();

	if(hasData(buffer)){

		*ch = buffer->data[buffer->index_consumer++];
		buffer->index_consumer %= BUFFER_SIZE;
		if(buffer->count>0){
			buffer->count--;
		}

		flag = TRUE;
	}

//	ExitCritical();
	vPortExitCritical();

	return flag;
}
//------------------------------------------------------------------------

inline short getCount(RingBuffer* buffer){

	return buffer!=NULL?buffer->count:0;
}
//------------------------------------------------------------------------

inline bool isFull(RingBuffer* buffer){

	return getCount(buffer)>= BUFFER_SIZE;
}
//------------------------------------------------------------------------

inline bool hasData(RingBuffer* buffer){

	return getCount(buffer)>0;
}
//------------------------------------------------------------------------
