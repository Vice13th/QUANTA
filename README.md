# QUANTA2 - Embedded Quantum Wave Simulation Laboratory

[![Arduino compile](https://github.com/Vice13th/QUANTA2/actions/workflows/arduino-compile.yml/badge.svg)](https://github.com/Vice13th/QUANTA2/actions/workflows/arduino-compile.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform: Arduino UNO](https://img.shields.io/badge/Platform-Arduino%20UNO-blue.svg)](https://docs.arduino.cc/hardware/uno-rev3/)

QUANTA2 is an experimental embedded-computing project that visualizes selected one-dimensional quantum-mechanics concepts on an Arduino UNO R3. It combines a discretized complex wavefunction, configurable potential landscapes, probability-based position sampling, a compact momentum-space transform, and a 128x64 I2C OLED interface.

> **Scientific scope:** QUANTA2 is a classical numerical simulation running on a conventional microcontroller. It is not a quantum computer, does not contain qubits, and does not perform quantum computation.

## Project at a glance

| Item | Current implementation |
|---|---|
| Target board | Arduino UNO R3 |
| MCU | ATmega328P, 16 MHz |
| Spatial grid | 128 cells |
| Wavefunction | Separate signed 16-bit real and imaginary arrays |
| Potential | 128 signed 8-bit samples |
| Display | 128x64 I2C OLED |
| Momentum view | 64-point radix-2 FFT visualization |
| Experiments | 8 generated potential scenarios |
| Main source | [firmware/QUANTA2/QUANTA2.ino](firmware/QUANTA2/QUANTA2.ino) |
| License | MIT |

## Features

- Eight experiment scenarios: tunneling, resonance, quantum well, potential step, harmonic oscillator, Bragg crystal, disorder, and quantum bouncer.
- Gaussian wave-packet initialization.
- Integer-based, finite-difference-inspired time evolution.
- Probability-density and potential-landscape rendering.
- A 64-point FFT-based momentum-space view.
- Cumulative probability sampling inspired by the Born rule.
- Measurement counters and left/right probability indicators.
- Optional button and buzzer.
- Watchdog/Timer1 timing-state mixing for the pseudo-random generator.

Feature descriptions reflect the source code. They do not imply that every numerical or physical behavior has passed independent scientific validation.

## Hardware

Required:
- Arduino UNO R3 or compatible ATmega328P board.
- 128x64 I2C OLED module compatible with the firmware initialization sequence.

Optional:
- Momentary push button.
- Small buzzer.

| Component | Connection |
|---|---|
| OLED SDA | A4 |
| OLED SCL | A5 |
| Button | D2 to GND |
| Buzzer | D9 |
| Built-in status/detector LED | D13 |

The firmware probes I2C addresses `0x3C` and `0x3D`. See the [Hardware Guide](docs/HARDWARE.md) before wiring or troubleshooting. The exact OLED controller and module compatibility must be checked on the physical display.

## Getting started

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) or [Arduino CLI](https://docs.arduino.cc/arduino-cli/).
2. Connect the UNO over USB.
3. Open `firmware/QUANTA2/QUANTA2.ino` from this repository.
4. Select **Arduino UNO** and the correct serial port.
5. Compile, then upload.
6. Connect the OLED to A4/A5 and power-cycle the board.

Command-line build:

```bash
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli compile --fqbn arduino:avr:uno firmware/QUANTA2
```

Upload (replace the port with the one shown on your computer):

```bash
arduino-cli upload --fqbn arduino:avr:uno --port COM8 firmware/QUANTA2
```

For Linux/macOS, the port may look like `/dev/ttyACM0` or `/dev/ttyUSB0`. Uploading is deliberately separate from compilation; do not upload until the target board and port have been checked.

## Controls

When the optional button is connected to D2:
- **Short press:** advance/trigger the measurement cycle.
- **Long press:** switch between wave and momentum visualization.

The optional buzzer on D9 provides a short detector sound. D13 indicates detector activity and blinks if neither configured OLED address is detected.

## Documentation

- [Technical Overview](docs/TECHNICAL_OVERVIEW.md) - source-level architecture, numerical model, data flow, and limitations.
- [Hardware Guide](docs/HARDWARE.md) - wiring, display detection, startup, and troubleshooting.
- [Validation Plan](docs/VALIDATION.md) - evidence, open questions, and test procedures.
- [Roadmap](docs/ROADMAP.md) - proposed next engineering milestones.
- [References](docs/REFERENCES.md) - hardware and numerical-method background.
- [Support](SUPPORT.md) - where to start when you need help.
- [Contributing](CONTRIBUTING.md) - contribution and review expectations.
- [Security Policy](SECURITY.md) - responsible vulnerability reporting.
- [Changelog](CHANGELOG.md) - repository changes.

## Memory and performance

The supplied installation/build report recorded:
- Flash: **12,204 bytes** (approximately 37%).
- SRAM: **1,564 bytes** (approximately 76%).
- Reported remaining SRAM: **484 bytes**.

A clean local build of the repository source reproduced these figures. The remaining SRAM margin is limited. Stack usage, runtime stability, and worst-case behavior must be measured before expanding the firmware.

## Validation status

The supplied installation report records a successful Arduino UNO compile/upload on COM8. It identifies an installed source snapshot with SHA-256:

```text
162BB11166AD2BF11EFBE75F37F1EB868AF1A15500C0A31EDBD1B5DC25601518
```

The firmware source file committed in this repository has SHA-256:

```text
DA1425220B12157306CBC7D2AB1C21C7A37277791A3F571760887E1DA57993AF
```

These source snapshots are not byte-identical. The repository source was compiled separately, but the installation report does not prove that this repository copy was uploaded to the physical board.

A clean local compile was performed with Arduino CLI 1.5.1 and Arduino AVR Boards core 1.8.8. It succeeded: 12,204 bytes Flash and 1,564 bytes SRAM, leaving 484 bytes. The strict-warning build showed four unused-parameter warnings in the AVR core's `new.cpp` and the low-memory stability warning. No warning originating in the QUANTA2 sketch was shown.

The repository includes a CI compile workflow. Hardware and scientific validation remain separate activities; see the [Validation Plan](docs/VALIDATION.md).

## Scientific and engineering limitations

- The current sequential integer update has not been demonstrated to be unitary or norm-preserving.
- The measurement routine samples from discrete `|psi(x)|^2` weights, then reconstructs a Gaussian packet; it is a simplified measurement-inspired model, not a full measurement-operator implementation.
- The FFT display is a compact visualization and needs reference comparison before quantitative interpretation.
- Watchdog/Timer1 mixing is not a certified entropy source and is not suitable for cryptography.
- OLED controller compatibility is module-dependent.
- The ATmega328P has only 2 KB SRAM; current reported usage leaves a narrow margin.

Please avoid describing QUANTA2 as a validated quantum simulator until the tests in the validation plan have been completed and their results published.

## Contributing

Issues and pull requests are welcome, especially for numerical verification, embedded-memory optimization, hardware compatibility, reproducible builds, and documentation. Please read [CONTRIBUTING.md](CONTRIBUTING.md) first.

## License

This project is distributed under the MIT License. See [LICENSE](LICENSE).

---

**Project status:** Experimental embedded prototype. Firmware upload is reported successful; hardware and numerical validation are in progress.
