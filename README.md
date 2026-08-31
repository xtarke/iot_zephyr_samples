# Zephyr RTOS Examples for Raspberry Pi Pico W

This repository contains a collection of IoT examples and utilities built with [Zephyr RTOS](https://zephyrproject.org/) specifically targeted for the Raspberry Pi Pico W. It provides starting points for Wi-Fi connectivity, basic GPIO manipulation, and debugging configurations.

## Repository Structure

| Sample / Folder | Description |
| :--- | :--- |
| **`blinky`** | The classic "Hello World" of embedded systems. Toggles the onboard LED to verify the basic build system and GPIO functionality. |
| **`_debugger`** | Documentation and configuration files for debugging the RP2040. Includes a comprehensive guide for using the **Black Magic Probe**. |
| **`wifi_scan`** | Demonstrates how to initialize the Wi-Fi interface and perform an active scan to discover nearby wireless networks. |
| **`wifi_shell`** | Integrates the Zephyr shell subsystem, allowing you to interactively manage and monitor Wi-Fi connections via a serial terminal. |
| **`wifi_static`** | A template for establishing a Wi-Fi connection using a hardcoded/static network configuration (SSID and password). |

---

## Prerequisites

Before building these examples, ensure you have the standard (tested with v4.4.0) Zephyr development environment set up:

1. **Zephyr SDK:** Installed and configured on your host machine.
2. **West Tool:** The Zephyr meta-tool installed via Python pip.
3. **Pico W Board:** A Raspberry Pi Pico W and a suitable micro-USB cable.

*For detailed setup instructions, refer to the [official Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html).*

---

## Building and Flashing

To build any of the samples in this repository, navigate to the specific sample's directory or specify it in your `west build` command. 

### 1. Build the Sample
Use `west` to build the firmware for the Raspberry Pi Pico W. For example, to build the `blinky` sample:

```bash
west build -b rpi_pico/rp2040/w -p blinky