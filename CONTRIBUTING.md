# Contributing to QUANTA2

Thank you for considering a contribution. QUANTA2 welcomes focused improvements to embedded engineering, numerical verification, hardware compatibility, reproducible builds, and technical documentation.

Before contributing, please review the [Code of Conduct](CODE_OF_CONDUCT.md), [Technical Overview](docs/TECHNICAL_OVERVIEW.md), [Numerical Methods](docs/NUMERICAL_METHODS.md), and [Validation Plan](docs/VALIDATION.md).

## 1. Open an issue first

For substantial changes, open an issue before starting implementation. Describe the problem, expected behavior, constraints, and a practical way to verify the proposed change.

For hardware reports, include the board revision, OLED controller and I2C address, wiring, power source, and observed behavior. For build reports, include the commit, toolchain versions, command, and complete compiler output.

## 2. Pull request expectations

A pull request should:

1. Explain the problem and intended change.
2. Keep the diff focused and reviewable.
3. Preserve the ATmega328P resource constraints unless a target change is intentional.
4. Include a successful Arduino UNO build for firmware changes.
5. Include hardware evidence when pins, peripherals, timing, or power behavior change.
6. Include independent reference tests when the numerical kernel changes.
7. Update the relevant documentation and changelog.
8. Distinguish implemented behavior from proposed work and verified results.

## 3. Numerical and scientific changes

For changes to the simulation kernel, document:

- the mathematical equation and numerical method;
- units, grid spacing, time step, and boundary conditions;
- integer scaling, rounding, overflow, and saturation behavior;
- the reference implementation and test parameters;
- error metrics and raw comparison results; and
- known limitations and cases not tested.

Do not claim unitarity, convergence, conservation, physical accuracy, or validated randomness without appropriate evidence. A successful compile is not a scientific validation result.

## 4. Embedded engineering

- Avoid dynamic allocation unless its memory impact is justified.
- Review Flash, static SRAM, stack, and interrupt usage.
- Keep large constant tables in program memory where appropriate.
- Treat data shared with interrupt handlers carefully.
- Document new dependencies and pin their versions where reproducibility requires it.
- Do not upload firmware automatically in routine CI.

## 5. Documentation style

Write documentation in clear, concise English. Prefer task-oriented headings, copyable commands, explicit prerequisites, and tables for pin mappings or test evidence. Link to authoritative primary sources. Label statements as implemented, verified, or planned when the distinction matters.

Do not invent hardware test results, benchmark numbers, citations, maintainers, or release metadata.

## 6. Local build

Install the AVR platform and compile from the repository root:

```bash
arduino-cli core update-index
arduino-cli core install arduino:avr@1.8.8
arduino-cli compile --clean --warnings all --fqbn arduino:avr:uno firmware/QUANTA2
```

See the [Reproducibility Guide](docs/REPRODUCIBILITY.md) for evidence requirements. Uploading is a separate, deliberate operation.

## 7. Review process

Maintainers may request revisions, additional evidence, or a narrower scope before merging. Contributions that introduce unsupported scientific claims, undocumented hardware assumptions, or unacceptable memory risks may be deferred until those concerns are addressed.


## Firmware variants and evidence

- Treat `firmware/QUANTA2/` as the canonical current sketch.
- Preserve historical and installation snapshots under `firmware/archive/`; never silently substitute one variant for another.
- Compile the canonical sketch and all preserved snapshots independently.
- Update the firmware variant and deployment evidence documents whenever source identity changes.
- Separate source inspection, compile, upload, physical hardware behavior, and scientific reference validation in reports.
