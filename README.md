# TTK4155 Group 30
For our group to save our lab-work

## Using minicom to open a serial connection
The ATmega162 is currently configured to transmit printf messages using `USART0` with a baudrate of 38.4 kBaud, 8 data bits, 1 stop bit and no parity.

Use this command to connect to transmit and receive messages from the ATmega162.
`minicom -D /dev/ttyS0 -b 38400`

## Debugging
Run this command in one terminal to start AVaRICE

`avarice --edbg --ignore-intr :4242`

and then run this command in a second terminal to connect to the AVaRICE server using GDB.

`avr-gdb -tui -x .gdbinit build/a.out`
