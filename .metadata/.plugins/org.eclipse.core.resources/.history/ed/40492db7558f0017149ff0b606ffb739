#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "XF1.h"
#include "clock.h"
#include "Serialization.h"

void tlm2String(Telemetria* info,PayLoad* ans){

	if(info!=NULL && ans!= NULL){

		clearArrayPayLoad(ans);

		XF1_xsprintf(ans->Data,"%.8f:%.8f:%d:%d:%d:%.3f:%.3f:%.2f:%d:%d:%d:%d",
				info->GPS.Lat,
				info->GPS.Lng,
				info->Accelerometer.x,
				info->Accelerometer.y,
				info->Accelerometer.z,
				info->Accelerometer.x_g,
				info->Accelerometer.y_g,
				info->Accelerometer.z_g,
				info->GPS.Speed,
				info->Tank.Level,
				info->Tank.Lock,
				getCurrentTimeStamp());

		ans->Length = strlen(ans->Data);
	}
}
//------------------------------------------------------------------------
