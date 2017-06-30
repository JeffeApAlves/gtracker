/*
 * Data.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_DATA_H_
#define SOURCES_DATA_H_

#include <stdint.h>
#include <string.h>

typedef struct{

		float		Lat;
		float		Lng;
		uint16_t	Acc[3];
		uint16_t	Inc[3];
		int			Speed ;
		int			Level;
		int			Lock;
		char		Time[11];
		char		Date[7];
	} Info;


#define clearInfo(f) memset((void*)f,0,sizeof(Info));

#endif /* SOURCES_DATA_H_ */
