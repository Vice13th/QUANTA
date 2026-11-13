# Validation Plan and Evidence

## 1. Previously reported installation evidence

The supplied installation report records:

| Check | Reported result | Evidence status |
|---|---|---|
| Arduino UNO detected on COM8 | Successful | Reported |
| ATmega328P compilation | Successful | Reported |
| Firmware upload | Successful | Reported |
| Board re-detected after upload | Successful | Reported |
| Flash usage | 12,204 bytes (~37%) | Reported build output |
| SRAM usage | 1,564 bytes (~76%) | Reported build output |
| Remaining SRAM | 484 bytes | Reported build output |

The installed source snapshot SHA-256 recorded in that report is:

```text
162BB11166AD2BF11EFBE75F37F1EB868AF1A15500C0A31EDBD1B5DC25601518
```

This is a source-file hash, not a firmware binary hash.

## 2. Repository-source clean build

A clean local compile was performed against the firmware source committed in this repository. Its source SHA-256 is:

```text
DA1425220B12157306CBC7D2AB1C21C7A37277791A3F571760887E1DA57993AF
```

The repository source hash differs from the installed snapshot hash above. The two files are not byte-identical; the installation report does not establish that this repository copy was uploaded to the physical board.

Build evidence:

```text
Command: arduino-cli compile --clean --fqbn arduino:avr:uno --warnings all firmware/QUANTA2
Arduino CLI: 1.5.1
Arduino AVR Boards core: 1.8.8
Exit code: 0 (success)
Flash: 12,204 bytes (37% of 32,256 bytes)
Static SRAM: 1,564 bytes (76% of 2,048 bytes)
Remaining SRAM: 484 bytes
```

The strict-warning build reported four `-Wunused-parameter` warnings in the installed Arduino AVR core's `new.cpp` (`tag` parameters); no warning originating in the QUANTA2 sketch was shown. The compiler also emitted its low-memory stability warning. These warnings and the narrow SRAM margin should remain visible until addressed.

This is a compile result only. No upload of this repository copy and no physical OLED/button/buzzer or numerical runtime test is claimed.

## 3. Validation categories

### A. Build reproducibility
- Compile the repository sketch for `arduino:avr:uno`.
- Record Arduino CLI version and AVR core version.
- Record Flash and static SRAM usage.
- Ensure the CI workflow passes on clean commits.

### B. Hardware bring-up
- Verify I2C address detection at 0x3C and 0x3D.
- Verify all eight display pages.
- Test SSD1306 and SH1106 modules separately if both are claimed.
- Test button short/long press behavior.
- Test buzzer and D13 behavior.
- Record power source and test duration.

### C. Numerical correctness
- Measure `sum(|psi(x)|^2)` after every evolution step.
- Compare free-packet evolution with an analytical or trusted numerical reference.
- Check time-step and grid-resolution sensitivity.
- Test tunneling and double-barrier trends against reference cases.
- Compare harmonic oscillator behavior with reference solutions.
- Verify FFT bins, frequency ordering, scaling, and magnitude against a trusted FFT.
- Quantify boundary reflections and energy drift.

### D. Reliability
- Measure stack high-water mark and SRAM margin.
- Run all eight experiment modes repeatedly.
- Perform a multi-hour soak test.
- Record resets, hangs, display faults, and normalization warnings.
- Assess random output bias/correlation before making entropy claims.

## 4. Suggested test log template

| Field | Value |
|---|---|
| Firmware commit | |
| Source SHA-256 | |
| Board model/revision | |
| OLED model/controller | |
| I2C address | |
| Arduino CLI version | |
| AVR core version | |
| Compile result | |
| Flash / SRAM | |
| Runtime duration | |
| Experiment modes tested | |
| Resets or faults | |
| Norm drift observations | |
| Evidence (logs/photos) | |

## 5. Acceptance principles

- A successful compile is a build result, not a hardware result.
- A successful upload is not proof of runtime correctness.
- A visually plausible graph is not proof of numerical accuracy.
- A normalization alarm threshold is not a proof of conservation.
- A timing-noise PRNG is not automatically a true-random or cryptographic source.

Only mark a test as passed when a repeatable procedure and evidence are recorded.


## 6. Reproducible procedure

Use the exact commands and evidence fields in the [Reproducibility Guide](REPRODUCIBILITY.md). The repository's source-level algorithm and scientific limitations are described in [Numerical Methods](NUMERICAL_METHODS.md). Keep compile, upload, hardware, and numerical results as separate records.


## 7. Firmware provenance audit (12 November 2026)

The repository and supplied files contain multiple non-identical source snapshots. The canonical repository sketch is the eight-scenario implementation. The supplied Version 2.0 document and `QUANTA2(1).ino` attachment describe a separate five-experiment implementation. A local source associated with the installation report has SHA-256 `162BB11166AD2BF11EFBE75F37F1EB868AF1A15500C0A31EDBD1B5DC25601518` and is preserved separately. The user-supplied eight-experiment source is also archived separately.

See [Firmware Variants](FIRMWARE_VARIANTS.md) and [Deployment Evidence](DEPLOYMENT_EVIDENCE.md). The expanded CI workflow compiles each snapshot independently; that does not establish which source is physically running on the board.

