/*
 * Amazon FreeRTOS Greengrass Demo V1.2.6
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file aws_greengrass_discovery_demo.c
 * @brief A simple Greengrass discovery example.
 *
 * A simple example that perform discovery of the greengrass core device.
 * The JSON file is retrieved.
 */


/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"

/* Greengrass includes. */
#include "aws_ggd_config.h"
#include "aws_ggd_config_defaults.h"
#include "aws_greengrass_discovery.h"
#include "aws_greengrass_discovery_demo.h"

/* MQTT includes. */
#include "aws_mqtt_agent.h"

/* Demo includes. */
#include "aws_demo_config.h"

#define ggdDEMO_MAX_MQTT_MESSAGES      3
#define ggdDEMO_MAX_MQTT_MSG_SIZE      500
#define ggdDEMO_DISCOVERY_FILE_SIZE    2500
#define ggdDEMO_MQTT_MSG_TOPIC         "dt/dht11/climate"
#define ggdDEMO_MQTT_MSG_DISCOVERY     "{\"tempF\": %i,\"humidity\": %i}"

/* to access gpio of esp */
#include "driver/gpio.h"
#include "driver/rmt.h"
#include "soc/rmt_reg.h"
#include "driver/dht11.h"

const int wakeup_time_sec = 300;

/**
 * @brief Contains the user data for callback processing.
 */
typedef struct
{
    const char * pcExpectedString;      /**< Informs the MQTT callback of the next expected string. */
    BaseType_t xCallbackStatus;         /**< Used to communicate the success or failure of the callback function.
                                         * xCallbackStatus is set to pdFALSE before the callback is executed, and is
                                         * set to pdPASS inside the callback only if the callback receives the expected
                                         * data. */
    SemaphoreHandle_t xWakeUpSemaphore; /**< Handle of semaphore to wake up the task. */
    char * pcTopic;                     /**< Topic to subscribe and publish to. */
} GGDUserData_t;

/* The maximum time to wait for an MQTT operation to complete.  Needs to be
 * long enough for the TLS negotiation to complete. */
static const TickType_t xMaxCommandTime = pdMS_TO_TICKS( 20000UL );
static const TickType_t xTimeBetweenPublish = pdMS_TO_TICKS( 1500UL );
static char pcJSONFile[ ggdDEMO_DISCOVERY_FILE_SIZE ];

/*
 * The MQTT client used for all the publish and subscribes.
 */
static MQTTAgentHandle_t xMQTTClientHandle;
static BaseType_t prvMQTTConnect( GGD_HostAddressData_t * pxHostAddressData );
static void prvSendMessageToGGC( GGD_HostAddressData_t * pxHostAddressData );
static void prvDiscoverGreenGrassCore( void * pvParameters );

/*-----------------------------------------------------------*/

static void prvSendMessageToGGC( GGD_HostAddressData_t * pxHostAddressData )
{
    const char * pcTopic = ggdDEMO_MQTT_MSG_TOPIC;
    MQTTAgentPublishParams_t xPublishParams;
    MQTTAgentReturnCode_t xReturnCode;
    uint32_t ulMessageCounter;
    char cBuffer[ ggdDEMO_MAX_MQTT_MSG_SIZE ];
    const int gpio_pin    = GPIO_NUM_5;
    const int rmt_channel = 0;

    while( prvMQTTConnect( pxHostAddressData ) == pdPASS )
    {
        configPRINTF( ( "Connected...\n") );
        /* Publish to the topic to which this task is subscribed in order
         * to receive back the data that was published. */
        xPublishParams.xQoS = eMQTTQoS0;
        xPublishParams.pucTopic = ( const uint8_t * ) pcTopic;
        xPublishParams.usTopicLength = ( uint16_t ) ( strlen( pcTopic ) );


        int ulMessageCounter = 0;
        int failureCt = 0;
        while(failureCt < 5)
        {
            getData();
            int temp = getFtemp()-10;
            int hum = getHumidity();
            configPRINTF( ( "Temperature & humidity read %iF & %i%%\n", temp, hum));
            ets_delay_us( 2000000 );

            xPublishParams.ulDataLength = ( uint32_t ) sprintf( cBuffer, ggdDEMO_MQTT_MSG_DISCOVERY, temp, hum );
            xPublishParams.pvData = cBuffer;
            xReturnCode = MQTT_AGENT_Publish( xMQTTClientHandle,
                                              &xPublishParams,
                                              xMaxCommandTime );

            if( xReturnCode != eMQTTAgentSuccess )
            {
                configPRINTF( ( "mqtt_client - Failure to publish \n" ) );
                failureCt++;
            }

            vTaskDelay( xTimeBetweenPublish );
        }

        configPRINTF( ( "Disconnecting from broker.\r\n" ) );

        if( MQTT_AGENT_Disconnect( xMQTTClientHandle,
                                   xMaxCommandTime ) == eMQTTAgentSuccess )
        {
            configPRINTF( ( "Disconnected from the broker.\r\n" ) );

            if( MQTT_AGENT_Delete( xMQTTClientHandle ) == eMQTTAgentSuccess )
            {
                configPRINTF( ( "Deleted Client.\r\n" ) );
            }
            else
            {
                configPRINTF( ( "ERROR:  MQTT_AGENT_Delete() Failed.\r\n" ) );
            }
        }
        else
        {
            configPRINTF( ( "ERROR:  Did not disconnected from the broker.\r\n" ) );
        }
    }
}

