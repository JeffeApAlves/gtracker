/*
 * Serialization.h
 *
 *  Created on: Aug 27, 2017
 *      Author: Jefferson
 */

#ifndef SOURCES_SERIALIZATION_H_
#define SOURCES_SERIALIZATION_H_

#include <CommunicationFrame.h>
#include "Telemetria.h"

void tlm2String(Telemetria* info,PayLoad* ans);

#endif /* SOURCES_SERIALIZATION_H_ */
