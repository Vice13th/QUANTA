# QUANTA2 — Embedded Quantum Wave Simulation Laboratory

[![Arduino UNO build](https://github.com/Vice13th/QUANTA/actions/workflows/arduino-compile.yml/badge.svg)](https://github.com/Vice13th/QUANTA/actions/workflows/arduino-compile.yml)
[![MIT License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

QUANTA2 is an experimental **classical numerical simulation** of one-dimensional quantum wave-packet behavior on an ATmega328P-class Arduino UNO R3 with a 128×64 I²C OLED. It is not a quantum computer, contains no qubits, and does not perform quantum computation.

## Current engineering checkpoint — 2026-11-12

**The D2 long-press scenario-restart defect is open.** After the view-persistence correction in commit acb1d2b, the user reports that holding D2 returns the entire experiment to TUNNELING without a visible display blank/reboot. The root cause is not established. Production firmware remains the verified acb1d2b image; no diagnostic image was uploaded. See [the current checkpoint](docs/CHECKPOINT_2026-11-12_D2_LONG_PRESS.md).

The view-selection correction is deployed and flash-readback verified, but it must not be treated as a complete fix for D2 behavior. The latest GitHub Actions run for acb1d2b (43287974052) passed; PR #4 remains open and unmerged. Static SRAM is 1,570 B, leaving 478 B nominally; runtime stack high-water is unknown.

## Canonical source

The current repository development source is the **eight-scenario firmware** at `firmware/QUANTA2/QUANTA2.ino`. The five-experiment Version 2.0 source and other historical snapshots are separate, archived sketches. They are not interchangeable. See [Firmware Variants](docs/FIRMWARE_VARIANTS.md).

The canonical source defines TUNNELING, RESONANCE, QUANTUM WELL, POTENTIAL STEP, OSCILLATOR, BRAGG CRYSTAL, DISORDER, and QUANTUM BOUNCER. It also contains a 64-point momentum-space FFT view, a 128-cell complex wave state, integer-based sequential time updates, discrete position sampling, statistics, and optional button/buzzer interaction. These are source-level descriptions, not scientific validation claims.

## Scientific scope and limitations

- The finite-precision update has not been established as a unitary or convergent integrator.
- Position sampling is Born-inspired; Gaussian reinitialization after sampling is not a rigorous projection-operator measurement model.
- Watchdog/Timer1 timing-state mixing is not a certified entropy source and is unsuitable for cryptography.
- FFT ordering, scaling, aliasing, and quantitative momentum interpretation require independent reference tests.
- Compile success or firmware upload does not prove numerical accuracy, OLED compatibility, or long-term stability.

See [Numerical Methods](docs/NUMERICAL_METHODS.md) and [Technical Overview](docs/TECHNICAL_OVERVIEW.md).

## Hardware

Required: Arduino UNO R3 or compatible ATmega328P board (16 MHz), and a 128×64 I²C OLED compatible with the firmware's SSD1306-style initialization.

Optional: momentary button (D2 to GND), buzzer (D9 and GND).

| Signal | UNO pin |
|---|---|
| OLED SDA / SCL | A4 / A5 |
| OLED VCC / GND | Follow the exact module ratings |
| Button | D2 to GND |
| Buzzer | D9 |
| Status LED | D13 |

The canonical firmware probes I²C addresses `0x3C` and `0x3D`. This does not guarantee compatibility with every SSD1306/SH1106 breakout. Timer1 is reconfigured; PWM on pins 9/10 and Timer1-dependent libraries such as Servo are incompatible while this firmware runs. See [Hardware](docs/HARDWARE.md).

## Reproducible build

Baseline: Arduino CLI 1.5.1 and Arduino AVR Boards 1.8.8.

```bash
arduino-cli core update-index
arduino-cli core install arduino:avr@1.8.8
arduino-cli compile --clean --fqbn arduino:avr:uno firmware/QUANTA2
```

Identify the board/port before upload:

```bash
arduino-cli board list
arduino-cli upload --verify --fqbn arduino:avr:uno --port COM8 firmware/QUANTA2
```

Replace COM8 with the detected port. Arduino CLI upload does not compile first; compile and upload must be recorded as separate steps. See [Reproducibility](docs/REPRODUCIBILITY.md).

## Evidence-based status

| Area | Status at 2026-11-12 checkpoint |
|---|---|
| Canonical eight-scenario source | Present; branch HEAD acb1d2b |
| New-repository fix-branch CI | Passed: run 43388987132 for commit `60599bc` |
| Destination review | Draft PR #3 open; not merged |
| Production firmware on COM8 | acb1d2b image uploaded with --verify |
| Independent Flash readback | 12,618 application bytes; zero mismatches |
| D2 long-press behavior | **Open defect:** user reports entire experiment returns to TUNNELING in place |
| Diagnostic image | Not uploaded; 301 B nominal SRAM remaining in diagnostic build |
| Runtime stack high-water | Not measured |
| OLED pixels / electrical waveforms | Not independently verified |
| Numerical accuracy and convergence | Not independently validated |
| Android companion | Roadmap only |

Current production build: 12,618 B Flash, 1,570 B static SRAM, 478 B nominally free. This is not a runtime stack guarantee. The earlier 12,642-byte image is a prior firmware snapshot and is not the latest deployed build. See [Deployment Evidence](docs/DEPLOYMENT_EVIDENCE.md), [Validation](docs/VALIDATION.md), and [the D2 checkpoint](docs/CHECKPOINT_2026-11-12_D2_LONG_PRESS.md).

## Documentation

- [Current D2 long-press investigation checkpoint](docs/CHECKPOINT_2026-11-12_D2_LONG_PRESS.md)
- [Firmware variants and provenance](docs/FIRMWARE_VARIANTS.md)
- [Deployment evidence](docs/DEPLOYMENT_EVIDENCE.md)
- [Technical overview](docs/TECHNICAL_OVERVIEW.md)
- [Numerical methods](docs/NUMERICAL_METHODS.md)
- [Hardware guide](docs/HARDWARE.md)
- [Validation plan](docs/VALIDATION.md)
- [Reproducibility](docs/REPRODUCIBILITY.md)
- [Roadmap](docs/ROADMAP.md)
- [References](docs/REFERENCES.md)
- [Contributing](CONTRIBUTING.md)
- [Security](SECURITY.md)
- [Support](SUPPORT.md)
- [Citation](CITATION.cff)
- [Changelog](CHANGELOG.md)

## License

MIT. See [LICENSE](LICENSE).

## Latest hardware checkpoint (2026-11-12)

The latest verified production image recorded in the predecessor-repository hardware report was built from pre-migration commit `acb1d2b` (rewritten commit `26c6a326`): 12,618 application bytes, Arduino CLI --verify passed, and independent full Flash readback found zero mismatches. This image includes the view-persistence correction, but the user subsequently reported that D2 long press restarts the entire experiment to TUNNELING in place. The cause is unresolved. The earlier I2C bus recovery and 36,194-transfer soak evidence remain valid for the earlier tested image; do not conflate that 12,642-byte image with the latest 12,618-byte image. See docs/REMOTE_HARDWARE_TEST_2026-11-12_FINAL.md and docs/CHECKPOINT_2026-11-12_D2_LONG_PRESS.md.
