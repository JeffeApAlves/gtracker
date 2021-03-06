/*
 * clock.h
 *
 *  Created on: Aug 28, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_CLOCK_H_
#define INCLUDES_CLOCK_H_

#include <stdint.h>
#include <stdbool.h>

#define SEC_PER_MIN         60
#define SEC_PER_HOUR        3600
#define SEC_PER_DAY         86400
#define MOS_PER_YEAR        12
#define EPOCH_YEAR          1970
#define IS_LEAP_YEAR(year)  ( (((year)%4 == 0) && ((year)%100 != 0)) || ((year)%400 == 0) )
#define	FUSO_HORARIO_BR		((-3)*SEC_PER_HOUR)

uint32_t getCurrentTimeStamp();
void initClock();
//bool setClock(LDD_RTC_TTime* time);
bool setClockByString(char* date,char* time);
//void getClock(LDD_RTC_TTime* time);
void updateEntityClock();
void adjusteClock();
//void strToData(	LDD_RTC_TTime* date_time,char* date,char* time);
uint32_t strToTimeStamp(char* date,char* time);
uint32_t unix_time_in_seconds(uint8_t sec, uint8_t min, uint8_t hrs,
		uint8_t day, uint8_t mon, uint16_t year);
uint32_t getTimeStamp();
//bool getLocalClock(LDD_RTC_TTime* time);


typedef enum{CLOCK_INIT,CLOCK_STARTED,CLOCK_UPDATE,CLOCK_ADJUSTED,CLOCK_ERROR} STATUS_CLOCK;

extern volatile STATUS_CLOCK statuc_clock;

extern bool flag_1s;

#endif /* INCLUDES_CLOCK_H_ */
