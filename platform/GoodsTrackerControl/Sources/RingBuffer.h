/*
 * CircularBuffer.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_RINGBUFFER_H_
#define SOURCES_RINGBUFFER_H_

#include "Cpu.h"
#include "PE_Types.h"

#define BUFFER_SIZE		256

typedef struct{

	unsigned char	data[BUFFER_SIZE];
	unsigned char	index_producer;
	unsigned char	index_consumer;
	int				count;

} RingBuffer;

bool getData(RingBuffer* buffer,char* ch);
bool putData(RingBuffer* buffer,char ch);
bool hasData(RingBuffer* buffer);
bool isFull(RingBuffer* buffer);
unsigned int getCount(RingBuffer* buffer);

#define clearBuffer(buf)	memset(buf,0,sizeof(RingBuffer))

#endif /* SOURCES_RINGBUFFER_H_ */
