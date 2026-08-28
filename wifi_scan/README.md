# Overview

This sample demonstrates concurrent WiFi and LED operation on the
Raspberry Pi Pico W and Pico 2 W. A background thread blinks the
onboard LED via the CYW43 GPIO driver while the main thread performs
a WiFi scan.

## Building and Running

Verify the board and chip you are targeting provide Wi-Fi support.

For instance you can use RP-Pico 2 by selecting the rpi_pico/rp2040/w board.

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

To see avaliable Wifi Networks:

```
*** Booting Zephyr OS build v4.4.0 ***
I: Pico W WiFi + LED Demo

I: LED blinking started

Starting WiFi scan...
I:   SSID: XLAN_2.4G                        CH:  1 RSSI: -50
I:   SSID: CARLSEM LTDA                     CH:  3 RSSI: -89
I:   SSID: CARLSEM LTDA                     CH:  3 RSSI: -90
I:   SSID: Carlsem.JC                       CH:  3 RSSI: -89
I:   SSID:                                  CH:  0 RSSI: 0
I:   SSID: Starlink Nobel                   CH:  6 RSSI: -80
I:   SSID: Daiane                           CH:  6 RSSI: -89
I:   SSID: APTO_506                         CH:  8 RSSI: -64
I:   SSID: #CLARO-WIFI                      CH: 11 RSSI: -91
I: WiFi scan complete

I: Demo complete - LED continues blinking
```

