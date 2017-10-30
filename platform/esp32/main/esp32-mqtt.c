#include <stdio.h>

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "led.h"
#include "gps.h"
#include "accelerometer.h"
#include "tank.h"
#include "consumer.h"
#include "broker.h"
#include "network.h"
#include "serial.h"
#include "communication.h"

static const char *TAG = "app_main";

void app_main()
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] SDK version: %s", esp_get_idf_version());

    nvs_flash_init();
    LED_init();
    wifi_conn_init();
    broker_init();
    uart_init();
    communication_init();
    consumer_init();
    tank_init();
    accelerometer_init();
    gps_init();
}
//-----------------------------------------------------------------------
