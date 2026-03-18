# OpenThread Sensor Demo

Built with [Zephyr RTOS](https://zephyrproject.org/) and [OpenThread](https://openthread.io/). Two nRF52840-DK boards communicate over a Thread mesh network using CoAP. The client board reads environmental data from a Bosch BME680 sensor, encrypts it with AES, and transmits it to the server board, which decrypts the data and uses the onboard LEDs to indicate whether sensor values are within configured thresholds.


## Hardware Requirements

- 2× [nRF52840-DK](https://www.nordicsemi.com/Products/Development-hardware/nrf52840-dk) (or one nRF52840-DK + one [Adafruit Feather nRF52840](https://www.adafruit.com/product/4062) for the client)
- [Bosch BME680](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme680/) environmental sensor (connected to the client board via I2C)

---

## How It Works

### Client
1. A periodic timer fires every 4 seconds (with a 16-second initial delay to allow the Thread network to join).
2. Temperature, pressure, humidity, and gas resistance are read from the BME680 sensor.
3. The data is serialised into JSON, zero-padded to the AES block boundary, and encrypted with AES-ECB using TinyCrypt.
4. The encrypted payload is sent to the server via a CoAP PUT request to the `storedata` resource.

### Server
1. The CoAP server listens on the `storedata` resource.
2. On receiving a request, it decrypts the payload, strips zero-padding, and parses the JSON.
3. A 1-second timer drives the LED status display based on the latest parsed values.
4. Each LED corresponds to one sensor channel. LEDs **stay on** when the value is within threshold, and **blink** when it is out of range.

### LED / Button Mapping

| Button | LED | Sensor channel   | Min   | Max    |
|--------|-----|------------------|-------|--------|
| 0      | 0   | Temperature (°C) | 10    | 26     |
| 1      | 1   | Pressure (kPa)   | 50    | 100    |
| 2      | 2   | Humidity (%)     | 20    | 40     |
| 3      | 3   | Gas resistance   | 600 kΩ | 700 kΩ |

LEDs are off by default. Press the corresponding button to enable or disable monitoring for each channel.

---

## Configuration

### Network credentials
Before building, update the Thread network parameters in **both** `prj.conf` files. The default values are placeholders and must not be used as-is:

```conf
CONFIG_OPENTHREAD_NETWORK_NAME="your-network-name"
CONFIG_OPENTHREAD_PANID=0xABCD
CONFIG_OPENTHREAD_XPANID="00:11:22:33:44:55:66:77"
CONFIG_OPENTHREAD_NETWORKKEY="00:11:22:33:44:55:66:77:88:99:AA:BB:CC:DD:EE:FF"
```

Both boards must use identical values to join the same Thread network.

### AES key
The encryption key is hardcoded in `src/crypto.c` on both client and server and must match:

```c
uint8_t aes_key[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
```

> **Note:** This is an example key. Replace it with a securely generated value before any real deployment.

---

## Building and Flashing

Requires a working [Zephyr SDK and west](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) installation.

**Server** (nRF52840-DK):
```bash
cd server
west build -b nrf52840dk/nrf52840
west flash
```

**Client** (nRF52840-DK):
```bash
cd client
west build -b nrf52840dk/nrf52840
west flash
```

**Client** (Adafruit Feather nRF52840):
```bash
cd client
west build -b adafruit_feather_nrf52840
west flash
```

---

## Known Limitations

- **AES-ECB mode** is used instead of CBC due to challenges managing the initialisation vector with TinyCrypt. ECB is sufficient for this academic project but should not be used in production.
- **Integer-only sensor values** — Zephyr's JSON library does not support floating-point serialisation, so only the integer part (`val1`) of each `sensor_value` is transmitted.
- **CoAP over DTLS** was not implemented due to configuration issues with the mbedTLS integration in `prj.conf`.

---

## Dependencies

| Component    | Version / Source                          |
|--------------|-------------------------------------------|
| Zephyr RTOS  | https://github.com/zephyrproject-rtos/zephyr |
| OpenThread   | Bundled with Zephyr                       |
| TinyCrypt    | Bundled with Zephyr (`CONFIG_TINYCRYPT=y`) |
| BME680 driver| Bundled with Zephyr (`bosch,bme680`)      |
