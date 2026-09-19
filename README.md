# dinosaur-game-bot

An Arduino Mega 2560 project that plays Chrome's offline dinosaur game automatically. A photoresistor (LDR) taped to the screen detects incoming cacti, and a servo motor physically taps the spacebar to jump.

## How it works

The LDR sits on the screen along the path where obstacles travel, positioned ahead of the dino. As a dark cactus passes under it, the light level drops, which shows up as a voltage drop across an LDR/resistor voltage divider read on analog pin A0. When that voltage drops below a threshold, the Arduino fires a servo (pin 9) that taps the spacebar.

The code also accounts for the game's actual acceleration curve (pulled from Chromium's source, `SPEED: 6`, `ACCELERATION: 0.001`, `MAX_SPEED: 13`, all in px/frame at 60fps). Since the game runs at less than half speed for the first ~2 minutes, a fixed-timing trigger jumps too early during that window. The sketch adds a speed-scaled delay before firing so the timing holds up across the whole run, not just at max speed.

## Hardware

- Arduino Mega 2560
- Breadboard
- 1x LDR (photoresistor)
- 1x 10k ohm resistor
- 1x SG90 micro servo
- Jumper wires
- Tape or a zip tie for mounting

All parts came from the ELEGOO Mega 2560 Most Complete Starter Kit (EL-KIT-008).

## Wiring

**LDR voltage divider (on breadboard):**
- Arduino 5V → breadboard + rail
- Arduino GND → breadboard − rail
- LDR: one leg to + rail, other leg into a junction row
- 10k resistor: one leg into that same junction row, other leg to − rail
- Junction row → Arduino A0

**Servo:**
- Brown (GND) → breadboard − rail
- Red (V+) → breadboard + rail
- Orange (signal) → Arduino pin 9

## Setup

1. Wire the circuit as described above.
2. In the Arduino IDE: **Tools > Board > Arduino Mega or Mega 2560**, **Tools > Processor > ATmega2560**, then select the correct port.
3. Upload `dino_auto_player_final.ino`.
4. Mount the servo so its horn sits just above the spacebar.
5. Tape the LDR to the screen, roughly 20-30mm to the right of the dino, along the ground line where cacti travel.

## Calibration

Two constants need tuning for your specific physical setup, both flagged as `PLACEHOLDER` in the code:

- **`thresholdVoltage`** — uncomment the `Serial.println(sensorVoltage)` line, open Serial Monitor, and watch the readings with the game running. Set the threshold between the baseline (light background) and cactus (dark) readings, then comment the print line back out.
- **`extraDelayCoefficientMs`** — reset the Arduino exactly when a fresh game starts (the delay formula uses elapsed time since reset to estimate current game speed). Watch how early or late the dino jumps at low speed, near the start of a run, and adjust this constant up or down until the timing holds.

Also check `pressPosition` — the servo angle that presses the key — physically once mounted, and adjust if it isn't pressing hard enough or is over-traveling into the keyboard.

## Known limitations

- LDR position is a fixed compromise; a single sensor can't be perfectly tuned for every game speed simultaneously, though the speed-aware delay narrows that gap.
- Ambient room lighting shifts the LDR's baseline reading, so `thresholdVoltage` may need re-tuning if you move to a different room or time of day.
- The delay calibration assumes the Arduino reset lines up with the actual game start; a mismatch there throws off the early-game timing for that run.
