# Overview

The Blinky sample blinks an LED forever using the Zephyr GPIO API.

The source code shows how to:

- Get a pin specification from the __devicetree gpio_dt_spec__
- Configure the GPIO pin as an output
- Toggle the pin forever


## Requirements

Your board must:

- Have an LED connected via a GPIO pin (these are called "User LEDs" on many of Zephyr's boards).
- Have the LED configured using the ``led0`` devicetree alias.

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

After flashing, the LED starts to blink and messages with the current LED state are printed on the console. If a runtime error occurs, the sample exits without printing to the console.

## Build errors


You will see a build error at the source code line defining the ``struct
gpio_dt_spec led`` variable if you try to build Blinky for an unsupported
board.

On GCC-based toolchains, the error looks like this:

```
error: '__device_dts_ord_DT_N_ALIAS_led_P_gpios_IDX_0_PH_ORD' undeclared here (not in a function)
```

## Adding board support

To add support for your board, add something like this to your devicetree:

```dts
/{
   aliases {
      led0 = &myled0;
   };

   leds {
      compatible = "gpio-leds";
      myled0: led_0 {
         gpios = <&gpio0 13 GPIO_ACTIVE_LOW>;
      };
   };
};
```

The above sets your board's ``led0`` alias to use pin 13 on GPIO controller
``gpio0``. The pin flags :c:macro:`GPIO_ACTIVE_LOW` mean the LED is on when
the pin is set to its low state, and off when the pin is in its high state.

Tips:

- See :dtcompatible:`gpio-leds` for more information on defining GPIO-based LEDs   in devicetree.

- If you're not sure what to do, check the devicetrees for supported boards which   use the same SoC as your target.

- See zephyr_file: __include/zephyr/dt-bindings/gpio/gpio.h__ for the flags you can use in devicetree.

- If the LED is built in to your board hardware, the alias should be defined in your BOARD.dts file. Otherwise, you can define one in a __devicetree overlay__.
