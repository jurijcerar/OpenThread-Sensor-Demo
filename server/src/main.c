#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/openthread.h>
#include <zephyr/sys/printk.h>
#include "led.h"
#include "button.h"
#include "coap.h"
#include "crypto.h"
#include "json.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

K_TIMER_DEFINE(mytimer, led_timer_cb, NULL);

void addIPv6Address(void);

void main(void) {
    // Default led configuration
    configure_leds();

    // Default button configuration
    configure_buttons();

    // Start the periodic timer
    k_timer_start(&mytimer, K_SECONDS(10), K_SECONDS(1));

    addIPv6Address();

    // Initialize CoAP
    coap_init();

    // Main loop
    while (1) {
        k_msleep(SLEEP_TIME_MS);
    }
}

void addIPv6Address(void) {
    otInstance *Myinstance = openthread_get_default_instance();
    if (Myinstance == NULL) {
        printk("Failed to get OpenThread instance\n");
        return;
    }
    
    otNetifAddress aAddress;
    const otMeshLocalPrefix *ml_prefix = otThreadGetMeshLocalPrefix(Myinstance);
    if (ml_prefix == NULL) {
        printk("Failed to get Mesh Local Prefix\n");
        return;
    }

    uint8_t interfaceID[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

    memcpy(&aAddress.mAddress.mFields.m8[0], ml_prefix, 8);
    memcpy(&aAddress.mAddress.mFields.m8[8], interfaceID, 8);

    aAddress.mPrefixLength = 64;
    aAddress.mPreferred = true;
    aAddress.mValid = true;

    otError error = otIp6AddUnicastAddress(Myinstance, &aAddress);
    if (error != OT_ERROR_NONE) {
        printk("addIPAddress error : %d\n", error);
    } else {
        printk("IPv6 address added successfully\n");
    }
}
