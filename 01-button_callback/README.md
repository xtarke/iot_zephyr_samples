# Overview


A simple button demo showcasing the use of buttons with the `input` APIs.
The sample prints a message to the console each time a button is pressed.

## Requirements


The board hardware must have a device node capable of generating input KEY
events, typically a push button connected via a GPIO pin and defined in a
"gpio-keys" node. These are called "User buttons" on many of Zephyr's
`boards`.

The sample additionally supports an optional ``led0`` devicetree alias. If this is provided, the LED will be turned on when the button is pressed, and turned off when it is released.

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
I: Press the button

I: Button 3 pressed at 143470
I: Button 3 released at 147423
```
