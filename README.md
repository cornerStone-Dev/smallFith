# smallFith

Note: When using WSL, as of October 2023, WSL 1 is requried to use the `make testRun` recipe in the Makefile, rather than the default WSL 2.
WSL distros may be easily ported between versions as necessary by using the `wsl --set-version` command in powershell (eg ` wsl --set-version Ubuntu 1`)

The default console UART pins are GPIO12 (TX) and GPIO13 (RX)on a standard Raspberry Pi Pico.  When connecting a standard console cable connect as follows:
GPIO12 --> White
GPIO13 --> Green
GND    --> Black

Serial pins may be altered to any avialable UART, which may be required for non-pico RP2040 boards.  UART pins are configured in `./start.s` in a section labled `;@ Section UART` circa line 616.

Reccomended serial client it PuTTY, though others work with the right settings.
Serial Settings:
- Speed(baud):  115200
- Data bits:    8
- Stop bits:    1
- Parity:       None
- Flow control: XON/XOFF

Terminal Settings:
- Implicit CR in every LF:  ON
- Implicit LF in every CR:  OFF
- Local echo: Force on
- Local line editing:  Force on
