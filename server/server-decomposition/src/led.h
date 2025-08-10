#ifndef LED_H
#define LED_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "json.h"

// Function prototypes
void configure_leds(void);
void led_status(void);
void led_timer_cb(struct k_timer *dummy);

#define TEMP_MAX 26
#define TEMP_MIN 10
#define PRESS_MAX 100
#define PRESS_MIN 50
#define HUM_MAX 40
#define HUM_MIN 20
#define GAS_MAX 700000
#define GAS_MIN 600000


// External LED state variables
extern bool led0_enabled;
extern bool led1_enabled;
extern bool led2_enabled;
extern bool led3_enabled;

// External LED device structures
extern const struct gpio_dt_spec l0;
extern const struct gpio_dt_spec l1;
extern const struct gpio_dt_spec l2;
extern const struct gpio_dt_spec l3;

// External sensor data structure
extern struct SensorData sensor_data;

#endif // LED_H
