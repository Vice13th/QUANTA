# Deployment Evidence — 12 November 2026

## Supplied local installation report

| Field | Reported value | Evidence status |
|---|---|---|
| Host/device | Hive, Windows | Local report |
| Board | Arduino UNO / ATmega328P | Reported identification |
| Port | COM8 | Reported identification |
| Source path | `[REDACTED]` | Reported path |
| Source SHA-256 | `162BB11166AD2BF11EFBE75F37F1EB868AF1A15500C0A31EDBD1B5DC25601518` | Independently recomputed from local source during audit |
| Flash | 12,204 bytes | Reported build metric |
| Static SRAM | 1,564 bytes | Reported build metric |
| Nominal SRAM remaining | 484 bytes | Derived from 2,048-byte capacity |
| Upload | Successful | Supplied report; raw uploader transcript/readback checksum not supplied |
| OLED / controls / long-run runtime | Not verified | Outstanding |

The associated local source is preserved at `firmware/archive/installed-snapshot-2026-11-12/QUANTA2.ino`.

## Evidence distinctions

- A source SHA-256 is not a compiled HEX/BIN SHA-256.
- A success message is not flash readback verification.
- USB re-enumeration does not prove OLED/peripheral behavior.
- Historical memory metrics do not automatically apply to canonical or archived variants.
- No measured FPS, multi-hour soak, stack high-water, or numerical reference comparison is present in the supplied evidence.

## Required next hardware record

Capture exact Git commit/source SHA-256, CLI/core/FQBN, build memory output, board revision/port, complete upload output including verify result, OLED controller/address/wiring, evidence of all display pages, button/buzzer behavior, FPS, resets, duration, stack high-water, and numerical norm/error logs. Do not mark unperformed tests passed.


## Independent compile audit — 12 November 2026

A clean checkout of the merged branch was compiled on Hive (Windows) with Arduino CLI 1.5.1 and Arduino AVR Boards 1.8.8. All four sketches exited with code 0:

| Sketch | Flash | Static SRAM | Remaining SRAM | Result |
|---|---:|---:|---:|---|
| Canonical eight-scenario | 12,204 B | 1,564 B (76%) | 484 B | Pass |
| Five-experiment archive | 9,430 B | 1,530 B (74%) | 518 B | Pass |
| Supplied eight-experiment snapshot | 12,204 B | 1,564 B (76%) | 484 B | Pass |
| Reported installed-source snapshot | 12,204 B | 1,564 B (76%) | 484 B | Pass |

GitHub Actions run [43219411457](https://github.com/Vice13th/QUANTA2/actions/runs/43219411457) also passed all four sketches on the PR head. The hosted runner reported 12,214 B Flash for the three eight-experiment sketches and 9,430 B for the five-experiment source; static SRAM was 1,564 B and 1,530 B respectively. The 10-byte Flash difference for eight-experiment builds between local Windows and hosted Linux remains unexplained. Do not treat these two build environments as bit-for-bit reproducible until the compiler/toolchain details are compared. Both environments agree on static SRAM.

The hosted build emitted four unused-parameter warnings from the Arduino AVR core's `new.cpp`; no project-source warning was identified in the captured lines. The eight-experiment builds retain the core's low-memory warning. These are compile results only, not runtime stack or hardware stability results.


## Remote hardware readback and I²C test — 12 November 2026

See [Remote Hardware Test](REMOTE_HARDWARE_TEST_2026-11-12.md). The physical UNO on COM8 was found to contain the five-experiment archive, not the eight-experiment source previously reported. The five-experiment application region matched its independently compiled HEX exactly (9,430 bytes, zero mismatches). A temporary I²C diagnostic found OLED address 0x3C; 400 kHz writes completed 24 full frames with zero transfer errors, while 100 kHz showed five intermittent frame-transfer errors plus one initialization error. The original five-experiment image was restored and verified by a second flash readback (zero application-byte mismatches). OLED visual output and physical controls were not independently observed.
