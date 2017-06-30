/*
 * Data.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_DATATLM_H_
#define SOURCES_DATATLM_H_

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
	} DataTLM;


#define clearDataTLM(f) memset((void*)f,0,sizeof(DataTLM));

#endif /* SOURCES_DATATLM_H_ */
