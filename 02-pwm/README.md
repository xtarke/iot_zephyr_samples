# Overview

This is a Zephyr RTOS application that demonstrates the initializing of a PWM device setting it to a 50% duty cycle using a predefined period.

## Requirements

The board hardware must have a device node capable of generating PWM. A devicetree pwm_led0 must be defined in the board overlay.

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
I: PWM sample started, T = 1000000 ns
```

When capturing the signal using an oscilloscope or logic analyzer, it should look like the following repeating sequence.

```
     ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
    ─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─
```