/*-----------------------------------------------------------*/

static BaseType_t prvMQTTConnect( GGD_HostAddressData_t * pxHostAddressData )
{
    MQTTAgentConnectParams_t xConnectParams;
    BaseType_t xResult = pdPASS;

    /* Connect to the broker. */
    //srand(time(NULL));   // should only be called once
    //int r = rand();
    xConnectParams.pucClientId = ( const uint8_t * ) ( clientcredentialIOT_THING_NAME );
    //xConnectParams.pucClientId = ( const uint8_t * ) ( r );
    xConnectParams.usClientIdLength = ( uint16_t ) ( strlen( clientcredentialIOT_THING_NAME ) );
    xConnectParams.pcURL = pxHostAddressData->pcHostAddress;
    xConnectParams.usPort = clientcredentialMQTT_BROKER_PORT;
    xConnectParams.xFlags = mqttagentREQUIRE_TLS | mqttagentURL_IS_IP_ADDRESS;
    xConnectParams.xURLIsIPAddress = pdTRUE; /* Deprecated. */
    xConnectParams.pcCertificate = pxHostAddressData->pcCertificate;
    xConnectParams.ulCertificateSize = pxHostAddressData->ulCertificateSize;
    xConnectParams.pvUserData = NULL;
    xConnectParams.pxCallback = NULL;
    xConnectParams.xSecuredConnection = pdTRUE; /* Deprecated. */

    if( MQTT_AGENT_Connect( xMQTTClientHandle,
                            &xConnectParams,
                            xMaxCommandTime ) != eMQTTAgentSuccess )
    {
        configPRINTF( ( "ERROR: Could not connect to the Broker.\r\n" ) );
        xResult = pdFAIL;
    }

    return xResult;
}

/*-----------------------------------------------------------*/

static void prvDiscoverGreenGrassCore( void * pvParameters )
{
    GGD_HostAddressData_t xHostAddressData;

    ( void ) pvParameters;

    /* Create MQTT Client. */
    if( MQTT_AGENT_Create( &( xMQTTClientHandle ) ) == eMQTTAgentSuccess )
    {
        memset( &xHostAddressData, 0, sizeof( xHostAddressData ) );

        /* Demonstrate automated connection. */
        configPRINTF( ( "Attempting automated selection of Greengrass device\r\n" ) );

        //keep retrying if fails.
        while ( GGD_GetGGCIPandCertificate( pcJSONFile,
                                        ggdDEMO_DISCOVERY_FILE_SIZE,
                                        &xHostAddressData )
            != pdPASS ){

            configPRINTF( ( "Auto-connect: Failed to retrieve Greengrass address and certificate.\r\n" ) );

        }

        configPRINTF( ( "Greengrass device discovered.\r\n" ) );
        configPRINTF( ( "Establishing MQTT communication to Greengrass...\r\n" ) );
        for( ; ; )
        {
            prvSendMessageToGGC( &xHostAddressData );

            /* Report on space efficiency of this demo task. */
            #if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
                {
                    configPRINTF(
                        ( "Heap low watermark: %u. Stack high watermark: %u.\r\n",
                          xPortGetMinimumEverFreeHeapSize(),
                          uxTaskGetStackHighWaterMark( NULL ) ) );
                }
            #endif
        }
    }

    vTaskDelete( NULL );
}

/*-----------------------------------------------------------*/

void vStartGreenGrassDiscoveryTask( void )
{
    //nvs_flash_init();
    //esp_sleep_enable_timer_wakeup(wakeup_time_sec  * 1000000);  // Wake up every 5 minutes
    setDHTPin(16);

    ( void ) xTaskCreate( prvDiscoverGreenGrassCore,
                          "IoT_GGD",
                          democonfigGREENGRASS_DISCOVERY_TASK_STACK_SIZE,
                          NULL,
                          democonfigGREENGRASS_DISCOVERY_TASK_PRIORITY,
                          NULL );
}
