target extended-remote | openocd -c 'gdb_port pipe; log_output openocd.log' -f interface/stlink.cfg -f target/stm32f0x.cfg
monitor reset halt
continue &
shell sleep 2
interrupt
bt
info threads
thread apply all bt
quit
