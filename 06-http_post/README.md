# Overview

This sample allows testing Wi-Fi drivers for various boards witi static credentials.

## Prerequisites for the Pico W

Building for the Raspberry Pi Pico W requires the AIROC binary blobs provided by Infineon. Run the command below to retrieve those files:

```bash
west blobs fetch hal_infineon
```

## Prerequisites for the ESP32

Building for the ESP32 requires Espressif HAL requires RF binary blobs. Run the command below to retrieve those files:

```bash
west blobs fetch hal_espressif
```

## Building and Running

Verify the board and chip you are targeting provide Wi-Fi support.

For instance you can use RP-Pico W by selecting the rpi_pico/rp2040/w board.

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

For esp32s3-dev-kit:

```bash
west build -b esp32s3_devkitc/esp32s3/procpu -p
```

To flash using internal debug probe (USB-C):

```bash
west flash --esp-device /dev/ttyACM0
```

To open espressif monitor:

```bash
west espressif monitor -p /dev/ttyACM0
```

Tip: look into esp32s3 overlay and change "chosen node" if you want to use uart0 for log messages.

## Sample console interaction

After boot, we can see the following console output:

```
*** Booting Zephyr OS build v4.4.0 ***
I: TagoIO post sample.
I: Initializing Wi-Fi driver
I: Connecting to WiFi...
I: Waiting to connect!
I: Received: 192.168.0.185
I: Network connected
I: Posting data.
I: Post successful...
I: Posting data.
I: Post successful...
I: Posting data.
I: Post successful...
```

Simetimes the board could connect in firsts attemps:

```
*** Booting Zephyr OS build v4.4.0 ***
I: TagoIO post sample.
I: Initializing Wi-Fi driver

[13706] Function whd_wifi_join failed at line 2179 checkres = 33555438 
E: Failed to connect with network
E: Connecting to Wi-Fi failed, err: -11
E: New try in 5s.

[28553] Function whd_wifi_join failed at line 2179 checkres = 33555457 
E: Failed to connect with network
E: Connecting to Wi-Fi failed, err: -11
E: New try in 5s.
I: Received: 192.168.0.169
```