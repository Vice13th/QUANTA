# Reproducibility Guide

## 1. Purpose

This guide defines how to reproduce the repository's Arduino UNO compile and how to report results without confusing compilation, upload, hardware operation, and scientific validation.

## 2. Reference build environment

The repository's recorded local build used:

| Component | Version / setting |
|---|---|
| Arduino CLI | 1.5.1 |
| Arduino AVR Boards | 1.8.8 |
| Board FQBN | `arduino:avr:uno` |
| Sketch directory | `firmware/QUANTA2` |
| Build mode | Clean compile with all warnings enabled |

The CI workflow uses the same Arduino AVR platform version and a pinned GitHub Actions commit for the build action. A CI result demonstrates that the workflow completed in its runner environment; it does not demonstrate hardware operation.

## 3. Install the toolchain

```bash
arduino-cli version
arduino-cli core update-index
arduino-cli core install arduino:avr@1.8.8
arduino-cli core list
```

Record the actual CLI and platform versions from the output. If the requested platform version is no longer available from the configured package index, do not silently substitute another version; document the alternative and treat it as a different build environment.

## 4. Clean compile

Run from the repository root:

```bash
arduino-cli compile --clean --warnings all --fqbn arduino:avr:uno firmware/QUANTA2
```

The command compiles the sketch for the UNO target. It does not upload firmware. Preserve the complete output, including warnings and memory usage.

## 5. Identify and upload to the board

First list the connected boards:

```bash
arduino-cli board list
```

After confirming the target and serial port, upload explicitly:

```bash
arduino-cli upload --fqbn arduino:avr:uno --port COM8 firmware/QUANTA2
```

Replace `COM8` with the actual port. Upload is a separate operation; the Arduino CLI upload command does not compile the sketch first. Never infer the target port from an earlier session.

## 6. Capture build evidence

For each reported build, record:

- repository URL and commit SHA;
- firmware source SHA-256;
- Arduino CLI version;
- Arduino AVR platform version;
- board FQBN;
- exact compile command;
- complete compiler output;
- Flash and static SRAM usage;
- host operating system; and
- date/time of the build.

For each upload or hardware test, additionally record the board revision, port, OLED controller and I²C address, wiring, power source, test duration, observed behavior, and supporting logs or photographs.

## 7. Distinguish evidence levels

| Evidence | What it demonstrates | What it does not demonstrate |
|---|---|---|
| Source inspection | What the committed code appears to implement | Correct runtime behavior |
| Successful compile | The source compiled for the specified toolchain and target | Upload, peripheral operation, or scientific validity |
| Successful upload | A binary was transferred to the selected board | Correct display, stability, or numerical accuracy |
| Hardware test | The recorded hardware behavior under stated conditions | General compatibility with all modules or boards |
| Reference comparison | Agreement for the tested cases and stated metrics | Universal correctness outside the tested domain |

## 8. Reproducibility checklist

- [ ] Record the exact commit and source hash.
- [ ] Use a clean build.
- [ ] Record exact CLI and platform versions.
- [ ] Preserve warnings and memory reports.
- [ ] Identify the board and port before upload.
- [ ] Separate compile, upload, hardware, and numerical results.
- [ ] Publish test parameters, raw outputs, and comparison methods.
- [ ] Do not mark unperformed tests as passed.
