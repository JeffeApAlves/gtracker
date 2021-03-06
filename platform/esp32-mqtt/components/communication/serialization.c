#include <string.h>

#include "clock.h"
#include "Frame.h"
#include "serialization.h"

static const char SEPARATOR[] = {CHAR_SEPARATOR,CHAR_STR_END};

void tlm2String(Telemetria* tlm,PayLoad* ans){

	if(tlm!=NULL && ans!= NULL){

		sprintf(ans->Data,"%.8f%c%.8f%c%d%c%d%c%d%c%.3f%c%.3f%c%.2f%c%d%c%d%c%d%c%d",
				tlm->GPS.Lat, 				CHAR_SEPARATOR,
				tlm->GPS.Lng, 				CHAR_SEPARATOR,
				tlm->Accelerometer.x, 		CHAR_SEPARATOR,
				tlm->Accelerometer.y, 		CHAR_SEPARATOR,
				tlm->Accelerometer.z, 		CHAR_SEPARATOR,
				tlm->Accelerometer.x_g,		CHAR_SEPARATOR,
				tlm->Accelerometer.y_g,		CHAR_SEPARATOR,
				tlm->Accelerometer.z_g,		CHAR_SEPARATOR,
				tlm->GPS.Speed, 			CHAR_SEPARATOR,
				tlm->Tank.Level, 			CHAR_SEPARATOR,
				tlm->Tank.Lock, 			CHAR_SEPARATOR,
				strToTimeStamp(tlm->GPS.Date,tlm->GPS.Time_UTC));

		ans->Length = strlen(ans->Data);
	}
}
//------------------------------------------------------------------------

void header2String(Header* header,char* out){

	char resource[4];

	getResourceName(resource,header->resource);

	sprintf(out,"%05d%c%05d%c%010d%c%s%c%s%c%03d%c",
			header->address, 		CHAR_SEPARATOR,
			header->dest, 			CHAR_SEPARATOR,
			header->time_stamp,		CHAR_SEPARATOR,
			header->operacao, 		CHAR_SEPARATOR,
			resource,				CHAR_SEPARATOR,
			header->lengthPayLoad,	CHAR_SEPARATOR
	);
}
//------------------------------------------------------------------------

void getResourceName(char* out,Resource resource) {

	switch(resource){

		default:
		case CMD_NONE:		strcpy(out,"---");	break;
		case CMD_LOCK:		strcpy(out,"LCK");	break;
		case CMD_TLM:		strcpy(out,"TLM");	break;
		case CMD_LCD:		strcpy(out,"LCD");	break;
		case CMD_TOUCH:		strcpy(out,"TOU");	break;
		case CMD_ACC:		strcpy(out,"ACC");	break;
		case CMD_PWM:		strcpy(out,"PWM");	break;
		case CMD_ANALOG:	strcpy(out,"ANL");	break;
		case CMD_LED:		strcpy(out,"LED");	break;
	}
}
//------------------------------------------------------------------------

inline void checkSum2String(unsigned int checksum,char* out) {

	sprintf(out, "%c%02X", CHAR_SEPARATOR , checksum);
}
//------------------------------------------------------------------------

/**
 *
 *  Faz a serialização de um pacote
 *
 */
void package2Frame(CommunicationPackage* package,Frame* frame,bool with_checksum) {

	clearFrame(frame);

	// Serialização do cabeçalho
	header2String(&package->Header,frame->Data);

	//Concatena com o payload
	AppendFrame(frame,package->PayLoad.Data);

	// calcula e concatena o checksum
	if(with_checksum){

		AppendFrame(frame,SEPARATOR);
		sprintf(frame->checksum, "%02X", calcChecksum (frame->Data,frame->Length));
		AppendFrame(frame,frame->checksum);
	}
}
//------------------------------------------------------------------------
