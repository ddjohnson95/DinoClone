# DinoClone
A simple clone of Google's T-Rex Run game for the STM32F0 Discovery Development board. The display is graphical LCD using the the KS0108B parallel interface chipset avaliable here: https://www.sparkfun.com/products/710. Documentation here: https://www.sparkfun.com/datasheets/LCD/GDM12864H.pdf

A graphics library is included. The code can be easily modified to include your own sprites or even make your own games!

TODO: Add wiring diagram. Add music/sound capabilities.

Wiring is as follows

PB0  -> DB0

PB1  -> DB1

PB2  -> DB2
PB3  -> DB3
PB4  -> DB4
PB5  -> DB5
PB6  -> DB6
PB7  -> DB7
PB8  -> CS2
PB9  -> CS1
PB10 -> /RES
PB11 -> R/W
PB12 -> D/I
PB13 -> E

Control pusbuttons are on PA6 and PA7.

![Picture of the game running on the STM32F0 Discovery](https://github.com/ddjohnson95/DinoClone/blob/master/DinoDemo.jpg)
