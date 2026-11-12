# References and Further Reading

These sources provide background for the hardware platform and numerical methods. They are not evidence that the current QUANTA2 integrator has been validated.

## Hardware and toolchain

- Arduino UNO Rev3 documentation: https://docs.arduino.cc/hardware/uno-rev3/
- Arduino memory guide: https://docs.arduino.cc/learn/programming/memory-guide
- Arduino CLI documentation: https://docs.arduino.cc/arduino-cli/
- Arduino CLI compile command: https://docs.arduino.cc/arduino-cli/commands-reference/arduino-cli_compile
- Arduino AVR core: https://github.com/arduino/ArduinoCore-avr
- Microchip ATmega328P product and datasheet: https://www.microchip.com/en-us/product/ATmega328P

## Numerical methods

- Antoine Levitt, numerical time evolution notes for quantum mechanics: https://www.imo.universite-paris-saclay.fr/~antoine.levitt/blog/quantum-mechanics/sec_numerical_dynamics.html
- “A Generalized FDTD Method with Absorbing Boundary Condition for Solving a Time-Dependent Linear Schrodinger Equation” (2012): https://doi.org/10.4236/ajcm.2012.23022
- “The Split-Operator Technique for the Study of Spinorial Wave Packet Dynamics” (2015): https://doi.org/10.4208/cicp.110914.281014a
- “The discrete Green's function method for wave packet expansion via the free Schrödinger equation” (2024): https://doi.org/10.1016/j.jcp.2024.113131

## How to use these references

Use numerical-method references to design and validate an independent reference solver. Do not cite them as proof that QUANTA2 implements those methods correctly. Record the exact method, discretization, boundary conditions, and error metrics used in any future comparison.
