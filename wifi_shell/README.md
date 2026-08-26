# Overview

This sample allows testing Wi-Fi drivers for various boards by
enabling the Wi-Fi shell module that provides a set of commands:
scan, connect, and disconnect.  It also enables the net_shell module
to verify net_if settings.

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

To scan avaliable Wifi Networks:

```
uart:~$ wifi scan

wifi scan
Scan requested

Num  | SSID                             (len) | Chan (Band)   | RSSI | Security             | BSSID             | MFP     
1    | XLAN_2.4G                        9     | 1    (2.4GHz) | -47  | WPA/WPA2/WPA3 PSK    | 92:5E:44:C6:AD:1C | Disable 
2    | APTO_506                         8     | 9    (2.4GHz) | -63  | WPA2-PSK             | C4:EB:FF:E5:B9:FB | Disable 
3    | Valmor 2.4G                      11    | 11   (2.4GHz) | -89  | WPA2-PSK             | 84:0B:BB:4F:14:F0 | Disable 
4    | XLAN_2.4G                        9     | 1    (2.4GHz) | -46  | WPA/WPA2/WPA3 PSK    | 92:5E:44:C6:AD:1C | Disable 
Scan request done
```
To connect:

```
uart:~$ wifi connect -s "lpae_wifi"  -p "my wifi password" -k 1
Connected
Connection requested
[00:00:49.819,000] <inf> net_dhcpv4: Received: 192.168.0.169
