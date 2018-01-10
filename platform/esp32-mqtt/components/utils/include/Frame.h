/*
 * Array.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_FRAME_H_
#define SOURCES_FRAME_H_

#include <stdlib.h>
#include <stdint.h>

#define	LEN_FRAME			300
#define ARRAY_LEN_CHECKSUM	3

typedef struct{

	//usado na recepção do checksum
	int		CountCheckSum;

	// armazena o checksum
	char	checksum[ARRAY_LEN_CHECKSUM];

	// armazena todo o frame
	int		Length;
	char	Data[LEN_FRAME];

} Frame;

#define clearFrame(f)	memset((void*)f,0,sizeof(Frame));

unsigned int calcChecksum(const char *buff, size_t sz);
unsigned int putDataArray(Frame *frame, char data);
void AppendFrame(Frame* frame,const char* data);
void subString(char* out,char* str, uint16_t start_field,uint16_t end_field,const char a_delim);
void getField(char* out,char* str, uint16_t num,const char a_delim);
uint16_t getNumField(char* str,const char a_delim);

#endif /* SOURCES_ARRAY_H_ */
