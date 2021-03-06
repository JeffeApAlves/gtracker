/*
 * CircularBuffer.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_RINGBUFFER_H_
#define SOURCES_RINGBUFFER_H_

#include <stdbool.h>
//#include "Cpu.h"
//#include "PE_Types.h"

#define BUFFER_SIZE		128

typedef struct{

	unsigned char	data[BUFFER_SIZE];
	short	index_producer;
	short	index_consumer;
	short	count;

} RingBuffer;

bool getData(RingBuffer* buffer,char* ch);
bool putData(RingBuffer* buffer,char ch);
short getCount(RingBuffer* buffer);
void putString(RingBuffer* buffer,const char* str);

#define clearBuffer(buf)	memset((void*)buf,0,sizeof(RingBuffer))
#define hasData(buf)		(getCount(buf)>0)
#define isFull(buf)			(getCount(buf)>= BUFFER_SIZE)

#endif /* SOURCES_RINGBUFFER_H_ */
