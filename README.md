# QUANTA2 — Embedded Quantum Wave Simulation Laboratory

[![Arduino UNO build](https://github.com/Vice13th/QUANTA2/actions/workflows/arduino-compile.yml/badge.svg)](https://github.com/Vice13th/QUANTA2/actions/workflows/arduino-compile.yml)
[![MIT License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

QUANTA2 is an experimental **classical numerical simulation** of one-dimensional quantum wave-packet behavior on an ATmega328P-class Arduino UNO R3 with a 128×64 I²C OLED. It is not a quantum computer, contains no qubits, and does not perform quantum computation.

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

| Area | Status |
|---|---|
| Canonical eight-scenario source | Present in repository |
| Existing main-branch CI compile | Passed for the recorded main commit |
| Expanded four-snapshot CI | Passed on PR head; post-merge main run separately tracked |
| Physical board upload | Supplied report records upload of a different source snapshot |
| Exact flash readback / binary identity | Not independently verified |
| OLED, button, buzzer runtime | Not yet verified by a physical test record |
| FPS, soak test, stack high-water | Not measured in supplied evidence |
| Numerical accuracy and convergence | Not independently validated |
| Android companion | Roadmap only |

The supplied installation report lists 12,204 bytes Flash and 1,564 bytes static SRAM (484 bytes nominally remaining). These are historical reported figures, not fresh measurements of every source snapshot. Static SRAM does not measure runtime stack high-water usage. See [Deployment Evidence](docs/DEPLOYMENT_EVIDENCE.md) and [Validation](docs/VALIDATION.md).

## Documentation

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
