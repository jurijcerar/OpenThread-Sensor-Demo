#include "button.h"
#include "led.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

// Define Button devices
static const struct gpio_dt_spec b0 = GPIO_DT_SPEC_GET(DT_ALIAS(button0), gpios);
static const struct gpio_dt_spec b1 = GPIO_DT_SPEC_GET(DT_ALIAS(button1), gpios);
static const struct gpio_dt_spec b2 = GPIO_DT_SPEC_GET(DT_ALIAS(button2), gpios);
static const struct gpio_dt_spec b3 = GPIO_DT_SPEC_GET(DT_ALIAS(button3), gpios);

static struct gpio_callback button_cb_data;

extern bool led0_enabled, led1_enabled, led2_enabled, led3_enabled;
extern const struct gpio_dt_spec l0, l1, l2, l3;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    if (pins & BIT(b0.pin)) {
        led0_enabled = !led0_enabled;
        gpio_pin_set_dt(&l0, led0_enabled);
    }
    if (pins & BIT(b1.pin)) {
        led1_enabled = !led1_enabled;
        gpio_pin_set_dt(&l1, led1_enabled);
    }
    if (pins & BIT(b2.pin)) {
        led2_enabled = !led2_enabled;
        gpio_pin_set_dt(&l2, led2_enabled);
    }
    if (pins & BIT(b3.pin)) {
        led3_enabled = !led3_enabled;
        gpio_pin_set_dt(&l3, led3_enabled);
    }
}

void configure_buttons(void) {
    gpio_pin_configure_dt(&b0, GPIO_INPUT | GPIO_PULL_UP);
    gpio_pin_configure_dt(&b1, GPIO_INPUT | GPIO_PULL_UP);
    gpio_pin_configure_dt(&b2, GPIO_INPUT | GPIO_PULL_UP);
    gpio_pin_configure_dt(&b3, GPIO_INPUT | GPIO_PULL_UP);

    gpio_init_callback(&button_cb_data, button_pressed, BIT(b0.pin) | BIT(b1.pin) | BIT(b2.pin) | BIT(b3.pin));
    gpio_add_callback(b0.port, &button_cb_data);
    gpio_add_callback(b1.port, &button_cb_data);
    gpio_add_callback(b2.port, &button_cb_data);
    gpio_add_callback(b3.port, &button_cb_data);

    gpio_pin_interrupt_configure_dt(&b0, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure_dt(&b1, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure_dt(&b2, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure_dt(&b3, GPIO_INT_EDGE_TO_ACTIVE);
}
