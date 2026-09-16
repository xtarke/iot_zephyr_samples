arm-none-eabi-gdb -q -batch \
  -ex "target extended-remote /dev/ttyACM0" \
  -ex "set remotetimeout 30" \
  -ex "monitor swdp_scan" \
  -ex "attach 1" \
  -ex "monitor reset halt" \
  -ex "kill"


arm-none-eabi-gdb -q -batch \
  -ex "target extended-remote /dev/ttyACM0" \
  -ex "monitor frequency 1000000" \
  -ex "set remotetimeout 30" \
  -ex "monitor swdp_scan" \
  -ex "attach 1" \
  -ex "load" \
  -ex "kill" \
  build/zephyr/zephyr.elf
