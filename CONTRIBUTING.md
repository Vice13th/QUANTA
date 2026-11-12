# Contributing to QUANTA2

Thank you for your interest in QUANTA2. Contributions are welcome in embedded engineering, numerical verification, hardware compatibility, testing, and documentation.

## Before opening an issue

- Search existing issues.
- Include board and display details for hardware problems.
- Include the firmware commit and build environment for build problems.
- Distinguish observed behavior from interpretation.

## Pull requests

A good pull request should:

1. Explain the problem and the proposed change.
2. Keep changes focused and reviewable.
3. Preserve the ATmega328P memory constraints unless the target is intentionally changed.
4. Include build evidence for firmware changes.
5. Include hardware evidence when peripherals or timing are affected.
6. Include numerical reference tests for changes to the simulation kernel.
7. Update documentation when behavior, wiring, controls, or limitations change.

## Numerical-code expectations

- State the mathematical method being implemented.
- Document units, discretization, boundary conditions, and scaling.
- Do not claim unitarity, convergence, physical accuracy, or conservation without test evidence.
- Compare representative cases against an independent reference.
- Report known limitations and failure modes.

## Style and maintainability

- Prefer clear names and comments for non-obvious numerical operations.
- Avoid dynamic allocation on the constrained target unless justified.
- Keep constant tables in program memory when appropriate.
- Treat interrupt-shared state carefully.
- Avoid introducing dependencies without documenting their versions and purpose.

## Local build

```bash
arduino-cli core install arduino:avr
arduino-cli compile --fqbn arduino:avr:uno .
```

Do not upload automatically as part of a routine CI build. Hardware upload tests must be deliberate and target the intended board and port.
