###HY-Mini STM32v Media Control System
This project is working as a Frontend for a small media
system in combination with a Raspberry Pi. The Pi will 
handle the media playback and will drive the content on the
display.

Communication between the STM32 and the Pi is happening via USART.
Following Pins on the STM32 are used for this

PA9  -> USART1_TX
PA10 -> USART1_RX

Working at 115200 8N1