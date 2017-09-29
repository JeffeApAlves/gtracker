/*
 * serial.c
 *
 *  Created on: 23 de set de 2017
 *      Author: jefferson
 */

// Task para tratamento da recepção

#include <string.h>

#include "sdkconfig.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "protocol.h"
#include "serial.h"

static const char *TAG = "serial";

RingBuffer			bufferTx;

// Fila de recepção da uart
static QueueHandle_t uart0_queue;

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(BUF_SIZE);

    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {

            ESP_LOGI(TAG, "uart[%d] event:", UART_NUM);

            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.
                in this example, we don't process data in event, but read data outside.*/
                case UART_DATA:
                    uart_get_buffered_data_len(UART_NUM, &buffered_size);
                    ESP_LOGI(TAG, "data, len: %d; buffered len: %d", event.size, buffered_size);

                    receivePackage();
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow\n");
                    //If fifo overflow happened, you should consider adding flow control for your application.
                    //We can read data out out the buffer, or directly flush the rx buffer.
                    uart_flush(UART_NUM);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full\n");
                    //If buffer full happened, you should consider encreasing your buffer size
                    //We can read data out out the buffer, or directly flush the rx buffer.
                    uart_flush(UART_NUM);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break\n");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error\n");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error\n");
                    break;
                //UART_PATTERN_DET
                case UART_PATTERN_DET:
                    ESP_LOGI(TAG, "uart pattern detected\n");
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d\n", event.type);
                    break;
            }
        }
    }

    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}
//-------------------------------------------------------------------------------------------------

void uart_init(void){

	// Clear buffer circular
	clearBuffer(&bufferTx);

	// Parametros de configuração
	uart_config_t uart_config = {
	   .baud_rate = 57600,
	   .data_bits = UART_DATA_8_BITS,
	   .parity = UART_PARITY_DISABLE,
	   .stop_bits = UART_STOP_BITS_1,
	   .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
	   .rx_flow_ctrl_thresh = 122,
	};

	// Configura uart
	uart_param_config(UART_NUM, &uart_config);

	// Configura nivel do log
	esp_log_level_set(TAG, ESP_LOG_INFO);

	//Instala driver da uart e pega fila.
	uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 10, &uart0_queue, 0);

	//Configura pinos da UART (usando pinos default)
	// uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

	uart_set_pin(UART_NUM, PIN_TXD,PIN_RXD,PIN_RTS,PIN_CTS);

	//Set uart pattern detect function.
	uart_enable_pattern_det_intr(UART_NUM, '+', 3, 10000, 10, 10);

	//Create a task to handler UART event from ISR
	xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}
//------------------------------------------------------------------------------------

inline bool uart_wr_bytes(const char* data, size_t len){

	return uart_write_bytes(UART_NUM, (const char*) data, len)>=0;
}
//------------------------------------------------------------------------------------

inline bool uart_wr_data(const char data){

	printf("%c",data);
	return uart_write_bytes(UART_NUM, (const char*) &data, 1)>=0;
}
//------------------------------------------------------------------------------------

inline bool uart_wr_string(const char* data){

	int len = strlen(data);

	if(len>0){
		printf(data);
		printf("\n");
	}

	return uart_write_bytes(UART_NUM, (const char*) data, len )>=0;
}
//------------------------------------------------------------------------------------

inline bool putTxData(char data) {

	return uart_wr_data(data);
}
//------------------------------------------------------------------------

inline bool putTxString(const char* str) {

	return uart_wr_string(str);
}
//------------------------------------------------------------------------

inline bool putRxData(char data) {

	return true;
}
//------------------------------------------------------------------------

bool getRxData(char* ch){

	uint8_t byte[2];

	int len = uart_read_bytes(UART_NUM, byte, 1 ,10/ portTICK_RATE_MS);

	*ch = (char)byte[0];


	return len>0;
}
//------------------------------------------------------------------------

/*
 * TODO como recuperar uma informação do nuffer de sáida
 * Pega um char do buffer circular da transmissao
 *
 */
inline bool getTxData(char* ch){

	return 0;
}
//------------------------------------------------------------------------


/**
 * TODO como verificar se existe dados nu buffer de saída
 * Verifica se existe dados no buffer circular de transmiassa
 */
inline bool hasTxData(void){

	return 0;
}
//------------------------------------------------------------------------

void startTX(void){
}
//------------------------------------------------------------------------

/*
 *
 * Verifica se exsite algum dado no buffer circular de recebimento
 *
 */
inline bool isAnyRxData(){

	size_t buffered_size;

	esp_err_t st = uart_get_buffered_data_len(UART_NUM, &buffered_size);

	return (st==ESP_OK) && (buffered_size>0);
}
//------------------------------------------------------------------------
