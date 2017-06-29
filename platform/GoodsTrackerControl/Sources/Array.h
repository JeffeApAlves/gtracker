/*
 * Array.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_ARRAY_H_
#define SOURCES_ARRAY_H_

#include "stdlib.h"

#define SIZE_HEADER		30
#define SIZE_PAYLOAD	256
#define SIZE_FRAME		300
#define SIZE_CHECKSUM	3

typedef struct{

	int		CountCheckSum;
	char	checksum[SIZE_CHECKSUM];

	int		Count;
	char	Data[SIZE_FRAME];

} ArrayFrame;


typedef struct{

	int		Count;
	char	Data[SIZE_PAYLOAD];

} ArrayPayLoad;


typedef struct{

	int		count;
	char	data[SIZE_HEADER];

} ArrayHeader;


unsigned int calcChecksum(const char *buff, size_t sz);

#define clearArrayFrame(f)		memset((void*)f,0,sizeof(ArrayFrame));
#define clearArrayPayLoad(f)	memset((void*)f,0,sizeof(ArrayPayLoad));
//#define clearArrayHeader(f)		memset((void*)f,0,sizeof(ArrayHeader));

inline unsigned int putDataArray(ArrayFrame *frame, char data);

#endif /* SOURCES_ARRAY_H_ */
