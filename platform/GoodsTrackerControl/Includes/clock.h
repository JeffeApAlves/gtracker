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

#include "RTC1.h"

typedef LDD_RTC_TTime		rtc_clock;

#define SEC_PER_MIN         60
#define SEC_PER_HOUR        3600
#define SEC_PER_DAY         86400
#define MOS_PER_YEAR        12
#define EPOCH_YEAR          1970
#define IS_LEAP_YEAR(year)  ( (((year)%4 == 0) && ((year)%100 != 0)) || ((year)%400 == 0) )
#define	FUSO_HORARIO_BR		((-3)*SEC_PER_HOUR)

uint32_t getCurrentTimeStamp();
void clock_init();
bool setClock(rtc_clock* clock);
bool setClockByString(char* date,char* time);
void getClock(rtc_clock* clock);
void updateEntityClock();
void adjusteClock(char* date,char* time);
void strToData(	rtc_clock* clock,char* date,char* time);
uint32_t strToTimeStamp(char* date,char* time);
uint32_t unix_time_in_seconds(uint8_t sec, uint8_t min, uint8_t hrs,
		uint8_t day, uint8_t mon, uint16_t year);
uint32_t getTimeStamp();
bool getLocalClock(rtc_clock* clock);


typedef enum{CLOCK_INIT,CLOCK_STARTED,CLOCK_UPDATE,CLOCK_ADJUSTED,CLOCK_ERROR} STATUS_CLOCK;

extern volatile STATUS_CLOCK statuc_clock;

#endif /* INCLUDES_CLOCK_H_ */
