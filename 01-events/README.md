# Overview

This is a Zephyr RTOS application that demonstrates how to handle hardware inputs, utilize kernel events, and control GPIO outputs.

## Requirements

The board hardware must have a device node capable of generating input KEY
events, typically a push button connected via a GPIO pin and defined in a
"gpio-keys" node. These are called "User buttons" on many of Zephyr's
`boards`.

The sample additionally supports an optional ``led0`` devicetree alias. If this is provided, the LED will be toggle on when the button is pressed.

## Building and Running

Build and flash Blinky as follows, changing ``rpi_pico`` for your board:

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

```
*** Booting Zephyr OS build v4.3.0 ***
I: Press the button to trigger an event
I: Button 3 pressed at 133478
I: Button 3 released at 139170
I: An event has been posted: 3
I: Button 3 pressed at 160282
I: Button 3 released at 164634
I: An event has been posted: 3
```
