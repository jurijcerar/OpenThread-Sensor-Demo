#include "sensor.h"
#include "coap_client.h"
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>

struct SensorData sensor_data;

void sensor_work_cb(struct k_work *work)
{
    const struct device *const dev = DEVICE_DT_GET_ONE(bosch_bme680);

    if (!device_is_ready(dev)) {
        printk("sensor: device not ready.\n");
        return;
    }

    sensor_sample_fetch(dev);
    sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &sensor_data.temp);
    sensor_channel_get(dev, SENSOR_CHAN_PRESS,         &sensor_data.press);
    sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY,      &sensor_data.humidity);
    sensor_channel_get(dev, SENSOR_CHAN_GAS_RES,       &sensor_data.gas_res);

    /* Send data immediately after collection so CoAP always uses fresh values */
    coap_send_data_request();
}

K_WORK_DEFINE(sensor_work, sensor_work_cb);
