## RP2040 gameboy camera project - WIP

Currently only contains a simple driver for the SHARP LS013B7DH05, which I am going to play around with as a "viewfinder".

## Building, flashing

Run `source setup.sh`, then `./flash.sh`. You can run `./serial.sh` in another tab to show the log output. `./flash.sh` will wait until it sees the device (you have to press down BOOTSEL + RESET).

## MacOS eject notifications
Adafruit has a [wonderful blog](https://blog.adafruit.com/2021/05/11/how-to-tone-down-macos-big-surs-circuitpy-eject-notifications/) showing you how to tone down the annoying "disk ejected unsafely" notifications. It doesn't remove it but at least they auto-dismiss.

## Pinout

```
13 - LED
20 - (reserved for spi0 RX)
6  - spi0 SCK
19 - spi0 TX
1  - spi0 CSN
16 - (reserved for NeoPixel)
2 - gbcam START
3 - gbcam SIN
7 - gbcam LOAD
8 - gbcam XCK
9 - gbcam RESET
10 - gbcam READ
11 - SN74LVC245A OE pin
26 - adc0 gbcam VOUT
```

## Component docs

SHARP LS013B7DH05: [Programming Sharp’s Memory LCDs](https://www.sharpsde.com/fileadmin/products/Displays/2016_SDE_App_Note_for_Memory_LCD_programming_V1.3.pdf)

M64283FP (GB Camera): No official online datasheet (it's from the 90s) but there are sources available. [MITSUBISHI Image Sensor (Artificial Retina Chip)](https://github.com/Raphael-Boichot/Play-with-the-Game-Boy-Camera-Mitsubishi-M64282FP-sensor/blob/main/Additionnal%20informations/Mitsubishi%20Integrated%20Circuit%20M64283FP%20Image%20Sensor.pdf)
