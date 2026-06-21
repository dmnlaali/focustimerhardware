/*
  Stasis Focus Timer - firmware
  Arduino Uno + 1.8" ST7735 TFT (128x160, SPI) + 3 clicky buttons + passive buzzer

  ** VOLTAGE WARNING **
  The Uno's SPI pins are 5V. Many ST7735 modules are 3.3V-logic. If yours is not
  marked 5V-tolerant / has no onboard level shifter, run SCK, MOSI, CS, DC, RST
  through a logic-level shifter (or resistor dividers) and power VCC from 3V3,
  or you can damage the display.

  Buttons (each: pin -> button -> GND, using internal pull-ups):
    GP10  START / PAUSE
    GP11  RESET   (back to current preset)
    GP12  MODE    (toggle FOCUS <-> BREAK)
  Buzzer (passive): GP13

  Build:
    Arduino IDE -> select board "Arduino Uno" (no extra board package needed).
    Library Manager -> install "Adafruit ST7735 and ST7789 Library"
      (it pulls in Adafruit GFX automatically).
*/

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// ---------- display pins ----------
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8
// Uno hardware SPI is fixed: SCK = D13, MOSI = D11
// Wire TFT SCK -> D13, TFT SDA/MOSI -> D11

// ---------- inputs / output ----------
#define BTN_START 2
#define BTN_RESET 3
#define BTN_MODE  4
#define BUZZER    5

// ---------- timer presets ----------
const long FOCUS_SECONDS = 25L * 60;
const long BREAK_SECONDS = 5L  * 60;

// ---------- colors ----------
#define COL_BG     ST77XX_BLACK
#define COL_FOCUS  ST77XX_CYAN
#define COL_BREAK  ST77XX_GREEN
#define COL_TEXT   ST77XX_WHITE

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// ---------- state ----------
bool focusMode = true;
long remaining = FOCUS_SECONDS;
bool running = false;
unsigned long lastTickMs = 0;
char lastShown[6] = "";

const uint8_t btnPins[3] = {BTN_START, BTN_RESET, BTN_MODE};
bool btnState[3] = {HIGH, HIGH, HIGH};   // pulled up = HIGH = released

void beep(int freq, int ms, int times = 1) {
  for (int i = 0; i < times; i++) {
    tone(BUZZER, freq, ms);
    delay(ms + 60);
  }
}

long sessionLength() {
  return focusMode ? FOCUS_SECONDS : BREAK_SECONDS;
}

void drawStatic() {
  tft.fillScreen(COL_BG);
  tft.setTextColor(COL_TEXT);
  tft.setTextSize(2);
  tft.setCursor(8, 6);
  tft.print("STASIS");
}

void drawMode() {
  tft.fillRect(0, 28, tft.width(), 18, COL_BG);
  tft.setTextSize(2);
  tft.setTextColor(focusMode ? COL_FOCUS : COL_BREAK);
  tft.setCursor(8, 28);
  tft.print(focusMode ? "FOCUS" : "BREAK");

  tft.setTextColor(COL_TEXT);
  tft.setCursor(100, 28);
  tft.print(running ? "RUN " : "PAUSE");
}

void drawTime(bool force) {
  char buf[6];
  int m = remaining / 60;
  int s = remaining % 60;
  sprintf(buf, "%02d:%02d", m, s);
  if (!force && strcmp(buf, lastShown) == 0) return;
  strcpy(lastShown, buf);

  tft.fillRect(0, 60, tft.width(), 44, COL_BG);
  tft.setTextSize(4);
  tft.setTextColor(focusMode ? COL_FOCUS : COL_BREAK);
  tft.setCursor(18, 66);
  tft.print(buf);
}

// returns true once, on the HIGH->LOW edge (button press)
bool justPressed(uint8_t i) {
  bool now = digitalRead(btnPins[i]);
  bool edge = (btnState[i] == HIGH && now == LOW);
  btnState[i] = now;
  if (edge) delay(15);   // simple debounce
  return edge;
}

void setup() {
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BTN_MODE,  INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  tft.initR(INITR_BLACKTAB);   // if colors are wrong / there's a border, try INITR_GREENTAB or INITR_REDTAB
  tft.setRotation(3);          // landscape -> 160 x 128
  drawStatic();
  drawMode();
  drawTime(true);
}

void loop() {
  // START / PAUSE
  if (justPressed(0)) {
    running = !running;
    lastTickMs = millis();
    beep(1500, 60);
    drawMode();
  }

  // RESET current preset
  if (justPressed(1)) {
    running = false;
    remaining = sessionLength();
    beep(1200, 60);
    drawMode();
    drawTime(true);
  }

  // MODE toggle
  if (justPressed(2)) {
    focusMode = !focusMode;
    running = false;
    remaining = sessionLength();
    beep(1800, 60);
    drawMode();
    drawTime(true);
  }

  // countdown
  if (running && millis() - lastTickMs >= 1000) {
    lastTickMs += 1000;
    remaining--;
    if (remaining <= 0) {
      remaining = 0;
      running = false;
      drawTime(true);
      beep(2200, 150, 3);
      // auto-roll into the other mode, ready to start
      focusMode = !focusMode;
      remaining = sessionLength();
      drawMode();
    }
    drawTime(false);
  }

  delay(5);
}
