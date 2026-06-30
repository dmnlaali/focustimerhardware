# Stasis Focus Timer

A little Pomodoro timer for my desk — 25 min of work, 5 min of break — running on an Arduino Uno with a small color screen, three buttons and a buzzer. No phone, no app. Plug in USB and it runs.

I built it because my phone timer kept turning into a "just check one thing" trap. A dumb little box that only does timers fixed that.

![Focus Timer — finished build](/photo1.png)
![Focus Timer — screen running](/photo2.png)


## Quick warning about the screen

Some of these 1.8" ST7735 screens only handle 3.3V. If yours isn't marked as 5V tolerant, power its **VCC from the 3V3 pin** (not 5V) — otherwise you can damage it. If you're not sure, 3V3 is the safe choice.

## What it does

- 25 min focus / 5 min break sessions
- Three buttons: start/pause, reset, switch mode
- Big MM:SS countdown — cyan for focus, green for break
- Buzzes at the end and rolls into the next block
- Runs on its own, just needs USB power

## Parts

| Part | Qty | ~Price |
|------|-----|--------|
| Arduino Uno (or clone) | 1 | $8.00 |
| 1.8" ST7735 TFT LCD (128×160, SPI) | 1 | $13.66 |
| Phantom Blue clicky switches | 10 | $3.07 |
| Passive piezo buzzer | 1 | $1.00 |
| 3D-printed case | 1 | ~$2.00 filament |
| Wires / headers / bits | — | ~$5.13 |
| **Total** | | **$32.86** |

## Wiring

**Screen**

| Uno pin | Screen pin |
|---------|------------|
| D13 | SCK / CLK |
| D11 | SDA / MOSI / DIN |
| D10 | CS |
| D9  | DC / A0 / RS |
| D8  | RST |
| 3V3 | VCC + LED (backlight) |
| GND | GND |

**Buttons & buzzer**

| Uno pin | Goes to |
|---------|---------|
| D2 | Button 1 (start/pause) → GND |
| D3 | Button 2 (reset) → GND |
| D4 | Button 3 (mode) → GND |
| D5 | Buzzer + (− to GND) |

The buttons use the Uno's built-in pull-ups, so each one just connects its pin to GND.

## Code

It's all in `focus_timer.ino`.

1. In the Arduino IDE, pick board **Arduino Uno**.
2. In Library Manager, install **"Adafruit ST7735 and ST7789 Library"** (it grabs Adafruit GFX too).
3. Put `focus_timer.ino` in a folder called `focus_timer/`, open it, and upload.

> If the colors look off or there's a coloured border, change `INITR_BLACKTAB` in `setup()` to `INITR_GREENTAB` or `INITR_REDTAB`. Depends on your screen batch — mine took some trial and error.

### Controls
- Button 1 — start / pause
- Button 2 — reset
- Button 3 — switch focus/break

## Case

Designed in Onshape, files are in [`/cad`](cad/) as STEP. Printed at 0.2 mm layer height.

## Maybe later
- Adjustable timer lengths from the buttons
- Battery so it's wireless
- A session counter on screen

## License
MIT