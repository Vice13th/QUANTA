# Remote Hardware Test — 12 November 2026

## Executive result

The remote test session reached the physical Arduino UNO R3 on Hive / Windows at COM8. A read-only full-flash readback established that the application initially running on the board was the archived **five-experiment QUANTA 2.0** firmware, not the eight-experiment source previously reported as installed.

The pre-test application region matched the independently compiled five-experiment HEX exactly: 9,430 application bytes and zero mismatches. It did not match the local eight-experiment build: 9,134 mismatched addresses among 12,204 build bytes.

A temporary I²C/OLED diagnostic sketch was uploaded for hardware checks. The exact original five-experiment firmware was then restored, uploaded with `--verify`, and confirmed again by a second full-flash readback with zero application-byte mismatches.

## Environment and identity

| Item | Observed |
|---|---|
| Host | Hive, Windows |
| Board discovery | Arduino UNO, COM8, serial |
| FQBN | `arduino:avr:uno` |
| USB VID/PID | `2341:0043` |
| AVR signature | `1E 95 0F` (ATmega328P) |
| Arduino CLI | 1.5.1 |
| Arduino AVR Boards | 1.8.8 |
| avrdude | 8.0-arduino.1 |
| Bootloader protocol | Arduino / STK500v1, 115200 baud |

## Firmware identity

| Candidate | Flash | Static SRAM | Readback comparison |
|---|---:|---:|---|
| Local eight-experiment source | 12,204 B | 1,564 B | Does not match initial board flash |
| Archived five-experiment source | 9,430 B | 1,530 B | Exact match; zero mismatches |

This is a direct application-flash comparison, not an inference from a source filename or a previous upload log. It shows that the board was running the five-experiment source at the start of this test session.

## I²C and OLED diagnostics

A temporary diagnostic sketch (5,640 B Flash / 444 B static SRAM) scanned the bus and sent SSD1306-style initialization and full-frame data writes.

- Exactly one I²C device was detected: address `0x3C`.
- The device ACKed scans at both 100 kHz and 400 kHz.
- Repeated full-frame tests used four 128×64 frames per speed per cycle.
- At 100 kHz, five intermittent frame-transfer errors were observed across 20 frames (Wire status codes 4 and 2). The reported transactions included command/data writes on pages 2, 4, and 5.
- At 400 kHz, zero frame-transfer errors were observed across 24 frames.
- One additional initialization transaction error was counted; its exact status code was not captured.
- D2 was read as RELEASED throughout the observation window; no physical button actuation was performed.

The five-experiment firmware configures I²C at 400 kHz. The diagnostic results therefore support the bus's ability to ACK and accept sustained writes at the firmware's configured speed, but the intermittent 100 kHz errors still warrant a wiring/pull-up/power check if slower-bus compatibility is required.

**Visual limitation:** no camera or human observation of the OLED pixels was available. I²C ACK and successful writes do not prove visible output, correct panel geometry, contrast, or the correct SSD1306/SH1106 controller mapping.

## Restoration and final state

- Rebuilt the archived five-experiment sketch: 9,430 B Flash / 1,530 B SRAM.
- Uploaded it to COM8 using `arduino-cli upload --verify`; the command returned exit code 0.
- Read all 32,768 bytes of flash after restoration.
- Compared the restored application region to the rebuilt HEX: 9,430 bytes, zero mismatches.
- Arduino CLI rediscovered the UNO on COM8 after restoration.

Post-restore readback:
- File: `uno-com8-restored-five-exp-readback.hex`
- SHA-256: `D9A9347B094629C6C65A6E4FC50CA72E26FADD83BAC5E0761505DAAFE69F59DA`
- Application comparison: exact match, zero mismatches.

**The board was deliberately returned to its pre-test five-experiment firmware. The eight-experiment firmware was not installed during this test session.**

## Not tested

- Visual confirmation of OLED output
- Physical button actuation
- Buzzer acoustic output
- Stack high-water / runtime SRAM margin
- Long-duration stability of the eight-experiment firmware
- Numerical norm conservation, solver convergence, FFT accuracy, or comparison with a trusted numerical reference

## Next step

Select the intended eight-experiment source revision explicitly, then install that revision and run a dedicated instrumented runtime build reporting scenario index, norm, reset cause, frame timing, and memory diagnostics over Serial. Retain the verified five-experiment image as the recovery baseline until that deployment is approved and tested.
