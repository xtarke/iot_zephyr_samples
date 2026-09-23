# Overview

This is a Zephyr RTOS application that demonstrates Zephyr threads. It implements a main thread that reads from a message queue, along with two additional threads that handle shared global data safely using a mutex and push data into the message queue.

## Requirements

There is no external hardware requirements.

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
*** Booting Zephyr OS build v4.4.0 ***
I: Main thread, waiting for queue messages.
I: Hello from thread_1!
I: Hello from thread_2, enqueue 0.
I: Main received 0.
I: Hello from thread_2, enqueue 1.
I: Main received 1.
I: Hello from thread_2, enqueue 2.
I: Main received 2.
I: Hello from thread_2, enqueue 3.
I: Main received 3.
I: Hello from thread_1!
I: Hello from thread_2, enqueue 4.
I: Main received 4.
I: Hello from thread_2, enqueue 5.
I: Main received 5.
I: Hello from thread_2, enqueue 6.
I: Main received 6.
I: Hello from thread_2, enqueue 7.
I: Main received 7.
I: Hello from thread_1!
I: Hello from thread_2, enqueue 8.
I: Main received 8.
```