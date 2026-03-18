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

#define SLEEP_TIME_MS 1000

/* Timer drives LED status updates at 1 s intervals */
K_TIMER_DEFINE(mytimer, led_timer_cb, NULL);

static void addIPv6Address(void);

void main(void)
{
    /* Configure peripherals */
    configure_leds();
    configure_buttons();

    /* Initialize CoAP before starting the timer */
    coap_init();

    /* Add a static mesh-local IPv6 address for CoAP reachability */
    addIPv6Address();

    /* Start periodic LED status timer (initial delay 10 s, period 1 s) */
    k_timer_start(&mytimer, K_SECONDS(10), K_SECONDS(1));

    /* Main loop */
    while (1) {
        k_msleep(SLEEP_TIME_MS);
    }
}

static void addIPv6Address(void)
{
    otInstance *Myinstance = openthread_get_default_instance();
    if (Myinstance == NULL) {
        printk("Failed to get OpenThread instance\n");
        return;
    }

    const otMeshLocalPrefix *ml_prefix = otThreadGetMeshLocalPrefix(Myinstance);
    if (ml_prefix == NULL) {
        printk("Failed to get Mesh Local Prefix\n");
        return;
    }

    otNetifAddress aAddress;
    uint8_t interfaceID[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

    memcpy(&aAddress.mAddress.mFields.m8[0], ml_prefix, 8);
    memcpy(&aAddress.mAddress.mFields.m8[8], interfaceID, 8);

    aAddress.mPrefixLength = 64;
    aAddress.mPreferred    = true;
    aAddress.mValid        = true;

    otError error = otIp6AddUnicastAddress(Myinstance, &aAddress);
    if (error != OT_ERROR_NONE) {
        printk("addIPv6Address error: %d\n", error);
    } else {
        printk("IPv6 address added successfully\n");
    }
}
