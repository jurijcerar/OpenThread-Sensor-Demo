#include "coap.h"
#include "crypto.h"
#include <openthread/thread.h>
#include <openthread/coap.h>
#include <zephyr/net/openthread.h>
#include <zephyr/sys/printk.h>

// Buffer for data
#define TEXTBUFFER_SIZE 256
char myText[TEXTBUFFER_SIZE];
uint16_t myText_length = 0;

// Define CoAP resource 
static otCoapResource m_storedata_resource = {
    .mUriPath = "storedata",
    .mHandler = storedata_request_cb,
    .mContext = NULL,
    .mNext = NULL
};

static void storedata_respond_send(otMessage *p_request_message, const otMessageInfo *p_message_info) {
    otError error = OT_ERROR_NO_BUFS;
    otMessage *p_response;
    otInstance *p_instance = openthread_get_default_instance();

    p_response = otCoapNewMessage(p_instance, NULL);
    if (p_response == NULL) {
        printk("Failed to allocate message for CoAP request ! \n");
        return;
    }

    do {
        error = otCoapMessageInitResponse(p_response, p_request_message, OT_COAP_TYPE_ACKNOWLEDGMENT, OT_COAP_CODE_CHANGED);
        if (error != OT_ERROR_NONE) {
            break;
        }

        error = otCoapSendResponse(p_instance, p_response, p_message_info);
    } while (false);
    
    if (error != OT_ERROR_NONE) {
        printk("Failed to send store data response: %d\n", error);
        otMessageFree(p_response);
    }
}

void storedata_request_cb(void *p_context, otMessage *p_message, const otMessageInfo *p_message_info) {
    otCoapCode messageCode = otCoapMessageGetCode(p_message);
    otCoapType messageType = otCoapMessageGetType(p_message);

    do {
        if (messageType != OT_COAP_TYPE_CONFIRMABLE && messageType != OT_COAP_TYPE_NON_CONFIRMABLE) {
            break;
        }
        if (messageCode != OT_COAP_CODE_PUT) {
            break;
        }

        myText_length = otMessageRead(p_message, otMessageGetOffset(p_message), myText, TEXTBUFFER_SIZE - 1);
        myText[myText_length] = '\0';

        uint8_t decrypted_data[256];
        size_t decrypted_data_len = sizeof(decrypted_data);

        if (decrypt_data((const uint8_t *)myText, myText_length, decrypted_data, &decrypted_data_len) != 0) {
            printk("Decryption failed\n");
            return;
        }

        // Remove padding after decryption
        size_t unpadded_len = decrypted_data_len;
        while (unpadded_len > 0 && decrypted_data[unpadded_len - 1] == 0) {
            unpadded_len--;
        }
        decrypted_data[unpadded_len] = '\0';

        strncpy(myText, (char *)decrypted_data, TEXTBUFFER_SIZE - 1);
        myText[TEXTBUFFER_SIZE - 1] = '\0';

        if (messageType == OT_COAP_TYPE_CONFIRMABLE) {
            storedata_respond_send(p_message, p_message_info);
        }
    } while (false);
}

void coap_init(void) {
    otError error = OT_ERROR_NONE;
    otInstance *p_instance = openthread_get_default_instance();
    m_storedata_resource.mContext = p_instance;

    do {
        error = otCoapStart(p_instance, OT_DEFAULT_COAP_PORT);
        if (error != OT_ERROR_NONE) {
            break;
        }

        otCoapAddResource(p_instance, &m_storedata_resource);
    } while (false);

    if (error != OT_ERROR_NONE) {
        printk("coap_init error: %d\n", error);
    }
}
