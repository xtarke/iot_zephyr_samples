# Overview

This sample demonstrates how to use the ```ADC driver API <adc_api>```.

Depending on the target board, it reads ADC samples from one or more channels and prints the readings on the console. If voltage of the used reference can be obtained, the raw readings are converted to millivolts.

The pins of the ADC channels are board-specific. Please refer to the board or MCU datasheet for further details. For RP Pico W board, see the table below.

| GPIO Pin | ADC Channel 
| :--- | :--- | 
| GP26 | ADC0 | 
| GP27 | ADC1 | 
| GP28 | ADC2 | 


# Building and Running

The ADC peripheral and pinmux is configured in the board's ``.dts`` file. Make
sure that the ADC is enabled (``status = "okay";``).

In addition to that, this sample requires an ADC channel specified in the
``io-channels`` property of the ``zephyr,user`` node. This is usually done with
a devicetree overlay. 

Configuration of channels (settings like gain, reference, or acquisition time)
also needs to be specified in devicetree, in ADC controller child nodes. Also
the ADC resolution and oversampling setting (if used) need to be specified
there. 

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


## Sample console
=============

```
*** Booting Zephyr OS build v4.1.0 ***
I: ADC reading[0]:
I: 	adc@4004c000, channel 0: 
I: 	Raw: 283
I: 	Volts: 228 mV

I: ADC reading[1]:
I: 	adc@4004c000, channel 0: 
I: 	Raw: 819
I: 	Volts: 659 mV

I: ADC reading[2]:
I: 	adc@4004c000, channel 0: 
I: 	Raw: 899
I: 	Volts: 724 mV
```
