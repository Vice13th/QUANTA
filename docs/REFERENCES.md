# References and Further Reading

This page lists primary platform documentation and background literature relevant to QUANTA2. A reference supports the design or verification process; it does not prove that the current firmware implements a cited numerical method correctly.

## 1. Arduino hardware and toolchain

1. Arduino. [Arduino UNO Rev3 documentation](https://docs.arduino.cc/hardware/uno-rev3/).
2. Arduino. [Arduino memory guide](https://docs.arduino.cc/learn/programming/memory-guide).
3. Arduino. [Arduino CLI documentation](https://docs.arduino.cc/arduino-cli/).
4. Arduino. [Arduino CLI compile command](https://docs.arduino.cc/arduino-cli/commands-reference/arduino-cli_compile).
5. Arduino. [Arduino CLI upload command](https://docs.arduino.cc/arduino-cli/commands-reference/arduino-cli_upload).
6. Arduino. [Arduino AVR Boards core](https://github.com/arduino/ArduinoCore-avr).
7. Microchip. [ATmega328P product page and datasheet](https://www.microchip.com/en-us/product/ATmega328P).

## 2. Numerical methods and quantum wave-packet computation

1. Levitt, A. [Numerical time evolution for quantum mechanics](https://www.imo.universite-paris-saclay.fr/~antoine.levitt/blog/quantum-mechanics/sec_numerical_dynamics.html).
2. [A Generalized FDTD Method with Absorbing Boundary Condition for Solving a Time-Dependent Linear Schrodinger Equation](https://doi.org/10.4236/ajcm.2012.23022) (2012).
3. [The Split-Operator Technique for the Study of Spinorial Wave Packet Dynamics](https://doi.org/10.4208/cicp.110914.281014a) (2015).
4. [The discrete Green's function method for wave packet expansion via the free Schrödinger equation](https://doi.org/10.1016/j.jcp.2024.113131) (2024).

## 3. Scientific software, reproducibility, and benchmarking

1. Task Force on Best Practices for Software Registries. [Nine Best Practices for Research Software Registries and Repositories: A Concise Guide](https://doi.org/10.48550/arXiv.2012.13117) (2020). This work discusses scope statements, user and contributor guidance, authorship, metadata, conditions of use, privacy, retention, and end-of-life policies.
2. Bartz-Beielstein, T., et al. [Benchmarking in Optimization: Best Practice and Open Issues](https://doi.org/10.48550/arXiv.2007.03488) (2020). The survey emphasizes clearly specified problems, suitable measures, thoughtful analysis, comprehensible reporting, and reproducibility.

## 4. GitHub repository and supply-chain practices

1. GitHub. [Best practices for repositories](https://docs.github.com/en/repositories/creating-and-managing-repositories/best-practices-for-repositories).
2. GitHub. [About repository README files](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/about-readmes).
3. GitHub. [About citation files](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/about-citation-files).
4. GitHub. [Setting guidelines for repository contributors](https://docs.github.com/en/communities/setting-up-your-project-for-healthy-contributions/setting-guidelines-for-repository-contributors).
5. GitHub. [Security hardening for GitHub Actions](https://docs.github.com/en/actions/security-for-github-actions/security-guides/security-hardening-for-github-actions).
6. GitHub. [Best practices for securing your build system](https://docs.github.com/en/code-security/tutorials/implement-supply-chain-best-practices/securing-builds).
7. Contributor Covenant. [Community Code of Conduct resources](https://www.contributor-covenant.org/).

## 5. How these references are used

- Arduino and Microchip sources define the target hardware and toolchain context.
- Numerical-method sources inform the design of independent reference solvers and test cases.
- Scientific-software sources inform reproducibility, benchmarking, metadata, and transparent reporting.
- GitHub sources inform repository structure, citations, contributor guidance, and workflow security.

Do not cite a numerical-method paper as evidence that QUANTA2 implements that method. Any future accuracy claim must identify the exact firmware commit, numerical method, boundary conditions, test parameters, reference implementation, and error metrics.


## Platform and CI security references

- Arduino CLI compile reference: https://docs.arduino.cc/arduino-cli/commands-reference/arduino-cli_compile
- Arduino CLI upload reference (upload does not compile first): https://docs.arduino.cc/arduino-cli/commands-reference/arduino-cli_upload
- Arduino CLI getting started: https://arduino.github.io/arduino-cli/dev/getting-started/
- Arduino AVR core and UNO board definitions: https://github.com/arduino/ArduinoCore-avr
- GitHub Actions secure use: https://docs.github.com/en/actions/reference/security/secure-use
- GitHub Actions workflow dependency lockfiles: https://docs.github.com/en/actions/concepts/security/workflow-dependency-lockfiles

Third-party actions are pinned to full commit SHAs. The compile workflow grants only `contents: read` at job level and no workflow-level token permissions.
