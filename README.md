## RP2040 gameboy camera project - WIP

Currently only contains a simple driver for the SHARP LS013B7DH05, which I am going to play around with as a "viewfinder".

pinout:
```
13 - LED
20 - (reserved for spi0 RX)
6  - spi0 SCK
19 - spi0 TX
1  - spi0 CSN
16 - (reserved for NeoPixel)
```
