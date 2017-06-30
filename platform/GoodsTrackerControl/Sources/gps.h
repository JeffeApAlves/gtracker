/*
 * gps.h
 *
 *  Created on: 27/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_GPS_H_
#define SOURCES_GPS_H_

#include <stdbool.h>
#include "PE_Types.h"
#include "utils.h"
#include "NMEAFrame.h"

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
	NMEA_FRAME_NOK,
	NMEA_FRAME_OK,
	NMEA_EXEC,
	NMEA_EXEC_ERROR,

} StatusNMEA;

static void NMEA_rxStart(void);
static void NMEA_receiveFrame(void);
static void NMEA_receiveCheckSum(void);
static void NMEA_rxCR(void);
static void NMEA_rxLF(void);
static void NMEA_verifyFrame(void);
static void NMEA_acceptRxFrame(void);
static void NMEA_sendResult(void);
static void NMEA_errorRxFrame(void);
static void NMEA_errorExec(void);

static void setGPSStatus(StatusNMEA sts);
static bool NMEA_decoderFrame(void);
static void decoderGGA(List* list,DataNMEA* data);
static void decoderRMC(List* list,DataNMEA* data);
static void decoderGSA(List* list,DataNMEA* data);

//API
bool getGPSData(char* ch);
inline bool putGPSData(char data);
void NMEA_Run(void);
void NMEA_init(void);

#endif /* SOURCES_GPS_H_ */
