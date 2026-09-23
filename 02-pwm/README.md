# Overview

This is a Zephyr RTOS application that demonstrates the initializing of a PWM device setting it to a 50% duty cycle using a predefined period.

## Requirements

The board hardware must have a device node capable of generating PWM. A devicetree ```pwm_dev``` must be defined in the [board overlay](./boards/rpi_pico_w.overlay).

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

## Pin and channel map

| GPIO Pin | Slice PWM | Zephyr Macro (`pinctrl`) | DTS channel (`pwms`) |
| :--- | :--- | :--- | :--- |
| **GP0 / GP1** | Slice 0 (A/B) | `PWM_0A_P0` / `PWM_0B_P1` | 0 / 1 |
| **GP2 / GP3** | Slice 1 (A/B) | `PWM_1A_P2` / `PWM_1B_P3` | 2 / 3 |
| **GP4 / GP5** | Slice 2 (A/B) | `PWM_2A_P4` / `PWM_2B_P5` | 4 / 5 |
| **GP6 / GP7** | Slice 3 (A/B) | `PWM_3A_P6` / `PWM_3B_P7` | 6 / 7 |
| **GP8 / GP9** | Slice 4 (A/B) | `PWM_4A_P8` / `PWM_4B_P9` | 8 / 9 |
| **GP10 / GP11** | Slice 5 (A/B) | `PWM_5A_P10` / `PWM_5B_P11` | 10 / 11 |
| **GP12 / GP13** | Slice 6 (A/B) | `PWM_6A_P12` / `PWM_6B_P13` | 12 / 13 |
| **GP14 / GP15** | Slice 7 (A/B) | `PWM_7A_P14` / `PWM_7B_P15` | 14 / 15 |
| **GP16 / GP17** | Slice 0 (A/B) | `PWM_0A_P16` / `PWM_0B_P17` | 0 / 1 |
| **GP18 / GP19** | Slice 1 (A/B) | `PWM_1A_P18` / `PWM_1B_P19` | 2 / 3 |
| **GP20 / GP21** | Slice 2 (A/B) | `PWM_2A_P20` / `PWM_2B_P21` | 4 / 5 |
| **GP22 / GP23** | Slice 3 (A/B) | `PWM_3A_P22` / `PWM_3B_P23` | 6 / 7 |
| **GP24 / GP25** | Slice 4 (A/B) | `PWM_4A_P24` / `PWM_4B_P25` | 8 / 9 |
| **GP26 / GP27** | Slice 5 (A/B) | `PWM_5A_P26` / `PWM_5B_P27` | 10 / 11 |
| **GP28 / GP29** | Slice 6 (A/B) | `PWM_6A_P28` / `PWM_6B_P29` | 12 / 13 |

---

## Hardware rules


1. **Channel index in devicetree**:

    - The channel parameter in the `pwms = <&pwm CHANNEL ...>` property follows the pattern `GPIO % 16`.
    - Example: Both **GP9** and **GP25** use channel `9`.

2. **Shared frequency**:
    - Pins associated with the same Slice (example: GP0 and GP1, or GP0 ​​and GP16) share the same frequency and clock divider.
    - The duty cycle can be adjusted individually for output A and output B of the same Slice.

3. **Restrictions on the Raspberry Pi Pico board**:

    - **GP25**: Connected to the integrated LED (*onboard*) for Pico and WiFI CS for Pico W.
    - **GP23, GP24, GP29**: Used internally in the power supply and VSYS reading circuit (not recommended for external use).





