# Zephyr RTOS Examples for Raspberry Pi Pico W

This repository contains a collection of IoT examples and utilities built with [Zephyr RTOS](https://zephyrproject.org/) specifically targeted for the Raspberry Pi Pico W. It provides starting points for Wi-Fi connectivity, basic GPIO manipulation, and debugging configurations.

## Repository Structure

| Sample / Folder | Description |
| :--- | :--- |
| **01-blinky** | The classic "Hello World" of embedded systems. Toggles the onboard LED to verify the basic build system and GPIO functionality. |
| **01-button_callback** | A simple button demo showcasing the use of buttons with the `input` APIs. |
| **01-events** | This is a Zephyr RTOS application that demonstrates how to handle hardware inputs, utilize kernel events, and control GPIO outputs  |
| **02-adc_dt** | This sample demonstrates how to use the ```ADC driver API <adc_api>```. |
| **02-pwm** | This is a Zephyr RTOS application that demonstrates the initializing of a PWM device setting it to a 50% duty cycle using a predefined period. |
| **03-bmp_280_i2c** | This is a Zephyr RTOS application that demonstrates Zephyr sensor API reading a BMP280 sensor. |
| **04-threads** | This is a Zephyr RTOS application that demonstrates Zephyr threads. |
| **05-wifi_scan** | Demonstrates how to initialize the Wi-Fi interface and perform an active scan to discover nearby wireless networks. |
| **05-wifi_shell** | Integrates the Zephyr shell subsystem, allowing you to interactively manage and monitor Wi-Fi connections via a serial terminal. |
| **05-wifi_static** | A template for establishing a Wi-Fi connection using a hardcoded/static network configuration (SSID and password). |
| **06-http_post** | This application demonstrates how to connect a board to a Wi-Fi network using static credentials in **Zephyr RTOS** and periodically transmit sensor telemetry to the **Tago.IO** cloud platform via HTTP POST requests.
| **07-mqtt** | This application demonstrates how to establish a Wi-Fi connection and communicate with an MQTT broker using the **Zephyr RTOS** networking stack. |
| **_debugger** | Documentation and configuration files for debugging the RP2040. Includes a comprehensive guide for using the **Black Magic Probe**. |
| **__toolbox**

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
west build -b rpi_pico/rp2040/w -p
```