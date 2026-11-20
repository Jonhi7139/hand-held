# Hand-held
 
A DIY handheld game console built around a **Raspberry Pi 5**, with a custom **ESP32 controller**.The firmware is being written in C with ESP-IDF and FreeRTOS. The long-term goal is a handheld that is fully my own, from the controller firmware to a custom PCB.


> **Status:** in progress:
>   So far the firmware has the D-pad configured and an analog stick with i'ts built in button. The Debugging is being made over prints ni the terminal

## Architecture
 
```
 Buttons + analog stick ──► ESP32 controller ──► Raspberry Pi 5
```

The ESP32 handles all the input and, later, the power and battery management. The Pi only  responsible for running the games.

## Controller firmware
 
Code in [`Firmware/controller`](Firmware/controller), built with PlatformIO on the ESP-IDF framework for an ESP32 DevKit.
| File | What it does |
|---|---|
| `board.h` | All pin assignments in one place |
| `buttons.c` | Button configuration and reading task |
| `sticks.c` | Analog stick configuration and reading task |
| `main.c` | Creates the FreeRTOS tasks |
 
Design choices**
 
- **Periodic FreeRTOS tasks.** Buttons and sticks each run in their own task every 10 ms using `vTaskDelayUntil` for consistency

- **Data-driven inputs.** Each button and stick is an entry in an array of structs.

**Pinout**
 
| Input | GPIO |
|---|---|
| D-pad up / down / left / right | 32 / 14 / 13 / 33 |
| Left stick button (L3) | 25 |
| Left stick X / Y | 34 / 35 (ADC1) |

## Build and flash
 
```bash
cd Firmware/controller
pio run -t upload
pio device monitor   # 115200 baud
```

## Done so far
- ESP32 firmware reading the D-pad, L3 and the left analog stick in periodic FreeRTOS tasks

## Next steps
 
- [ ] Send the input to the Pi as a Bluetooth gamepad
- [ ] Debounce the buttons and add a dead zone and calibration for the stick
- [ ] Measure the latency from a button press to the event on the Pi
- [ ] Battery and power management on the ESP32
- [ ] Custom PCB
