#ifndef SENSOR_H
#define SENSOR_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>

// Structure to hold sensor data
struct SensorData {
    struct sensor_value temp;
    struct sensor_value press;
    struct sensor_value humidity;
    struct sensor_value gas_res;
};

// Function prototypes
void sensor_work_cb(struct k_work *work);

// External variables
extern struct k_work sensor_work;
extern struct SensorData sensor_data;

#endif // SENSOR_H
