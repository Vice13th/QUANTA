# Hardware Guide

## 1. Supported target

The firmware targets an Arduino UNO R3 based on the ATmega328P at 16 MHz. Compatible boards may work, but pin mappings, I²C behavior, and available memory should be checked before use.

## 2. Required components

- Arduino UNO R3 or compatible ATmega328P board.
- 128×64 I²C OLED module.

## 3. Optional components

- Momentary push button.
- Small buzzer compatible with the board's GPIO output.

## 4. Wiring

| Module | Signal | Arduino |
|---|---|---|
| OLED | SDA | A4 |
| OLED | SCL | A5 |
| OLED | VCC | Match module rating |
| OLED | GND | GND |
| Button | One terminal | D2 |
| Button | Other terminal | GND |
| Buzzer | Signal | D9 |
| Buzzer | Return | GND |

D13 is the built-in status/detector LED on the UNO board.

**Power warning:** check the OLED module's documented supply-voltage range and logic-level requirements. Do not assume every breakout board is 5 V tolerant. Avoid shorting power rails and disconnect power before changing wiring.

## 5. OLED detection

At startup, the firmware probes I²C address `0x3C`. If no device responds, it probes `0x3D`. If neither address responds, the firmware enters a repeating D13 LED blink loop.

The source uses a 400 kHz I²C clock and an SSD1306-style command sequence. Although the source comments mention SSD1306/SH1106, compatibility depends on the controller, module wiring, and column offset. Test the exact display module rather than assuming universal compatibility.

## 6. Controls

- Short button press on D2: trigger/advance the measurement cycle.
- Long button press on D2: toggle wave/momentum visualization.
- Buzzer on D9: short detector click.
- D13: detector activity or display-detection failure indication.

The current firmware uses simple loop-based press-duration detection. It does not implement a dedicated, formally verified debounce subsystem.

## 7. First power-on checklist

1. Disconnect USB power.
2. Verify SDA=A4 and SCL=A5.
3. Verify OLED VCC/GND polarity and module voltage rating.
4. Confirm no loose wires or short circuits.
5. Connect the button and buzzer only if needed.
6. Connect USB and observe D13/display behavior.
7. If the display remains blank, verify its I²C address and controller.
8. Power-cycle after wiring changes.

## 8. Troubleshooting

| Symptom | Checks |
|---|---|
| D13 blinks and no UI appears | OLED power, ground, SDA/SCL, address 0x3C/0x3D |
| Display powers but shows garbage | Controller compatibility, initialization sequence, column offset |
| Button does nothing | D2-to-GND wiring and momentary switch operation |
| No detector sound | D9 wiring, buzzer type, polarity |
| Random reset or frozen display | SRAM/stack margin, power stability, long-run validation |

## 9. Hardware evidence

Record the exact board revision, OLED controller/module, I²C address, supply arrangement, and test duration when reporting a hardware result. A successful firmware upload alone does not verify peripheral behavior.


## 10. Timer and display compatibility constraints

Timer1 is configured as a free-running counter for timing-state mixing. Do not assume Timer1-dependent libraries or PWM on UNO pins D9/D10 will work concurrently. The optional buzzer uses D9 as a digital output, not a PWM audio channel.

The SSD1306-style initialization sequence and configurable column offset are controller-specific. Detection at I²C address `0x3C` or `0x3D` does not prove SSD1306/SH1106 compatibility. Verify the exact controller and offset physically.

## 11. Startup I2C bus recovery

Before initializing Wire, the production firmware releases SDA/SCL and checks their line levels. If SDA is LOW while SCL is HIGH, it emits at most nine open-drain SCL pulses and then synthesizes a STOP if both lines release. It never actively drives a bus line HIGH. If SCL remains LOW, software stops the recovery attempt; check module power, wiring, pull-ups, and the device holding the line. This is a bounded bus-clear attempt, not a substitute for measuring electrical rise time and VOL.

The firmware sets a 25 ms Wire timeout with TWI-peripheral reset on timeout, counts endTransmission errors, and latches D13 on communication errors. Peripheral reset alone cannot release a slave that physically holds SDA/SCL LOW.

## D2 control behavior — latest field report (2026-11-12)

D2 is configured as INPUT_PULLUP and is intended to connect to GND through a momentary button. The user reports that a long press returns the entire experiment to TUNNELING, apparently in place and without a visible OLED blank/reboot. This is not accepted behavior and is not yet explained.

The long-press handler toggles the view flag kv; it does not directly reset the scenario counter sc. Do not assume the switch wiring or debounce is the cause without electrical/state evidence. Button waveform, contact bounce, reset-cause register, and scenario-state trace have not yet been captured. See CHECKPOINT_2026-11-12_D2_LONG_PRESS.md.
