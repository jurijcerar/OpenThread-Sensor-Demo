#include "coap_client.h"
#include <openthread/coap.h>
#include <openthread/thread.h>
#include <zephyr/net/openthread.h>
#include <tinycrypt/aes.h>
#include <tinycrypt/constants.h>
#include <zephyr/sys/printk.h>
#include "sensor.h"
#include "crypto.h"

#define AES_KEY_SIZE 16
#define AES_BLOCK_SIZE 16

void coap_send_data_response_cb(void *p_context, otMessage *p_message, const otMessageInfo *p_message_info, otError result) {
    if (result == OT_ERROR_NONE) {
        printk("Delivery confirmed!\n");
    } else {
        printk("Delivery error: %d\n", result);
    }
}

void coap_send_data_request(void) {
    otError error = OT_ERROR_NONE;
    otMessage *myMessage;
    otInstance *myInstance = openthread_get_default_instance();
    otMessageInfo myMessageInfo;

    const otMeshLocalPrefix *ml_prefix = otThreadGetMeshLocalPrefix(myInstance);
    uint8_t serverInterfaceID[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

    char mySensorDataJson[120];
    snprintf(mySensorDataJson, sizeof(mySensorDataJson),
             "{\"temperature\": %d, \"pressure\": %d, \"humidity\": %d, \"gas_resistance\": %d}",
             sensor_data.temp.val1, sensor_data.press.val1, sensor_data.humidity.val1, sensor_data.gas_res.val1);

    printk("Original data: %s\n", mySensorDataJson);

    // Padding the data to the AES block size
    size_t input_len = strlen(mySensorDataJson);
    size_t padded_len = ((input_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    uint8_t padded_data[padded_len];
    memcpy(padded_data, mySensorDataJson, input_len);
    memset(padded_data + input_len, 0, padded_len - input_len); // Zero-padding

    // Encrypt the data
    uint8_t encrypted_data[padded_len];
    if (encrypt_data(padded_data, padded_len, encrypted_data) != 0) {
        printk("Encryption failed!\n");
        return;
    }

    // Sending the encrypted data
    do {
        myMessage = otCoapNewMessage(myInstance, NULL);
        if (myMessage == NULL) {
            printk("Failed to allocate message for CoAP request!\n");
            return;
        }

        otCoapMessageInit(myMessage, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_PUT);

        error = otCoapMessageAppendUriPathOptions(myMessage, "storedata");
        if (error != OT_ERROR_NONE) {
            break;
        }

        error = otCoapMessageAppendContentFormatOption(myMessage, OT_COAP_OPTION_CONTENT_FORMAT_JSON);
        if (error != OT_ERROR_NONE) {
            break;
        }

        error = otCoapMessageSetPayloadMarker(myMessage);
        if (error != OT_ERROR_NONE) {
            break;
        }

        error = otMessageAppend(myMessage, encrypted_data, padded_len);
        if (error != OT_ERROR_NONE) {
            break;
        }

        memset(&myMessageInfo, 0, sizeof(myMessageInfo));
        memcpy(&myMessageInfo.mPeerAddr.mFields.m8[0], ml_prefix, 8);
        memcpy(&myMessageInfo.mPeerAddr.mFields.m8[8], serverInterfaceID, 8);
        myMessageInfo.mPeerPort = OT_DEFAULT_COAP_PORT;

        error = otCoapSendRequest(myInstance, myMessage, &myMessageInfo, coap_send_data_response_cb, NULL);

    } while (false);

    if (error == OT_ERROR_NONE) {
        printk("Coap data sent\n");
    } else {
        printk("Coap data send error: %d\n", error);
    }
}

void coap_init(void) {
    otInstance *p_instance = openthread_get_default_instance();
    otError error = otCoapStart(p_instance, OT_DEFAULT_COAP_PORT);
    if (error != OT_ERROR_NONE) {
        printk("Coap Init error: %d\n", error);
    }
}

void send_data_timer_cb(struct k_timer *dummy) {
    k_work_submit(&sensor_work);
    coap_send_data_request();
}
