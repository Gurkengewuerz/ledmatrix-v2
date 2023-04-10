## LED Matrix (English)

This project was created as a successor of the [version 1](https://github.com/Gurkengewuerz/ledmatrix) LED Matrix.  
This version mainly offers animations for a high density table.

## Changes to v1

- Square IKEA LACK table (55x55cm instead of 95x55cm)
- black/brown wood instead of the white one
- ESP32 instead of an Raspberry Pi Zero W
- 25x25 LEDs instead of 10x20
- lasered dividers with Finnpappe
- Includes sensors like accelerometer/gyroscope, current sensor and temperature

## Hardware

- 10.5m WS2812b LED strips (60 LEDs/m)
- IKEA Lack table
- Power supply (5V, enough to power all the LEDs)
- ESP32-C3 (WEMOS C3 mini)

## Build and Flashing

First of all, install PlatformIO with your favourite IDE (i.e. VSCode). Follow [this](https://platformio.org/platformio-ide) for instructions.

Clone the repo

```sh
git clone https://github.com/Gurkengewuerz/ledmatrix-v2.git
```

Open cloned folder with your PlatformIO IDE like VSCode.

```shell
cd ledmatrix-v2
code .
```

Connect the ESP32-C3 via USB. Select `env` variant **ledmatrix-local** on VisualCode and build it, `PlatformIO icon -> env:ledmatrix-local -> Build`, in the same menu, select `upload`.

```bash
pio run -e ledmatrix-local --target upload
```

PlatformIO will build and upload the binaries to the microcontroller.

## Development

Development can either be done on the microcontroller itself or by running it locally in a simulator. The simulator is running inside the terminal. A Terminal with TrueColor support is highly recommended!  
The simulator depends on the system GCC toolchain that must be added to the `PATH` system environment variable. Please open the system terminal and type `gcc --version`. If the `gcc` command is not found, you have to install the GCC toolchain manually depending on your operating system:

To build and run the simulator use `pio run --environment simulation-native --target exec` or omit `--target exec` to just build and run inside your prefered terminal.
