/*
 * broker.h
 *
 *  Created on: 21 de set de 2017
 *      Author: jefferson
 */

#ifndef COMPONENTS_BROKER_BROKER_H_
#define COMPONENTS_BROKER_BROKER_H_

#include "CommunicationFrame.h"

void data_cb(void *self, void *params);
void publish_cb(void *self, void *params);
void subscribe_cb(void *self, void *params);
void connected_cb(void *self, void *params);
void disconnected_cb(void *self, void *params);
void reconnect_cb(void *self, void *params);


void broker_init(void);
void startBroker(void);
void stopBroker(void);
void publishPackage(CommunicationPackage* frame);
void setClientID(int id);

#endif /* COMPONENTS_BROKER_BROKER_H_ */
