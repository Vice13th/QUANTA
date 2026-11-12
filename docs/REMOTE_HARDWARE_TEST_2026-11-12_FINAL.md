# QUANTA2 — Final Remote Hardware Validation (2026-11-12)

## Target and toolchain
- Remote host: Hive; Arduino enumerated as COM8, arduino:avr:uno.
- MCU signature reported by AVRDUDE: 1E 95 0F (ATmega328P-compatible signature).
- Arduino CLI 1.5.1; Arduino AVR Boards core 1.8.8; AVRDUDE 8.0-arduino.1.
- The previously open five-experiment IDE sketch was not used for upload. The canonical eight-scenario repository source was built explicitly.

## Final production image
- Source: firmware/QUANTA2/QUANTA2.ino.
- Source SHA-256: C652782B264EF2DA9CF8A6A8D6D457E35A5B9A66E918DCA8DF60A8F870FA4246.
- Clean Arduino UNO build succeeded with --warnings all.
- Flash: 12,642 bytes; static SRAM: 1,570 bytes of 2,048; nominal remainder: 478 bytes.
- HEX SHA-256: FCC809C3B0BAB9641CFD74A658751ED4BC33B83597AF0AA13C704F4E8DA18FF5.
- Production upload to COM8 with Arduino CLI --verify: exit code 0.
- Independent post-upload full Flash readback: 32,768 bytes.
- Parsed Intel HEX comparison: all 12,642 application bytes matched, zero mismatches.
- Full Flash readback SHA-256: 0EF38B2B25877C4A1E31D17ECE9078A95A46DF60311A257DE2AB6B30906A07BD.
- A temporary diagnostic image was separately built and uploaded with verify, then the final production image was restored with verify. The final readback was taken after restoration.

## I2C bus fault and recovery
The diagnostic first observed SDA=LOW, SCL=HIGH; both 0x3C and 0x3D returned Wire status 5 (timeout). This is consistent with a physically stuck bus, not an ordinary address NACK.

A safe open-drain bus-clear operation released SCL and generated one SCL pulse. SDA then rose HIGH. After recovery:
- At 100 kHz, 0x3C returned status 0 and 0x3D returned status 2 (expected address NACK).
- At 400 kHz, 0x3C returned status 0 and 0x3D returned status 2.

The same bounded startup recovery was added to production firmware. It only attempts clock pulses when SDA is low and SCL is high, never actively drives either line HIGH, and stops if SCL cannot be released. If SCL is physically held LOW, software bus-clear cannot safely force it high; inspect power, wiring, pull-ups, and the slave.

A 400 kHz OLED transfer soak completed 411 frames and 36,194 I2C transactions. Final counters:
- status 1 (buffer too long): 0
- status 2 (address NACK): 0
- status 3 (data NACK): 0
- status 4 (other TWI error): 1
- status 5 (timeout): 0

That is one transient status-4 error in 36,194 transfers (about 0.0028%), with no NACKs or timeouts. This is strong evidence of sustained bus operation, but not a zero-error result. The firmware latches I2C errors for the D13 status LED.

Wire.setWireTimeout(25000, true) resets/reinitializes the AVR TWI peripheral on a timeout; it does not itself clock a physically stuck bus clear. The production startup routine now handles the specific SDA-low/SCL-high condition.

## Watchdog observation
The diagnostic read WDTCSR=0x40, meaning WDIE=1, WDE=0, and prescaler bits zero: interrupt-only mode. Microchip documents 2,048 watchdog oscillator cycles and a typical 16 ms timeout for this prescaler. The diagnostic measured approximately 642–644 interrupts per 5-second millis() window after startup (about 7.8 ms per interrupt), faster than nominal datasheet timing. The initial window was 472 ticks.

The earlier assumption that interrupt-only mode automatically clears WDIE was incorrect; the datasheet specifies automatic WDIE clearing for combined interrupt-and-reset mode (WDE=1). No WDIE re-arm was retained in production. The observed timing discrepancy remains unresolved; the MCU physical top-marking and oscillator behavior have not been independently identified. No watchdog prescaler change was made. This timing-mixed generator is not a certified entropy source.

## EEPROM and memory caveats
- No EEPROM write operation was issued by the upload commands; the uploader wrote Flash only.
- Two post-upload EEPROM readbacks matched each other byte-for-byte.
- The post-upload EEPROM content differed at 14 byte addresses from an earlier pre-upload readback. The cause is unresolved; do not claim EEPROM contents were preserved byte-for-byte relative to that older snapshot.
- A no-LTO -fstack-usage build reports per-function static stack frames, but runtime stack high-water is not measured. The 478-byte SRAM remainder is not a guarantee of safe worst-case stack usage.

## Evidence not established
- No camera or human visual confirmation of OLED pixels was available.
- D2 button interaction and D9 buzzer audibility were not independently confirmed by a human observer.
- The single status-4 I2C event remains to be investigated with electrical instrumentation.
- No oscilloscope/logic-analyzer measurements of SDA/SCL rise time, VOL, or pull-up resistance were taken.
- Numerical convergence and physical accuracy are outside this hardware upload/communication test.

## Conclusion
The self-recovering eight-scenario production firmware is installed on COM8, AVRDUDE upload verification passed, and an independent full Flash readback matched all 12,642 application bytes with zero differences. The I2C bus was physically recovered from SDA-low using one SCL pulse, and a sustained 400 kHz OLED transaction test completed with one status-4 error and zero timeouts/NACKs. Visual and electrical acceptance remain open.
