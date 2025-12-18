#ifndef JSON_H
#define JSON_H

#include <zephyr/kernel.h>

// Structure to hold sensor data
struct SensorData {    
    int temperature;
    int pressure;
    int humidity;
    int gas_resistance;
};

// Function prototype
void parse_json_cb(struct k_work *work);

// External variables
extern struct k_work json_work;
extern struct SensorData sensor_data;

#endif // JSON_H
