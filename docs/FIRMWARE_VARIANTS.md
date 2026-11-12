# Firmware Variants, Source Identity, and Provenance

This index identifies each source snapshot. Version labels alone do not identify a firmware binary; use the exact path, SHA-256, board target, and toolchain.

| Variant | Experiments | Repository path | Source SHA-256 / identity | Role |
|---|---:|---|---|---|
| QUANTA 1.0 | Barrier tunneling | Supplied project document Appendix A | Not archived as standalone source | Historical reference |
| QUANTA 2.0 | 5 | `firmware/archive/QUANTA2-5EXP/QUANTA2-5EXP.ino` | `11B6BFE903C3D60205DC16C599A3254E22BC38B81741522D6F2A380B3208E808` | Supplied five-experiment source |
| User eight-experiment snapshot | 8 | `firmware/archive/QUANTA2-8EXP-user-snapshot-2026-11-12/QUANTA2-8EXP-user-snapshot-2026-11-12.ino` | `32D44795CDE15476F3740B5F4132083585D54032B9CE343372791CD2807468B1` | Supplied eight-experiment source |
| Reported installed snapshot | 8 | `firmware/archive/installed-snapshot-2026-11-12/installed-snapshot-2026-11-12.ino` | `162BB11166AD2BF11EFBE75F37F1EB868AF1A15500C0A31EDBD1B5DC25601518` | Local source associated with installation report |
| Canonical repository source | 8 | `firmware/QUANTA2/QUANTA2.ino` | Git blob SHA-1 at audited base: `ffb9bfde75e7d0cfc0e0143e2469b32e51ed88a0` | Current development line |

Git blob SHA-1 is not a SHA-256 digest. Source hashes identify text files, not compiled HEX/BIN files.

## Feature boundaries

The five-experiment source defines TUNNEL, BLOCH, HARMONIC, ANDERSON, and BORN. Its BORN mode freezes a profile and accumulates a histogram. It directly targets I²C address `0x3C`.

The eight-experiment line defines TUNNELING, RESONANCE, QUANTUM WELL, POTENTIAL STEP, OSCILLATOR, BRAGG CRYSTAL, DISORDER, and QUANTUM BOUNCER. It includes a 64-point FFT view, optional D2 button and D9 buzzer, address probing at `0x3C`/`0x3D`, and a configurable display column offset.

The user-supplied eight-experiment file, local installed-source snapshot, and canonical GitHub source are retained separately. Do not assume they are byte-identical.

## Promotion policy

1. `firmware/QUANTA2/` is the only default/current development target.
2. Keep historical snapshots in `firmware/archive/`; do not silently replace the canonical sketch.
3. Each hardware deployment record must identify source SHA-256, build artifact hash, board/FQBN, toolchain, port, upload verification, and physical test evidence.
4. CI compile success is not hardware or scientific validation.
5. Update this matrix whenever a variant is added, promoted, or retired.
