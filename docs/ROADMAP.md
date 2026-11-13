# QUANTA2 Roadmap

This roadmap separates current source functionality from proposed work. Items below are plans, not existing features.

## Phase 1 — QUANTA2.1: Verification and reliability

- Establish reproducible Arduino CLI builds.
- Add deterministic numerical test cases.
- Measure norm drift and numerical stability.
- Compare wave evolution with a desktop reference solver.
- Audit integer overflow, scaling, and boundary behavior.
- Measure stack high-water mark and SRAM headroom.
- Validate OLED controller compatibility.
- Add repeatable hardware test records.

**Exit criteria:** documented build, hardware, numerical, and long-duration results.

## Phase 2 — QUANTA3: Modular simulation engine

- Separate simulation state, numerical integration, experiments, and rendering.
- Introduce a documented numerical-integrator interface.
- Add configurable grid/time-step parameters where memory allows.
- Add reproducible experiment seeds.
- Add desktop reference tests and regression fixtures.
- Quantify display approximations separately from scientific data.

**Exit criteria:** architecture with testable numerical components and published comparison results.

## Phase 3 — Android companion (proposed)

- Define a versioned USB serial protocol.
- Add experiment selection and parameter control.
- Stream compact telemetry to an Android application.
- Export measurement history and experiment metadata.
- Add offline plots and data export.

This companion application and protocol are not part of the current firmware.

## Phase 4 — Expanded laboratory (proposed)

- Evaluate external memory or a more capable MCU.
- Explore higher-resolution grids and improved integrators.
- Add sensor/peripheral modules only when justified by the experiment.
- Preserve a clear distinction between simulation, measurement, and physical hardware.

## Prioritization

Numerical validity and resource safety take precedence over adding more visual effects or experiment names.


## Immediate prerequisite — source and deployment reconciliation

Before adding new visual features or changing the numerical engine:
- keep the canonical eight-scenario source separate from the five-experiment historical source;
- compile every preserved snapshot independently in CI;
- re-upload the selected canonical source only after recording its source hash and build report;
- verify OLED, controls, memory margin, and runtime on the actual board;
- establish deterministic desktop/reference tests before making numerical-correctness claims.

This is a verification milestone, not a claim that these tests have passed.

## Active blocker — D2 long-press scenario restart (2026-11-12)

**Priority: P0 for the current hardware acceptance path.** The user reports that a long D2 press returns the entire experiment to TUNNELING in place, without a visible display blank/reboot. The cause is unknown. The current handler only toggles kv; scenario index sc is advanced at the normal phase boundary; the normalization guard resets ph, not sc. This mismatch requires instrumentation before a source fix.

Next milestone:
1. Produce a low-footprint event trace for sc, ph, button threshold, normalization failure, and MCU reset cause.
2. Prove diagnostic SRAM/stack safety before any upload; the first Serial diagnostic left only 301 B nominal SRAM and was not installed.
3. Reproduce and isolate the failure.
4. Fix and regression-test the proven cause.
5. Verify all eight scenarios, both views, upload verification, and independent Flash readback.

Do not merge PR #4 or prioritize visual feature additions until this blocker is closed. Full details: CHECKPOINT_2026-11-12_D2_LONG_PRESS.md.
