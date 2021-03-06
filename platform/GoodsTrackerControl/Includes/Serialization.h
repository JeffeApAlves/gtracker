/*
 * Serialization.h
 *
 *  Created on: Aug 27, 2017
 *      Author: Jefferson
 */

#ifndef SOURCES_SERIALIZATION_H_
#define SOURCES_SERIALIZATION_H_

#include <stdbool.h>

#include "XF1.h"

#include "CommunicationFrame.h"
#include "Telemetria.h"

void getResourceName(char* out,Resource resource);
void tlm2String(Telemetria* info,PayLoad* ans);
void header2String(Header* header,char* out);
void package2Frame(CommunicationPackage* package,Frame* frame,bool with_checksum);
void checkSum2String(unsigned int checksum,char* out);


#define	TO_STRING	XF1_xsprintf

#endif /* SOURCES_SERIALIZATION_H_ */
