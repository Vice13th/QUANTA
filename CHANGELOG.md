# Changelog

All notable repository changes are documented here.

## Unreleased

### Remote hardware verification
- Read back the physical UNO flash and identified the running application as the five-experiment archive, not the previously reported eight-experiment image.
- Tested I²C/OLED ACK and frame writes at 100 kHz and 400 kHz; recorded intermittent 100 kHz errors and zero frame errors over 24 frames at 400 kHz.
- Restored the exact five-experiment firmware and verified its application bytes against a fresh HEX readback.
- Added a dated hardware test report and explicitly retained the unverified status of visual OLED output and physical controls.

### Source and evidence reconciliation
- Preserved the supplied five-experiment source, supplied eight-experiment source, and source associated with the reported board installation as distinct archived sketches.
- Added firmware variant and deployment evidence records with explicit source hashes.
- Expanded Arduino UNO CI to compile the canonical firmware and preserved snapshots independently.
- Tightened workflow token permissions and pinned actions/checkout to a reviewed v7.0.1 commit SHA.

### Documentation
- Reworked the README into an English project overview and practical quick-start guide.
- Added numerical-method and reproducibility guides.
- Added software citation metadata in `CITATION.cff`.
- Added a project Code of Conduct.
- Expanded references to include official GitHub/Arduino guidance and scientific-software literature.
- Clarified evidence levels and the distinction between source inspection, compilation, upload, hardware testing, and scientific validation.
- Corrected source comments that described the timing-mixed pseudo-random mechanism as physical noise.

### Engineering policy
- Documented the exact source-level update and its unverified numerical properties.
- Documented the limited SRAM headroom and the need for stack/runtime measurements.
- Standardized contributor expectations for numerical changes and reproducible evidence.

## Previous baseline

- Published the Arduino UNO firmware source and initial technical/hardware/validation documentation.
- Added a pinned Arduino UNO GitHub Actions compile workflow.
