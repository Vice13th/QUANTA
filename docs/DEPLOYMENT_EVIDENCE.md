# Deployment Evidence — 12 November 2026

## Supplied local installation report

| Field | Reported value | Evidence status |
|---|---|---|
| Host/device | Hive, Windows | Local report |
| Board | Arduino UNO / ATmega328P | Reported identification |
| Port | COM8 | Reported identification |
| Source path | Not retained in repository | Privacy minimization |
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

A historical GitHub Actions run in the predecessor repository (run ID `43219411457`) passed all four sketches on the PR head. This is source-repository evidence, not a CI run in `Vice13th/QUANTA`. The hosted runner reported 12,214 B Flash for the three eight-experiment sketches and 9,430 B for the five-experiment source; static SRAM was 1,564 B and 1,530 B respectively. The 10-byte Flash difference for eight-experiment builds between local Windows and hosted Linux remains unexplained. Do not treat these two build environments as bit-for-bit reproducible until the compiler/toolchain details are compared. Both environments agree on static SRAM.

The hosted build emitted four unused-parameter warnings from the Arduino AVR core's `new.cpp`; no project-source warning was identified in the captured lines. The eight-experiment builds retain the core's low-memory warning. These are compile results only, not runtime stack or hardware stability results.


## Remote hardware readback and I²C test — 12 November 2026

See [Remote Hardware Test](REMOTE_HARDWARE_TEST_2026-11-12.md). The physical UNO on COM8 was found to contain the five-experiment archive, not the eight-experiment source previously reported. The five-experiment application region matched its independently compiled HEX exactly (9,430 bytes, zero mismatches). A temporary I²C diagnostic found OLED address 0x3C; 400 kHz writes completed 24 full frames with zero transfer errors, while 100 kHz showed five intermittent frame-transfer errors plus one initialization error. The original five-experiment image was restored and verified by a second flash readback (zero application-byte mismatches). OLED visual output and physical controls were not independently observed.


A follow-up watchdog diagnostic observed `WDTCSR=0x40` and 512 then 642 WDT interrupts per board-reported 5,000 ms window. The resulting 102–128 Hz rate is faster than the nominal 16 ms WDP=0000 period; the board's own `millis()` was the timebase and Timer1 capture was inconclusive. This is recorded as a timing anomaly requiring independent calibration, not a confirmed failure. See [Remote Hardware Test](REMOTE_HARDWARE_TEST_2026-11-12.md).


References for the physical I²C and watchdog observations are listed in [Remote Hardware Test](REMOTE_HARDWARE_TEST_2026-11-12.md).

## New-repository CI verification — 13 November 2026

The new repository's `main` branch completed the pinned four-snapshot Arduino UNO workflow successfully in [GitHub Actions run 43388797556](https://github.com/Vice13th/QUANTA/actions/runs/43388797556), for commit `7312b27b82a63f9f6cb989315942cb6992446e7f`. The workflow used Arduino CLI 1.5.1, Arduino AVR Boards 1.8.8, and `arduino:avr:uno`. The hosted runner compiled the canonical sketch and all three archived snapshots. The eight-scenario sketches used 12,214 B Flash and 1,564 B static SRAM (484 B nominally remaining); the five-experiment archive used 9,430 B Flash and 1,530 B static SRAM (518 B nominally remaining). The eight-scenario builds emitted Arduino CLI's low-memory warning. This CI result demonstrates compilation only; it does not establish upload, runtime stack margin, peripheral behavior, or numerical validity.

The historical predecessor-repository run IDs are not new-repository CI evidence. New-repository branch CI for the test and firmware-review branches is tracked in PRs [#2](https://github.com/Vice13th/QUANTA/pull/2) and [#3](https://github.com/Vice13th/QUANTA/pull/3).