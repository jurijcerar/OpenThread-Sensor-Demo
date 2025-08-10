#ifndef COAP_H
#define COAP_H

#include <openthread/thread.h>

// Function prototypes
void storedata_request_cb(void *p_context, otMessage *p_message, const otMessageInfo *p_message_info);
void coap_init(void);

#endif // COAP_H
