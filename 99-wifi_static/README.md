# Overview

This sample allows testing Wi-Fi drivers for various boards witi static credentials.

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


## Sample console interaction

After boot, we can see the following console output:

```
*** Booting Zephyr OS build v4.4.0 ***
I: RP Pico W static wifi sample.
I: Initializing Wi-Fi driver
I: Received: 192.168.0.169
```

Simetimes the board could connect in firsts attemps:

```
*** Booting Zephyr OS build v4.4.0 ***
I: RP Pico W static wifi sample.
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