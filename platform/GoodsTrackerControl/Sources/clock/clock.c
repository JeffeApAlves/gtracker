/*
 * clock.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Jefferson
 */
#include "Cpu.h"
#include "stdlib.h"
#include "string.h"
#include "XF1.h"
#include "Telemetria.h"
#include "clock.h"

// Referencias:	https://github.com/msolters/make-unix-timestamp-c
//				https://community.nxp.com/docs/DOC-94734

const static char *DayOfWeekName[] = {
  "Dom","Seg","Ter","Qua","Qui","Sex","Sab"
};

const static int days_per_month[2][MOS_PER_YEAR] = {
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
  { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

const static int days_per_year[2] = {
  365, 366
};

LDD_TDeviceData		*MyRTCPtr;
LDD_RTC_TTime		Time;

volatile STATUS_CLOCK statuc_clock = CLOCK_INIT;

bool flag_1s = TRUE;

void initClock(){

	/* Initialize the device, preserve time settings */
	MyRTCPtr = RTC1_Init((LDD_TUserData *)NULL, FALSE);

	if(MyRTCPtr!=NULL){
/*
		LDD_RTC_TTime date_time = {

			.Hour		= 0,
			.Minute		= 0,
			.Second		= 0,
			.Year		= 1970,
			.Month		= 1,
			.Day		= 1,
			.timestamp	= 0
		};

		setClock(&date_time);
*/
		statuc_clock = CLOCK_STARTED;
	}
}
//-------------------------------------------------------------------------------------------------------------------

/*
 *
 * Configura novo horario
 *
 *
 */
bool setClockByString(char* date,char* time){

	LDD_RTC_TTime date_time;

	strToData(&date_time,date,time);

	return setClock(&date_time);
}
//-------------------------------------------------------------------------------------------------------------------

bool setClock(LDD_RTC_TTime* time){

	LDD_TError	Error;

	bool flag = FALSE;

	if(		time!=NULL &&
			time->Year>EPOCH_YEAR &&
			time->Day>0 && time->Day<=31 &&
			time->Month>0 && time->Month<=12){

		Error	= RTC1_SetTime(MyRTCPtr,time);

		if(Error==ERR_OK){

			getClock(&Time);

			flag = TRUE;
		}
	}

	return flag;
}
//-------------------------------------------------------------------------------------------------------------------


void getClock(LDD_RTC_TTime* time){

	RTC1_GetTime(MyRTCPtr, time);
	Time.timestamp = getCurrentTimeStamp();
}
//-------------------------------------------------------------------------------------------------------------------

void updateEntityClock(){

	if(statuc_clock == CLOCK_ADJUSTED){

		RTC1_GetTime(MyRTCPtr, &Time);
		Time.timestamp++;
	}

	flag_1s = TRUE;
}
//-------------------------------------------------------------------------------------------------------------------

void adjusteClock(){

	if(statuc_clock == CLOCK_STARTED){

		if(setClockByString(telemetria.GPS.Date,telemetria.GPS.Time_UTC)){

			statuc_clock = CLOCK_ADJUSTED;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------

uint32_t getCurrentTimeStamp(){

	return unix_time_in_seconds((uint8_t)Time.Second, (uint8_t)Time.Minute, (uint8_t)Time.Hour,
			(uint8_t)Time.Day, (uint8_t)Time.Month, (uint16_t)Time.Year);
}
//-------------------------------------------------------------------------------------------------------------------

void strToData(	LDD_RTC_TTime* date_time,char* date,char* time){

	char year[3];	year[2]		= '\0';
	char month[3];	month[2]	= '\0';
	char day[3];	day[2]		= '\0';
	char hrs[3];	hrs[2] 		= '\0';
	char min[3];	min[2] 		= '\0';
	char sec[3];	sec[2] 		= '\0';

	memset(date_time,0,sizeof(LDD_RTC_TTime));

	if(strlen(time)>=6){
		strncpy(hrs, 	time,2);
		strncpy(min, 	time+2,2);
		strncpy(sec, 	time+4,2);

		date_time->Hour		= atoi(hrs);
		date_time->Minute	= atoi(min);
		date_time->Second	= atoi(sec);
	}

	if(strlen(date)>=6){

		strncpy(day, 	date,2);
		strncpy(month,	date+2,2);
		strncpy(year,	date+4,2);

		date_time->Year		= atoi(year);
		date_time->Month	= atoi(month);
		date_time->Day		= atoi(day);

		// ano de 2 digitos
		if(date_time->Year>0 && date_time->Year<100){
			date_time->Year+=2000;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------

uint32_t unix_time_in_seconds(uint8_t sec, uint8_t min, uint8_t hrs,
		uint8_t day, uint8_t mon, uint16_t year) {

	uint32_t ts = 0;

	//  Add up the seconds from all prev years, up until this year.
	uint8_t years = 0;
	uint8_t leap_years = 0;
	for (uint16_t y_k = EPOCH_YEAR; y_k < year; y_k++) {
		if (IS_LEAP_YEAR(y_k)) {
			leap_years++;
		} else {
			years++;
		}
	}
	ts += ((years * days_per_year[0]) + (leap_years * days_per_year[1]))
			* SEC_PER_DAY;

	//  Add up the seconds from all prev days this year, up until today.
	uint8_t year_index = (IS_LEAP_YEAR(year)) ? 1 : 0;
	for (uint8_t mo_k = 0; mo_k < (mon - 1); mo_k++) { //  days from previous months this year
		ts += days_per_month[year_index][mo_k] * SEC_PER_DAY;
	}
	ts += (day - 1) * SEC_PER_DAY; // days from this month

	//  Calculate seconds elapsed just today.
	ts += hrs * SEC_PER_HOUR;
	ts += min * SEC_PER_MIN;
	ts += sec;

	return ts;
}
//---------------------------------------------------------------------------------------------------------
