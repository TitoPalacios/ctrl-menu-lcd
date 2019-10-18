# ctrl-menu-lcd
How to use rotary encoder and lcd to manage information in tree menu format.

Libraries
-
>**DS3232RTC** - Arduino Library for Maxim Integrated DS3232 and DS3231 Real-Time Clocks
>-- Jack Christensen [https://github.com/JChristensen/DS3232RTC.git]

>**Encoder** - Quadrature Encoder Library for Arduino
>-- Paul Stoffregen [https://github.com/PaulStoffregen/Encoder]

>**LCDMenuLib2** - Easy creation of a tree based menu with screensaver and multi layers.
>-- Nils Feldkaemper [https://github.com/Jomelo/LCDMenuLib2]

>**LiquidCrystal_I2C** - Hitachi HD44780 & clone LCD screens over I²C expander PCF8574
>-- ejoyneering [https://github.com/enjoyneering/LiquidCrystal_I2C.git]

>**Time** - Time keeping library
>-- Michael Margolis [https://github.com/PaulStoffregen/Time]


# Notes
I recommend to use a capacitor (0.01uF) between ground and each of the terminals of the rotary encoder to avoid debounce problem with the encoder.
