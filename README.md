# QUANTA2 â€” Embedded Quantum Wave Simulation Laboratory

[![Arduino UNO build](https://github.com/Vice13th/QUANTA2/actions/workflows/arduino-compile.yml/badge.svg)](https://github.com/Vice13th/QUANTA2/actions/workflows/arduino-compile.yml)
[![MIT License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Target](https://img.shields.io/badge/Target-Arduino%20UNO%20R3-00979D.svg)](https://docs.arduino.cc/hardware/uno-rev3/)

QUANTA2 is an experimental embedded-computing project that visualizes selected one-dimensional quantum-mechanics concepts on an Arduino UNO R3. It combines a compact complex-valued wave-packet model, configurable potential landscapes, discrete probability sampling, a 64-point momentum-space transform, and a 128 Ã— 64 IÂ²C OLED interface.

> **Scientific scope:** QUANTA2 is a classical numerical model running on a conventional microcontroller. It is not a quantum computer, contains no qubits, and does not perform quantum computation. The numerical implementation is experimental and has not yet been independently validated for quantitative scientific use.

## Project status



| Area | Current status |
|---|---|
| Firmware source | Published in this repository |
| Arduino UNO build | Reproduced locally; CI workflow configured |
| Firmware upload | A previous installation report records a successful upload of a different source snapshot |
| OLED, button, buzzer | Hardware behavior still requires documented verification |
| Numerical accuracy and stability | Not yet independently validated |
| Android companion | Proposed roadmap item; not part of this repository |

## What is implemented

- Eight generated experiment scenarios: tunneling, resonance, quantum well, potential step, harmonic oscillator, Bragg crystal, disorder, and quantum bouncer.
- Gaussian wave-packet initialization over a 128-cell spatial grid.
- Integer-based, sequential finite-difference-inspired time evolution.
- Potential-landscape and wave-amplitude visualization.
- A 64-point radix-2 FFT visualization using every second spatial sample.
- Discrete position sampling weighted by the computed squared amplitude.
- Measurement counters and left/right display indicators.
- Optional push-button and buzzer interaction.
- Watchdog/Timer1 timing-state mixing for a non-cryptographic pseudo-random generator.

These descriptions are based on source inspection. They are not claims that the numerical method, probability interpretation, FFT scaling, or random-number quality has passed scientific validation.

## Hardware

### Required

- Arduino UNO R3 or compatible ATmega328P board.
- 128 Ã— 64 IÂ²C OLED module compatible with the firmware's initialization sequence.

### Optional

- Momentary push button.
- Small buzzer.

| Component | Arduino connection |
|---|---|
| OLED SDA | A4 |
| OLED SCL | A5 |
| OLED VCC / GND | Follow the module's voltage specification |
| Button | D2 to GND |
| Buzzer | D9 and GND |
| Built-in status/detector LED | D13 |

The firmware probes IÂ²C addresses `0x3C` and `0x3D`. The source uses a 400 kHz IÂ²C clock and SSD1306-style initialization commands. Do not assume that every SSD1306 or SH1106 breakout is compatible; verify the exact controller, module voltage, and column offset. See the [Hardware Guide](docs/HARDWARE.md).

## Quick start

### 1. Install the toolchain

Install [Arduino CLI](https://docs.arduino.cc/arduino-cli/) and the AVR platform. The reproducible baseline used for the repository's local build is Arduino CLI 1.5.1 with Arduino AVR Boards 1.8.8.

```bash
arduino-cli core update-index
arduino-cli core install arduino:avr@1.8.8
```

### 2. Compile for Arduino UNO R3

Run this command from the repository root:

```bash
arduino-cli compile --clean --fqbn arduino:avr:uno firmware/QUANTA2
```

### 3. Upload deliberately

First identify the board and serial port:

```bash
arduino-cli board list
```

Then replace `COM8` with the port reported on your system:

```bash
arduino-cli upload --fqbn arduino:avr:uno --port COM8 firmware/QUANTA2
```

On Linux, a port may look like `/dev/ttyACM0` or `/dev/ttyUSB0`. Compilation and upload are separate operations; compiling does not program the board. Review the [Reproducibility Guide](docs/REPRODUCIBILITY.md) before reporting a build result.

## Controls

When the optional button is connected between D2 and GND:

- **Short press:** advance the current measurement/evolution cycle.
- **Long press:** switch between wave and momentum views.

The optional buzzer on D9 provides a short detector click. D13 indicates detector activity and blinks when neither configured OLED address responds.

## Architecture and documentation

| Document | Purpose |
|---|---|
| [Technical Overview](docs/TECHNICAL_OVERVIEW.md) | Firmware components, state representation, and runtime flow |
| [Numerical Methods](docs/NUMERICAL_METHODS.md) | Governing equation, source-level update, assumptions, and numerical limitations |
| [Hardware Guide](docs/HARDWARE.md) | Wiring, power precautions, display detection, and troubleshooting |
| [Validation Plan](docs/VALIDATION.md) | Existing evidence, acceptance criteria, and outstanding tests |
| [Reproducibility Guide](docs/REPRODUCIBILITY.md) | Pinned toolchain, clean builds, and evidence capture |
| [Roadmap](docs/ROADMAP.md) | Proposed engineering milestones |
| [References](docs/REFERENCES.md) | Primary platform documentation and scientific-software references |
| [Contributing](CONTRIBUTING.md) | Issue, review, testing, and documentation expectations |
| [Code of Conduct](CODE_OF_CONDUCT.md) | Community participation standards |
| [Security Policy](SECURITY.md) | Responsible vulnerability reporting |
| [Support](SUPPORT.md) | How to request help |
| [Citation](CITATION.cff) | Machine-readable software citation metadata |
| [Changelog](CHANGELOG.md) | Repository changes |

## Memory budget

The repository-source clean build recorded:

| Resource | Used | Total | Remaining |
|---|---:|---:|---:|
| Flash | 12,204 bytes | 32,256 bytes | 20,052 bytes |
| Static SRAM | 1,564 bytes | 2,048 bytes | 484 bytes |

The remaining SRAM margin is limited. Static SRAM usage is not a measurement of runtime stack high-water usage. Stack, interrupt, and worst-case runtime behavior must be measured on the target before adding substantial features.

## Validation and evidence

The repository source was clean-compiled locally with Arduino CLI 1.5.1 and Arduino AVR Boards 1.8.8. The build succeeded. The strict-warning build reported four unused-parameter warnings in the AVR core's `new.cpp` and the compiler's low-memory warning; no warning originating in the QUANTA2 sketch was shown.

A previous installation report records a successful upload on COM8, but its installed source hash differs from the source currently committed here. Therefore, this repository does **not** claim that the exact current GitHub source has been uploaded to or verified on the physical board.

A successful build is not proof of successful hardware operation or scientific validity. The current numerical update has not been shown to be unitary or norm-preserving; the measurement routine reinitializes a Gaussian packet after sampling; the FFT display is not calibrated as a momentum probability density; and the timing-mixed PRNG is not a certified entropy source. See [Validation Plan](docs/VALIDATION.md) and [Numerical Methods](docs/NUMERICAL_METHODS.md).

## Contributing

Focused contributions are welcome in numerical verification, memory optimization, hardware compatibility, reproducible builds, and documentation. Please read [CONTRIBUTING.md](CONTRIBUTING.md) and the [Code of Conduct](CODE_OF_CONDUCT.md) before opening a pull request.

## License

QUANTA2 is distributed under the MIT License. See [LICENSE](LICENSE).
