/*
 * broker.c
 *
 *  Created on: 21 de set de 2017
 *      Author: jefferson
 */
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "serialization.h"
#include "communication.h"
#include "protocol.h"
#include "mqtt.h"
#include "Frame.h"
#include "broker.h"

static const char *TAG = "broker";

static mqtt_client *client = NULL;

mqtt_settings settings = {

    .host = "192.168.42.1",
#if defined(CONFIG_MQTT_SECURITY_ON)
    .port = 8883, // encrypted
#else
    .port = 1883, // unencrypted
#endif
    .client_id = "tracker_default",
    .username = "senai_device_01",
    .password = "senai_device_01",
    .clean_session = 0,
    .keepalive = 120,
    .lwt_topic = "/lwt",
    .lwt_msg = "offline",
    .lwt_qos = 0,
    .lwt_retain = 0,
    .connected_cb = connected_cb,
    .disconnected_cb = disconnected_cb,
    //.reconnect_cb = reconnect_cb,
    .subscribe_cb = subscribe_cb,
    .publish_cb = publish_cb,
    .data_cb = data_cb
};
//--------------------------------------------------------------------

void startBroker(void){

	mqtt_start(&settings);
}
//--------------------------------------------------------------------

void stopBroker(void){

    mqtt_stop();
}
//--------------------------------------------------------------------

void connected_cb(void *self, void *params){

	client = (mqtt_client *)self;
//	mqtt_subscribe(client, "/CMD00004", 0);
}
//--------------------------------------------------------------------

void disconnected_cb(void *self, void *params){

}
//--------------------------------------------------------------------

void reconnect_cb(void *self, void *params){

}
//--------------------------------------------------------------------

void subscribe_cb(void *self, void *params){

}
//--------------------------------------------------------------------

void publish_cb(void *self, void *params){

}
//--------------------------------------------------------------------

void data_cb(void *self, void *params){

    mqtt_event_data_t *event_data = (mqtt_event_data_t *)params;

    if(event_data->data_offset == 0) {

        char *topic = malloc(event_data->topic_length + 1);
        memcpy(topic, event_data->topic, event_data->topic_length);
        topic[event_data->topic_length] = 0;
        ESP_LOGI(TAG, "[APP] Publish topic: %s", topic);
        free(topic);
    }

    // char *data = malloc(event_data->data_length + 1);
    // memcpy(data, event_data->data, event_data->data_length);
    // data[event_data->data_length] = 0;
    ESP_LOGI(TAG, "[APP] Publish data[%d/%d bytes]",
             event_data->data_length + event_data->data_offset,
             event_data->data_total_length);
}
//--------------------------------------------------------------------

void publishPackage(CommunicationPackage* package){

	if(client!=NULL && package!=NULL){

		Frame	frame;

		char	strtopic[50];

		package2Frame(package,&frame,false);

		sprintf(strtopic,"tlm/%05d",package->Header.address);

		mqtt_publish(client, strtopic,frame.Data, frame.Length , 0, 0);
	}
}
//--------------------------------------------------------------------

void setClientID(int id){

	char tracker_id[32];

	sprintf(tracker_id,"TRACKER_%05d",id);

	strcpy(settings.client_id,tracker_id);
}
//--------------------------------------------------------------------

void broker_init(void){

	setClientID(TRACKER_ADDRESS);
}
//--------------------------------------------------------------------
