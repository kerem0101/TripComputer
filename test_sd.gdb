target extended-remote | openocd -c "gdb_port pipe" -f interface/stlink.cfg -f target/stm32f0x.cfg
monitor reset halt
load
monitor reset run
shell sleep 20
monitor halt
print sd_test_read_buffer
quit
