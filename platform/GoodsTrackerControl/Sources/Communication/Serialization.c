#include <stdio.h>
#include <string.h>

#include "XF1.h"

#include "clock.h"
#include "Serialization.h"

void tlm2String(Telemetria* info,PayLoad* ans){

	if(info!=NULL && ans!= NULL){

		clearArrayPayLoad(ans);

		XF1_xsprintf(ans->Data,"%.8f%c%.8f%c%d%c%d%c%d%c%.3f%c%.3f%c%.2f%c%d%c%d%c%d%c%d",
				info->GPS.Lat, 				CHAR_SEPARATOR,
				info->GPS.Lng, 				CHAR_SEPARATOR,
				info->Accelerometer.x, 		CHAR_SEPARATOR,
				info->Accelerometer.y, 		CHAR_SEPARATOR,
				info->Accelerometer.z, 		CHAR_SEPARATOR,
				info->Accelerometer.x_g,	CHAR_SEPARATOR,
				info->Accelerometer.y_g,	CHAR_SEPARATOR,
				info->Accelerometer.z_g,	CHAR_SEPARATOR,
				info->GPS.Speed, 			CHAR_SEPARATOR,
				info->Tank.Level, 			CHAR_SEPARATOR,
				info->Tank.Lock, 			CHAR_SEPARATOR,
				strToTimeStamp(info->GPS.Date,info->GPS.Time_UTC));

		ans->Length = strlen(ans->Data);
	}
}
//------------------------------------------------------------------------

void header2String(Header* header,char* out){

	char resource[4];

	getResourceName(resource,header->resource);

	XF1_xsprintf(out,"%05d%c%05d%c%010d%c%s%c%s%c%03d%c",
			header->address, 		CHAR_SEPARATOR,
			header->dest, 			CHAR_SEPARATOR,
			header->time_stamp,		CHAR_SEPARATOR,
			header->operacao, 		CHAR_SEPARATOR,
			resource,				CHAR_SEPARATOR,
			header->lengthPayLoad,	CHAR_SEPARATOR
	);
}
//------------------------------------------------------------------------