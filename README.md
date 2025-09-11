# ttk4155_group24
For our group to save our lab-work

## Using minicom to open a serial connection
The ATmega162 is currently configured to transmit printf messages using `USART0` with a baudrate of 38.4 kBaud, 8 data bits, 1 stop bit and no parity.

Use this command to connect to transmit and receive messages from the ATmega162.
`minicom -D /dev/ttyS0 -b 38400`
