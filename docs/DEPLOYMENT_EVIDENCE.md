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

## Latest production image checkpoint — commit acb1d2b (2026-11-12)

This is the latest verified production image on the connected COM8 board:
- Source: firmware/QUANTA2/QUANTA2.ino
- Source SHA-256 at checkpoint: 667077B520E0088543D3037F87A4253663B6BFE69445900E0B551E38E5F6E3DF
- Commit: acb1d2b51412bcdfa8e86337adddb92054cc0a4b
- Build: 12,618 B Flash; 1,570 B static SRAM; 478 B nominal SRAM remaining.
- Upload: Arduino CLI --verify succeeded.
- Independent Flash readback: 12,618 application bytes compared against the generated HEX; zero mismatches.
- GitHub Actions run 43287974052: completed successfully for the same commit.
- PR #4 remains open and unmerged.

The image includes the view-persistence correction (bool sk=kv). After installing it, the user reported that a D2 long press returns the entire experiment to TUNNELING without a visible display blank/reboot. This is an unresolved runtime defect; the view correction must not be described as a complete D2 fix.

A later Serial diagnostic build was compiled but not uploaded: 14,054 B Flash / 1,747 B static SRAM / 301 B nominal remaining. It was rejected for deployment because of the reduced memory margin. No diagnostic image replaced the production firmware.
See CHECKPOINT_2026-11-12_D2_LONG_PRESS.md.

## New-repository CI verification — 13 November 2026

The new repository's `main` branch completed the pinned four-snapshot Arduino UNO workflow successfully in [GitHub Actions run 43388797556](https://github.com/Vice13th/QUANTA/actions/runs/43388797556), for commit `7312b27b82a63f9f6cb989315942cb6992446e7f`. The workflow used Arduino CLI 1.5.1, Arduino AVR Boards 1.8.8, and `arduino:avr:uno`. The hosted runner compiled the canonical sketch and all three archived snapshots. The eight-scenario sketches used 12,214 B Flash and 1,564 B static SRAM (484 B nominally remaining); the five-experiment archive used 9,430 B Flash and 1,530 B static SRAM (518 B nominally remaining). The eight-scenario builds emitted Arduino CLI's low-memory warning. This CI result demonstrates compilation only; it does not establish upload, runtime stack margin, peripheral behavior, or numerical validity.

The historical predecessor-repository run IDs are not new-repository CI evidence. New-repository branch CI for the test and firmware-review branches is tracked in PRs [#2](https://github.com/Vice13th/QUANTA/pull/2) and [#3](https://github.com/Vice13th/QUANTA/pull/3).