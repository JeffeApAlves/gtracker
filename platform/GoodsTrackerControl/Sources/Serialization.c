#include "XF1.h"
#include "Serialization.h"


void tlm2String(DataTLM* info,ArrayPayLoad* ans){

	if(info!=NULL && ans!= NULL){

		clearArrayPayLoad(ans);

	XF1_xsprintf(ans->Data,"%.8f:%.8f:%d:%d:%d:%.3f:%.3f:%.2f:%d:%d:%d:%s:%s",
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
				info->GPS.Time,
				info->GPS.Date);

		ans->Length = strlen(ans->Data);
	}
}
//------------------------------------------------------------------------

