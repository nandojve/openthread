# OpenThread on ATmega256RFR2 Example

This directory contains example platform drivers for the [Atmel
ATmegaRF256RFR2][atmegarf].

[atmegarf]: http://www.atmel.com/devices/ATMEGA256RFR2.aspx

The example platform drivers are intended to present the minimal code
necessary to support OpenThread.  As a result, the example platform
drivers do not necessarily highlight the platform's full capabilities.

## Toolchain

Download and install Atmel Studio 7 to and use the [GNU toolchain for
AVR][gnu-toolchain].

[gnu-toolchain]: http://www.atmel.com/tools/atmelstudio.aspx

## Build Examples

```bash
$ cd <path-to-openthread>
$ ./bootstrap
$ make -f examples/Makefile-atmegarf
```

After a successful build, the `elf` files are found in
`<path-to-openthread>/output/bin`.  You can convert them to `bin`
files using `arm-none-eabi-objcopy`:
```bash
$ arm-none-eabi-objcopy -O binary arm-none-eabi-ot-cli arm-none-eabi-ot-cli.bin
```

## Flash Binaries

Compiled binaries may be flashed onto the CC2538 using the [Serial
Bootloader Interface][atmegarf-bsl] or [JTAG interface][jtag].  The
[atmegarf-bsl.py script][atmegarf-bsl-tool] provides a convenient method
for flashing a CC2538 via the UART.

[atmegarf-bsl]: http://www.ti.com/lit/an/swra466a/swra466a.pdf
[atmegarf-bsl-tool]: https://github.com/JelmerT/atmegarf-bsl
[jtag]: https://en.wikipedia.org/wiki/JTAG

## Interact

1. Open terminal to `/dev/ttyUSB1` (serial port settings: 115200 8-N-1).
2. Type `help` for list of commands.

```bash
> help
help
channel
childtimeout
contextreusedelay
extaddr
extpanid
ipaddr
keysequence
leaderweight
masterkey
mode
netdataregister
networkidtimeout
networkname
panid
ping
prefix
releaserouterid
rloc16
route
routerupgradethreshold
scan
start
state
stop
whitelist
```
