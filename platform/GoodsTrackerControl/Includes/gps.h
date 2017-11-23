/*
 * gps.h
 *
 *  Created on: 27/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_GPS_H_
#define SOURCES_GPS_H_

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include "NMEA.h"
#include "utils.h"

#define	GPS_BIT_RX_CHAR		0x01

/*
 * Estrutura de dados do frame NMEA
 *
 */

typedef struct{

	char	Identifier[NMEA_LEN_ID];
	char	Time_UTC[NMEA_LEN_TIME];
	char	Date[NMEA_LEN_DATE];
	float	Lat;
	float	Lng;

	char	LatDirection;
	char	LngDirection;
	char	Status;
	char	SelectionMode;
	char	Mode;
	int		PRNNumber[12];

	int		Speed;
	int		FixQuality;
	int		NumberOfSatelites;
	float	HDOP;
	float	PDOP;
	float	VDOP;
	float	Altitude;
	float	HGeoid;
	float	MagVariation;

} GPS;


void gps_publish(void);
void gps_init(void);

#define clearGPS(f) memset((void*)f,0,sizeof(GPS));


BaseType_t gps_notify_rx_char(BaseType_t *xHigherPriorityTaskWoken);

extern QueueHandle_t	xQueueGPS;
extern GPS	gps;

#endif /* SOURCES_GPS_H_ */
