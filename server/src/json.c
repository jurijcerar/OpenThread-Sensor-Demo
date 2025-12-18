#include "json.h"
#include <zephyr/data/json.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define TEXTBUFFER_SIZE 256

extern char myText[TEXTBUFFER_SIZE];

// Declare sensor_data as external
struct SensorData sensor_data;

static const struct json_obj_descr sensor_descr[] = {
    JSON_OBJ_DESCR_PRIM(struct SensorData, temperature, JSON_TOK_NUMBER),
    JSON_OBJ_DESCR_PRIM(struct SensorData, pressure, JSON_TOK_NUMBER),
    JSON_OBJ_DESCR_PRIM(struct SensorData, humidity, JSON_TOK_NUMBER),
    JSON_OBJ_DESCR_PRIM(struct SensorData, gas_resistance, JSON_TOK_NUMBER),
};

void parse_json_cb(struct k_work *work) {
    int ret = json_obj_parse(myText, strlen(myText), sensor_descr, ARRAY_SIZE(sensor_descr), &sensor_data);
    if (ret < 0) {
        printk("JSON Parse Error: %d\n", ret);
    } else {
        printk("temperature: %d\n", sensor_data.temperature);
        printk("pressure: %d\n", sensor_data.pressure);
        printk("humidity: %d\n", sensor_data.humidity);
        printk("gas_resistance: %d\n", sensor_data.gas_resistance);
    }
}

K_WORK_DEFINE(json_work, parse_json_cb);
