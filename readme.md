# Stasis Focus Timer

A tiny desk gadget that keeps you in flow — a Pomodoro-style focus timer built around an Arduino Uno, a 1.8" color TFT, three clicky buttons, and a buzzer. Runs fully offline, no phone needed.

![Focus Timer — finished build](/photo1.png)
![Focus Timer — screen running](/photo2.png)

> **Replace the two images above with real photos of YOUR build** — one finished-hardware shot and one close-up of the screen running. The submission check fails if the README has no photos. Put them in a `docs/` folder.

## ⚠️ Display voltage note
The Arduino Uno drives its SPI pins at **5V**, but many 1.8" ST7735 modules are **3.3V logic**. If your board is not marked 5V-tolerant (no onboard level shifter), put the signal lines (SCK, MOSI, CS, DC, RST) through a logic-level shifter or resistor dividers, and power VCC from 3V3. Feeding 5V into a 3.3V-only panel can damage it.

## What it does
- 25-minute focus / 5-minute break sessions (Pomodoro)
- Three dedicated clicky buttons: start/pause, reset, switch mode
- Big color MM:SS readout on a 1.8" TFT (cyan for focus, green for break)
- Buzzer chime when a session ends, then auto-rolls into the next block
- Fully standalone — USB power, no wifi or app

## Bill of materials
| Part | Qty | ~Price (USD) |
|------|-----|--------------|
| Arduino Uno (or clone) | 1 | $8.00 |
| 1.8" ST7735 TFT LCD (128×160, SPI) | 1 | $13.66 |
| Phantom Blue clicky switches | 10 | $3.07 |
| Passive piezo buzzer | 1 | $1.00 |
| Custom 3D-printed enclosure | 1 | ~$2.00 filament |
| Wires / headers / misc | — | ~$5.13 |
| **Total** | | **$32.86** |


## Wiring

**Display (hardware SPI)**
| Uno pin | TFT module |
|---------|------------|
| D13 (SCK)  | SCK / CLK |
| D11 (MOSI) | SDA / MOSI / DIN |
| D10        | CS |
| D9         | DC / A0 / RS |
| D8         | RST / RESET |
| 3V3        | VCC + LED (backlight) — see voltage note |
| GND        | GND |

The module's SD-card pins (if present) are unused.

**Buttons & buzzer**
| Uno pin | Connects to |
|---------|-------------|
| D2 | Button 1 (start / pause) → GND |
| D3 | Button 2 (reset) → GND |
| D4 | Button 3 (mode) → GND |
| D5 | Buzzer + (− to GND) |

Buttons use the Uno's internal pull-ups, so each switch just bridges its pin to GND.

## Firmware
Written in Arduino C++ (`focus_timer.ino`).

1. In Arduino IDE, select board **Arduino Uno** (no extra board package needed).
2. **Library Manager → install "Adafruit ST7735 and ST7789 Library"** (it pulls in Adafruit GFX).
3. Put `focus_timer.ino` in a folder named `focus_timer/`, open it, and upload.

> If the screen colors look inverted or there's a coloured border, change `INITR_BLACKTAB` in `setup()` to `INITR_GREENTAB` or `INITR_REDTAB` — it depends on your exact ST7735 board batch.

### Controls
- **Button 1** — start / pause
- **Button 2** — reset the current session
- **Button 3** — toggle between focus and break

## Enclosure / CAD
Designed in Onshape. Source files live in [`/cad`](cad/) as **STEP** (`.step`) so they stay editable. Printed on FDM at 0.2 mm layer height.

## Future improvements
- Adjustable durations from the buttons
- Battery power for a fully wireless timer
- On-screen session counter

## License
MIT