## 8. Evidence boundary

- New-repository `main` CI run [43388797556](https://github.com/Vice13th/QUANTA/actions/runs/43388797556) passed for commit `7312b27b82a63f9f6cb989315942cb6992446e7f`, compiling all four snapshots with Arduino CLI 1.5.1 and Arduino AVR Boards 1.8.8 for `arduino:avr:uno`.
- The now-merged remote hardware evidence PR [#2](https://github.com/Vice13th/QUANTA/pull/2) passed its four-snapshot CI run [43388853047](https://github.com/Vice13th/QUANTA/actions/runs/43388853047). The firmware reliability PR [#3](https://github.com/Vice13th/QUANTA/pull/3) also passed CI on its prior head in run [43388857958](https://github.com/Vice13th/QUANTA/actions/runs/43388857958); the later documentation-only head `60599bcfc233d8ab8ec9db8c96d668864341cd39` also passed in run [43388987132](https://github.com/Vice13th/QUANTA/actions/runs/43388987132).
- Runs `43149417021` and `43219411457` belong to the predecessor repository and are historical evidence only, not CI runs in `Vice13th/QUANTA`.
- The merged hardware report in PR #2 and the draft firmware records in PR #3 concern specific historical firmware snapshots. They do not prove that the current `main` image is installed on the board. The reports include limited flash-readback and I2C evidence, but no human visual confirmation of OLED output or confirmed physical D2/D9 actuation.
- Numerical convergence, norm conservation, FFT accuracy, PRNG quality, runtime stack high-water, and multi-hour stability are not independently validated.
## 9. Independent multi-snapshot compile results (12 November 2026)

A clean clone of the merged branch was compiled locally on Hive with Arduino CLI 1.5.1 and Arduino AVR Boards 1.8.8. All four source directories compiled successfully with exit code 0:

| Snapshot | Flash | Static SRAM | Free SRAM | Result |
|---|---:|---:|---:|---|
| Canonical eight-scenario | 12,204 B | 1,564 B | 484 B | Pass |
| Five-experiment reference | 9,430 B | 1,530 B | 518 B | Pass |
| User eight-experiment snapshot | 12,204 B | 1,564 B | 484 B | Pass |
| Reported installed snapshot | 12,204 B | 1,564 B | 484 B | Pass |

A historical GitHub Actions run in the predecessor repository (run ID `43219411457`) passed the same four-snapshot matrix. This is source-repository evidence, not a CI run in `Vice13th/QUANTA`. Its hosted Linux runner reported 12,214 B Flash for the eight-experiment sources (10 B above the local Windows result) and identical static SRAM. The small Flash discrepancy is unresolved and should be investigated before claiming byte-for-byte reproducible builds.

With `--warnings all`, the local canonical compile succeeded and showed four unused-parameter warnings in Arduino AVR core `new.cpp`; no firmware-source warning appeared in the captured output. The canonical and eight-experiment variants leave 484 B for stack/local variables and trigger Arduino CLI's low-memory warning. This is a static memory report only; no stack high-water or soak test was run.

## 10. Remaining hardware and scientific tests

No physical upload of the current `main`-branch canonical image was performed as part of this repository reconciliation. Separate hardware records under PRs #2 and #3 document uploads/readbacks of other firmware snapshots and limited I2C transaction tests. They do not establish the current `main` image identity, human-visible OLED output, physical button/buzzer actuation, FPS, reset behavior, long-duration stability, runtime stack high-water, norm drift, FFT accuracy, or agreement with a trusted numerical reference.


## 11. Remote physical-board audit (12 November 2026)

See [Remote Hardware Test](REMOTE_HARDWARE_TEST_2026-11-12.md). A read-only avrdude readback of the physical UNO on COM8 proved the running application was the five-experiment archive, not the eight-experiment source. The five-experiment HEX matched 9,430 application bytes with zero mismatches; the eight-experiment local build did not match. A temporary I²C diagnostic found one OLED ACK at 0x3C at both 100 kHz and 400 kHz. Across repeated full-frame tests, 400 kHz produced zero errors over 24 frames; 100 kHz produced five intermittent transfer errors over 20 frames, plus one initialization error. The exact five-experiment firmware was restored and post-restore flash readback matched its HEX exactly. Visual OLED output, button/buzzer operation, stack high-water, long-run stability, and numerical correctness remain unverified.


The follow-up watchdog diagnostic observed the WDT interrupt register state (`WDTCSR=0x40`) and 512 then 642 interrupts per board-reported 5,000 ms interval. This is faster than the nominal 16 ms WDP=0000 setting. Since the timing reference was the MCU's own `millis()` and the Timer1 capture was inconclusive, the result is an unresolved timing anomaly; do not claim watchdog timing calibration or entropy quality. The five-experiment firmware was restored and its final flash readback again matched exactly.


See the dated [Remote Hardware Test](REMOTE_HARDWARE_TEST_2026-11-12.md) for raw observed watchdog counts, I²C status codes, and authoritative Arduino/Microchip references.
