#ifndef BUTTON_H
#define BUTTON_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>

struct gpio_callback; // Forward declaration

// Function prototypes
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void configure_buttons(void);

#endif // BUTTON_H
