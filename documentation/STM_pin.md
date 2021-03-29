## TABELLA PIN HIMAX

| NOME SU BREAKOUT | SIGNIFICATO                                                  | PINS SU STM | NOME SU BREAKOUT | SIGNIFICATO                               | PIN SU STM |
| ---------------- | ------------------------------------------------------------ | ----------- | ---------------- | ----------------------------------------- | ---------- |
| SDA              | i2c data                                                     | PB9         | GND              | ground                                    | -          |
| SCL              | i2c clock                                                    | PB8         | INT              | interrupt out - used for motion detection | -          |
| D7               | data out                                                     | PC7         | D6               | data out                                  | PC6        |
| D5               | data out                                                     | PC5         | D4               | data out                                  | PC4        |
| D3               | data out                                                     | PC3         | D2               | data out                                  | PC2        |
| D1               | data out                                                     | PC1         | D0               | data out                                  | PC0        |
| HCK              | horizontal clock - linea - interrupt                         | PB14        | PCK              | pixel clock - interrupt                   | PB13       |
| TRG              | trigger -per dire a camera quando voglio frame(stream md 3)  | PB1         | MCK              | master clock                              | PA8        |
| VCK              | vertical clock - frame  - interrupt                          | PB15        | ENB              | attivazione voltage shifter               | -          |
| 1V8              | alimentazione attuale                                        | 3V3         | GND              | ground                                    | -          |
| VDD              | ??                                                           | -           | GND              | ground                                    | GND        |

## TABELLA PIN FLIR

| PIN SU FLIR | DESCRIZIONE   | PIN SU STM |
| ----------- | ------------- | ---------- |
| Dietro      | Alimentazione | 5V         |
| 1           | Ground        | GND        |
| 5           | I2C - SDA     | PB3        |
| 8           | I2C - SCL     | PB10       |
| 12          | SPI - MISO    | PA6        |
| 9           | SPI - MOSI    | PA7        |
| 10          | SPI - CS      | PB6        |
| 7           | SPI - SCK     | PA5        |

