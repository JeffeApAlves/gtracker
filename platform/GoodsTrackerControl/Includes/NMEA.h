/*
 * NMEAFrame.h
 *
 *  Created on: 28/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_NMEAFRAME_H_
#define SOURCES_NMEAFRAME_H_

#include "RingBuffer.h"
#include "Frame.h"

#define	NMEA_SIZE_MIN_FRAME		30
#define	NMEA_LEN_CHECKSUM		2
#define	NMEA_LEN_TIME			11
#define	NMEA_LEN_DATE			7
#define	NMEA_LEN_ID				6

#define NMEA_CHAR_START		'$'
#define NMEA_CHAR_END		'*'
#define NMEA_CHAR_SEPARATOR	','
#define NMEA_CHAR_CR		'\r'
#define NMEA_CHAR_LF		'\n'
#define NMEA_CHAR_STR_END	'\0'

/**
 *
 * Maquina de estado para recebimento do frame
 */
typedef enum {

	NMEA_INIT,
	NMEA_INIT_OK,
	NMEA_RX_START,
	NMEA_RX_FRAME,
	NMEA_RX_END,
	NMEA_RX_CHECKSUM,
	NMEA_RX_LF,
	NMEA_RX_CR,

} StatusNMEA;

//API
void receiveNMEA(void);

extern RingBuffer		bufferRxNMEA;

#endif /* SOURCES_NMEAFRAME_H_ */
