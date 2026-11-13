# Numerical Methods and Scientific Scope

## 1. Purpose

This document distinguishes the mathematical model that motivates QUANTA2 from the finite-precision algorithm currently implemented in the firmware. The distinction is important: implementing a recognizable equation does not, by itself, establish numerical accuracy, convergence, or physical validity.

QUANTA2 is a classical embedded simulation. It is not quantum hardware and does not perform quantum computation.

## 2. Reference equation

The intended one-dimensional, time-dependent Schrödinger equation is

```text
i hbar * d(psi)/dt = [-(hbar^2 / 2m) * d^2/dx^2 + V(x)] psi
```

The source comments use dimensionless conventions:

- `hbar = 1`
- `m = 1`
- `dx = 1` grid cell
- `dt = 1/4` per evolution step
- `V(x) = pot[x] / 32`

Under these conventions, the reference Hamiltonian is

```text
H = -1/2 * d^2/dx^2 + V(x)
```

A constant energy offset changes only the global phase of an exact wavefunction and does not change position probabilities. The source comments describe an energy-zero shift by one. This convention should be preserved and documented when comparing the firmware with an independent solver.

## 3. Complex state and intended boundary conditions

The wavefunction is represented as

```text
psi(x,t) = u(x,t) + i v(x,t)
```

where the firmware stores `u` in `re[128]` and `v` in `im[128]`. The potential is stored in `pot[128]`.

The packet initializer sets both components to zero at the first and last grid cells. This corresponds to fixed, hard-wall endpoint values in the intended finite grid. The consequences of those boundaries, including reflected wave packets, must be included in reference comparisons.

The initial packet is Gaussian-shaped and includes a phase factor controlled by the momentum parameter. Its amplitude is scaled toward a discrete squared norm of approximately `5 × 10^7`. This is an implementation scale, not a physical unit calibration.

## 4. Source-level time update

The `step()` routine updates the real component first and the imaginary component second. In simplified source form:

1. For each interior cell, calculate a neighbor contribution from `im[x-1]` and `im[x+1]`, apply the local integer potential correction, and subtract the result from `re[x]`.
2. For each interior cell, calculate a neighbor contribution from the now-updated real array, apply the local integer potential correction, and add the result to `im[x]`.

The implementation uses signed integer arrays, fixed-point-like shifts, rounding offsets, and in-place updates. It does not compute a simultaneous floating-point update of both components.

**Important:** this source-level description is not a proof that the implemented update is an exact Crank–Nicolson, split-operator, or unitary method. The integration scheme, phase error, stability region, and conservation properties have not yet been independently established. The broad normalization alarm in the firmware is a fault indicator, not a numerical proof.

## 5. Probability sampling and post-selection

The `measure()` routine computes discrete weights

```text
w[x] = re[x]^2 + im[x]^2
```

It sums the weights, draws a pseudo-random integer threshold, and selects the first cell whose cumulative weight exceeds that threshold. This is inspired by discrete sampling from the Born position distribution.

After selecting a cell, the firmware initializes a new Gaussian packet centered at that position. It does not apply a rigorously derived projection operator to the pre-measurement state. Accordingly, the behavior should be described as **Born-inspired discrete sampling followed by Gaussian reinitialization**, not as a complete physical measurement model.

The pseudo-random generator mixes Watchdog interrupt timing with the Timer1 counter. No entropy rate, bias, correlation, or min-entropy analysis has been performed. It is not a certified hardware random-number generator and must not be used for cryptography or security-sensitive decisions.

## 6. Momentum-space visualization

The `kspace()` routine:

- samples 64 values from the 128-cell spatial arrays, using every second cell;
- reorders the samples using six-bit bit reversal;
- performs iterative radix-2 butterfly stages;
- scales intermediate values to reduce overflow risk; and
- converts approximate complex magnitudes into OLED bars.

This is a compact visualization. Before interpreting it quantitatively, verify frequency-bin ordering, the effect of spatial down-sampling, normalization, amplitude scaling, aliasing, and comparison against a trusted FFT implementation.

## 7. Required verification

A scientific validation effort should include, at minimum:

| Test | What it should establish |
|---|---|
| Free Gaussian packet | Propagation, dispersion, phase, and boundary behavior against an analytical or trusted reference |
| Norm tracking | Per-step and long-run change in `sum(|psi|^2)` |
| Time-step sensitivity | Whether reducing the time step changes the solution in the expected way |
| Grid sensitivity | Whether spatial refinement produces a convergent result |
| Potential step and barrier | Reflection/transmission trends and quantitative agreement with reference cases |
| Harmonic oscillator | Energy and wave-packet behavior against known solutions |
| FFT comparison | Bin order, scaling, magnitude, and error against a trusted implementation |
| Integer-range analysis | Overflow, truncation, rounding, and saturation behavior |
| Random sampling | Bias and correlation of the pseudo-random sequence and sampling distribution |

For every comparison, record the reference method, grid, time step, boundary conditions, parameter values, software version, error metric, and raw output.

## 8. Claims policy

Until the tests above have been completed and published:

- Do not claim that the firmware is unitary, convergent, or norm-preserving.
- Do not describe its OLED graph as a quantitatively calibrated probability or momentum distribution.
- Do not describe the pseudo-random generator as a true-random or cryptographic source.
- Do not infer scientific validation from a successful Arduino compile or firmware upload.

For the current implementation and resource budget, see the [Technical Overview](TECHNICAL_OVERVIEW.md). For the evidence log and test procedures, see the [Validation Plan](VALIDATION.md).

## 9.1 Sampling and watchdog implementation note

Discrete sampling now uses rejection sampling rather than direct modulo reduction, avoiding modulo-reduction bias under a uniform 32-bit input assumption. The Watchdog is configured in interrupt-only mode (WDIE=1, WDE=0); WDIE is not automatically cleared in this mode. The timing-mixed generator remains uncharacterized and must not be described as physical entropy or a certified random source. A 2026-11-12 hardware diagnostic observed approximately 642–644 interrupts per 5-second millis() window with WDTCSR=0x40; this timing differs from the nominal 16 ms datasheet timeout and remains unresolved pending identification/measurement of the exact MCU and watchdog oscillator.
