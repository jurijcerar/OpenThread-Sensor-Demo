#include "led.h"
#include "json.h"
#include <zephyr/drivers/gpio.h>

bool led0_enabled = false;
bool led1_enabled = false;
bool led2_enabled = false;
bool led3_enabled = false;

// Initializing the internal leds
const struct gpio_dt_spec l0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
const struct gpio_dt_spec l1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
const struct gpio_dt_spec l2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
const struct gpio_dt_spec l3 = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);

extern struct SensorData sensor_data;

void configure_leds(void) {
    gpio_pin_configure_dt(&l0, GPIO_OUTPUT);
    gpio_pin_configure_dt(&l1, GPIO_OUTPUT);
    gpio_pin_configure_dt(&l2, GPIO_OUTPUT);
    gpio_pin_configure_dt(&l3, GPIO_OUTPUT);
}

void led_status(void) {
    if (led0_enabled) {
        if(sensor_data.temperature > TEMP_MAX || sensor_data.temperature < TEMP_MIN) {
            gpio_pin_toggle_dt(&l0);
        } else {
            gpio_pin_set_dt(&l0, 1);
        }
    }
    if (led1_enabled) {
        if(sensor_data.pressure > PRESS_MAX || sensor_data.pressure < PRESS_MIN) {
            gpio_pin_toggle_dt(&l1);
        } else {
            gpio_pin_set_dt(&l1, 1);
        }
    }
    if (led2_enabled) {
        if(sensor_data.humidity > HUM_MAX || sensor_data.humidity < HUM_MIN) {
            gpio_pin_toggle_dt(&l2);
        } else {
            gpio_pin_set_dt(&l2, 1);
        }
    }
    if (led3_enabled) {
        if(sensor_data.gas_resistance > GAS_MAX || sensor_data.gas_resistance < GAS_MIN) {
            gpio_pin_toggle_dt(&l3);
        } else {
            gpio_pin_set_dt(&l3, 1);
        }
    }
}

void led_timer_cb(struct k_timer *dummy){ 
    k_work_submit(&json_work);
    led_status();    
}
