# Changelog

All notable repository changes are documented here.

## Unreleased

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

### QUANTA2 hardware reliability
- Added bounded I2C startup bus-clear handling for SDA-low/SCL-high recovery; the routine never actively drives bus lines HIGH and stops if SCL remains low.
- Added Wire timeout/error accounting and unbiased rejection sampling for discrete position selection.
- Recorded the 2026-11-12 COM8 upload, independent Flash readback, I2C bus recovery, 400 kHz OLED transfer soak, watchdog timing anomaly, and remaining visual/electrical acceptance gates in docs/REMOTE_HARDWARE_TEST_2026-11-12_FINAL.md.

### D2 long-press investigation checkpoint (2026-11-12)
- Recorded the user's clarified report: D2 long press returns the entire experiment to TUNNELING in place, without a visible display blank/reboot.
- Confirmed by source inspection that the long-press handler toggles only kv; normal scenario advancement changes sc, while the normalization guard resets ph but not sc.
- Recorded that the Serial diagnostic build (14,054 B Flash / 1,747 B static SRAM / 301 B nominal free) was compiled but intentionally not uploaded due to memory risk.
- Preserved the verified production image at commit acb1d2b; no firmware changes or uploads were made for this checkpoint. Root cause remains open.
- Added docs/CHECKPOINT_2026-11-12_D2_LONG_PRESS.md as the current handoff and source of truth for next actions.
