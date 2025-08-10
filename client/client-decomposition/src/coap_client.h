#ifndef COAP_CLIENT_H
#define COAP_CLIENT_H

#include <zephyr/kernel.h>
#include <openthread/message.h>
#include <openthread/ip6.h>

// Function prototypes
void coap_send_data_request(void);
void coap_send_data_response_cb(void *p_context, otMessage *p_message, const otMessageInfo *p_message_info, otError result);
void coap_init(void);
void send_data_timer_cb(struct k_timer *dummy);

#endif // COAP_CLIENT_H
