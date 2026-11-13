# QUANTA2 - Engineering Checkpoint: D2 Long-Press Restart Investigation

**Checkpoint date:** 2026-11-12
**Repository:** Vice13th/QUANTA (migrated destination)
**Branch:** fix/runtime-i2c-wdt-born-sampling
**Source checkpoint commit (pre-migration):** `acb1d2b51412bcdfa8e86337adddb92054cc0a4b` (rewritten commit: `26c6a3262f9423ad7b9b9e0ef8684cb2f33b327f`)
**Source pull request:** #4 in the predecessor repository; the branch history is preserved here. Review status in the destination repository is tracked separately.
**Canonical sketch:** firmware/QUANTA2/QUANTA2.ino
**Current canonical source SHA-256:** 667077B520E0088543D3037F87A4253663B6BFE69445900E0B551E38E5F6E3DF

## 1. User-observed failure - unresolved

After the D2 long-press view-selection correction was deployed, the user tested the board and reported that a long press causes **the entire experiment to return to the first scenario, TUNNELING**, rather than merely changing the graph. The user further clarified that this happens **in place**: the display does not visibly blank and restart before the return to TUNNELING.

This is a reproduced user observation, not yet a captured instrumented trace. Do not describe the defect as fixed. The previous view-selection correction fixed only the identified phase-dependent view inversion; it did not resolve this newly clarified scenario-restart behavior.

## 2. Current production firmware and deployment state

The board was last programmed with the production firmware corresponding to commit acb1d2b (view-selection correction). Recorded build/upload evidence:

- Arduino CLI 1.5.1; Arduino AVR Boards 1.8.8; target arduino:avr:uno; board port COM8.
- Flash: 12,618 bytes.
- Static SRAM: 1,570 bytes of 2,048; nominal remainder: 478 bytes.
- Arduino CLI upload with --verify succeeded.
- Independent full Flash readback was parsed and compared with the build HEX: 12,618 application bytes, zero mismatches.
- The board has **not** been flashed with the later diagnostic experiment described below.
- The diagnostic experiment was compiled only; it was not uploaded because it raised static SRAM use to 1,747 bytes, leaving only 301 bytes nominally free and triggering the low-memory warning.

The source worktree was verified clean at this checkpoint. No firmware source edits are included in this documentation checkpoint.

## 3. Source-level control-flow facts

Inspection of the current canonical sketch established:

1. The long-press event is detected in loop() when hold == 18; the event executes only kv ^= 1.
2. The view selector in draw() is now bool sk=kv; the previous expression kv^(ph==2) was removed by commit acb1d2b.
3. sc is the scenario index. scene() selects its potential and initial packet using the current sc; scene() does not explicitly set sc to zero.
4. Normal scenario advancement occurs only in loop() after the post-measurement phase: ph=0; sc=(sc+1)&7.
5. The normalization guard in draw() can set ph=0 when the computed total norm is outside the accepted range. That causes a new scene() call, but by itself does not set sc=0; it should repeat the current scenario, not necessarily TUNNELING.
6. measure() contains a zero-total guard that sets ph=0; the caller subsequently assigns ph=2. This path should be reviewed in the broader control-flow audit, but it does not itself explain a confirmed sc=0.
7. The sketch has global mutable state and only 478 bytes nominal static SRAM headroom in the production build. Runtime stack high-water and memory-corruption behavior have not been measured.

These facts make a simple â€œthe long-press handler directly resets the scenarioâ€ explanation inconsistent with the inspected source. They do not prove the root cause.

## 4. Diagnostic build decision

A temporary Serial-instrumented build was prepared to emit boot/phase/scenario/button/measurement/normalization events. It compiled to 14,054 bytes Flash and 1,747 bytes static SRAM, leaving 301 bytes nominally free. **It was deliberately not uploaded.** Serial diagnostics are therefore not present in the board firmware, and no Serial trace exists for the reported event.

The next diagnostic should avoid a persistent Serial footprint or use a carefully bounded on-screen event/state trace. First measure the additional SRAM and stack impact, inspect generated map/stack-usage data, and test the diagnostic build offline. Do not upload a build with unassessed memory risk.

## 5. Current evidence and confidence

| Item | Status |
|---|---|
| View-selection source correction (bool sk=kv) | Implemented in acb1d2b |
| Production compile and upload verification | Passed, 12,618 B Flash |
| Independent Flash readback | Passed, 12,618 application bytes; zero mismatches |
| GitHub Actions run 43287974052 for acb1d2b | Completed successfully |
| PR #4 | Open; not merged |
| User confirmation of corrected view persistence | Superseded by new report of whole-scenario restart |
| User report of whole scenario returning to TUNNELING on D2 long press | Reported; not instrumented |
| Visible OLED blank/reboot during event | User reports no visible blank/reboot |
| sc/ph trace at failure | Not captured |
| Runtime stack high-water / SRAM corruption | Not measured |
| D2 electrical waveform / debounce characterization | Not measured |
| Numerical norm drift over time | Not characterized sufficiently to attribute this symptom |

## 6. Working hypotheses - not conclusions

Prioritize investigation of:
- unexpected mutation/corruption of sc or adjacent state;
- a phase/normalization path that re-enters scene() while the scenario index has changed or is corrupted;
- timing/state interactions around the button event and rendering workload;
- a rapid MCU reset that is not visually obvious (less supported by the user report that it occurs in place, but not formally excluded without a reset-cause trace).

Do not claim that a brownout, button bounce, watchdog, or a specific arithmetic overflow is the cause without evidence.

## 7. Ordered next actions

1. Preserve this production image and capture its current source/build identifiers before further changes.
2. Add a minimal diagnostic state recorder that can distinguish sc changes, ph transitions, bad normalization events, and MCU resets without materially increasing SRAM or stack pressure.
3. Inspect all writes to sc, ph, hold, kv, and relevant array bounds; review compiler map and stack-usage output.
4. Build and measure diagnostic and production variants with identical toolchain settings; reject diagnostics with unsafe memory margin.
5. If safe, install a reversible diagnostic image and reproduce short press, long press, and long hold; record exact event sequence.
6. Fix only the proven cause, add a regression test or repeatable hardware test, then compile, upload with verify, and independently read back Flash.
7. Re-test all eight scenarios and both display modes; update this record with evidence and retain PR #4 open until acceptance criteria pass.

## 8. Safety and scope

No firmware change or upload was performed as part of this checkpoint. Do not merge PR #4 while the D2 long-press scenario-restart defect remains unresolved. The earlier I2C, watchdog, EEPROM, and numerical limitations in REMOTE_HARDWARE_TEST_2026-11-12_FINAL.md remain open and are not resolved by this checkpoint.
