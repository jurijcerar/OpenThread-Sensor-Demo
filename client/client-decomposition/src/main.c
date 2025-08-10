#include <zephyr/kernel.h>
#include "sensor.h"
#include "coap_client.h"

/* Timer for periodic data sending */
K_TIMER_DEFINE(mytimer, send_data_timer_cb, NULL);

int main(void) {
    // Start the periodic timer
    k_timer_start(&mytimer, K_SECONDS(16), K_SECONDS(4));

    // Initialize CoAP
    coap_init();

    // Main loop
    while (1) {
        k_sleep(K_MSEC(3000));
    }

    return 0;
}
