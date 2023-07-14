## Gameboy Camera webcam

This project turns the "eyeball" of the Gameboy Camera into a webcam.

## PCB design

Made in KiCad, check out the boards directory.

## Building, flashing

Run `source setup.sh`, then `./flash.sh`. You can run `./serial.sh` in another tab to show the log output. `./flash.sh` will wait until it sees the device (you have to press down BOOTSEL + RESET).

## MacOS eject notifications
Adafruit has a [wonderful blog](https://blog.adafruit.com/2021/05/11/how-to-tone-down-macos-big-surs-circuitpy-eject-notifications/) showing you how to tone down the annoying "disk ejected unsafely" notifications. It doesn't remove it but at least they auto-dismiss.

## Important assembly note

In rev1, the footprint for the JST connector (the one hooked up to the gameboy camera) is backwards. It needs to be turned 180 - fortunately there is just barely enough clearance that the board still works if you flip it around.

## Pinout

See pinout.h

## Component docs

SHARP LS013B7DH05: [Programming Sharp’s Memory LCDs](https://www.sharpsde.com/fileadmin/products/Displays/2016_SDE_App_Note_for_Memory_LCD_programming_V1.3.pdf)

M64283FP (GB Camera): No official online datasheet (it's from the 90s) but there are sources available. [MITSUBISHI Image Sensor (Artificial Retina Chip)](https://github.com/Raphael-Boichot/Play-with-the-Game-Boy-Camera-Mitsubishi-M64282FP-sensor/blob/main/Additionnal%20informations/Mitsubishi%20Integrated%20Circuit%20M64283FP%20Image%20Sensor.pdf)
