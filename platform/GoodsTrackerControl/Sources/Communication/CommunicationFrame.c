/*
 * DataFrame.c
 *
 *  Created on: Aug 31, 2017
 *      Author: Jefferson
 */

#include <CommunicationFrame.h>
#include <string.h>


void AppendPayLoad(PayLoad* payload,const char* data){

	strcat(payload->Data,data);
	payload->Length = strlen(payload->Data);
}
//------------------------------------------------------------------------
