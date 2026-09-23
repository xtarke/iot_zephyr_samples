# Overview

This is a Zephyr RTOS application that demonstrates Zephyr sensor API reading a [BMP280](https://www.eletrogate.com/sensor-de-pressao-e-temperatura-bmp280?srsltid=AU7gw4VNRVENl_4NKFFIROeztobGaFp2kz593M19T2t9RbKYeX0n-VID) sensor using I2C interface.

## Requirements

The board hardware must have a device node bmp280 in the i2c node. A devicetree ```bmp280``` must be defined in the [board overlay](./boards/rpi_pico_w.overlay).

## Building and Running

Build and flash as follows, changing ``rpi_pico`` for your board:

```bash
# Prestine build with Pico W board 
west build -b rpi_pico/rp2040/w -p
```

To flash using BlackMagicProbe:

```bash
# Flashing the device using BMP debugger 
west flash --runner blackmagicprobe --gdb-serial /dev/ttyACM0
```

Tip: the first flash command probably will fail. See this BlackMagick Probe [bug report](https://github.com/blackmagic-debug/blackmagic/issues/1985).


## Hardware pin connection

| Module Pin | Pico Board pin |
| :---       | :---           |
| VCC        | 3V3            |
| GND        | GND            |
| SDA        | GP4            |
| SCL        | GP5            |


## Sample console interaction

```
*** Booting Zephyr OS build v4.4.0 ***
I: Sample using device BMP280 starting.
I: T: 19.550000; P: 101.927207
I: T: 19.540000; P: 101.926074
I: T: 19.530000; P: 101.927113
I: T: 19.520000; P: 101.926484
I: T: 19.520000; P: 101.926984
```
