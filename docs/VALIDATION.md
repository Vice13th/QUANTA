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
