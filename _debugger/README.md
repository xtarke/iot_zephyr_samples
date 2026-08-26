# Black Magic Probe on STM32 Black Pill

This guide provides straightforward instructions to compile and flash the Black Magic Probe (BMP) firmware onto an STM32 Black Pill development board (typically STM32F401 or STM32F411).

See [this guide](https://github.com/blackmagic-debug/blackmagic/blob/main/src/platforms/common/blackpill-f4/README.md) for more information.

## Prerequisites

Ensure you have the following installed on your system:
* ARM GCC toolchain (`arm-none-eabi-gcc`)
* `make`
* `git`
* `dfu-util` (for flashing via USB DFU mode)
* `meson`

---

## 1. Clone the Repository

Clone the official Black Magic Probe repository and fetch the required submodules (such as libopencm3):

```bash
git clone https://codeberg.org/blackmagic-debug/blackmagic.git
cd blackmagic
git submodule update --init
```

## 2. Compile the Firmware

Run `meson` and set the `--cross-file` to `blackpill-f401cc.ini` (Check chip PN). This instructs the build system to use the correct linker script and hardware definitions for the Black Pill board.

```bash
meson setup build --cross-file=cross-file/blackpill-f401cc.ini -Dbmd_bootloader=true
```

Compile the firmware and bootloader.

```bash
ninja -C build
ninja -C build boot-bin
```

*Note: The build process will generate the necessary ELF and BIN files in the `build` directory.*

## 3. Flash the Firmware (via DFU)

The simplest way to flash the board without an external programmer is by using the STM32's built-in USB DFU bootloader.

### Enter DFU Mode:
1. Connect the Black Pill to your computer via USB.
2. Press and **hold** the `BOOT0` button.
3. Press and **release** the `NRST` (Reset) button.
4. **Release** the `BOOT0` button.

You can verify the board is in DFU mode by running `dfu-util -l`. You should see an STMicroelectronics device with the ID `0483:df11`.

### Flash the bootloader

```bash
dfu-util -d 0483:df11 --alt 0 -s 0x08000000:leave -D build/blackmagic_blackpill_f401cc_bootloader.bin
```

### Flash the firmware:

From the root of the `blackmagic` directory, flash the newly compiled `blackmagic.bin` to the flash memory address `0x08000000`:

```bash
./dfu-util -d 0483:df11 --alt 0 -s 0x08004000:leave -D build/blackmagic_blackpill_f401cc_firmware.bin
```

### Upgrade the firmware

```bash
dfu-util -d 1d50:6018 -a 0 -s 0x08004000:leave -D build/blackmagic_blackpill_f401cc_firmware.bin
``` 

## 4. Usage

After the flash completes and the board resets (or after you manually reconnect the USB), the Black Pill will enumerate as a composite USB device exposing two virtual COM ports (CDC ACM):

1. **GDB Server:** Used to connect your debugger (e.g., `target extended-remote /dev/ttyACM0`).

2. **UART Bridge:** Used as a standard USB-to-serial TTL adapter for target communication (e.g., `/dev/ttyACM1`).


## Pinout

| Function        | Pinout | Cluster   |
| --------------- | ------ | --------- |
| TDI             | PB6    | JTAG/SWD  |
| TDO/TRACESWO    | PB7    | JTAG/SWD  |
| TCK/SWCLK       | PB8    | JTAG/SWD  |
| TMS/SWDIO       | PB9    | JTAG/SWD  |
| nRST            | PA5    | JTAG/SWD  |
| TRST (optional) | PA6    | JTAG/SWD  |
| UART TX         | PA2    | USB USART |
| UART RX         | PA3    | USB USART |
| Power pin       | PA1    | Power     |
| LED idle run    | PC13   | LED       |
| LED error       | PC14   | LED       |
| LED bootloader  | PC15   | LED       |
| LED UART        | PA4    | LED       |
| User button KEY | PA0    |           |
	