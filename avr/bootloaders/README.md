# urboot.hex
### Feature-rich small AVR bootloader using [urprotocol](https://github.com/stefanrueger/urboot/blob/main/urprotocol.md)

See the [urboot](https://github.com/stefanrueger/urboot) repo for general information,
source code and build instructions. <br/>
See the [urboot.hex](https://github.com/stefanrueger/urboot.hex)
repo for millions(!) of pre-compiled bootloaders.

### Urboot + ATtiny13/A

|Size|Usage|Version|Features|Hex file| Bundled with MicroCore |
|:-:|:-:|:-:|:----------:|:--------------------:|:-----------:|
|252|256|u8.0|`w---jPr--`|`urboot_attiny13a.hex`| No          |
|256|256|u8.0|`w---jPr--`|`urboot_attiny13a_pr.hex`| **Yes**  |
|286|288|u8.0|`w---jPr-c`|`urboot_attiny13a_pr_ce.hex`| No    |
|314|320|u8.0|`we--jPr--`|`urboot_attiny13a_pr_ee.hex`| No    |
|352|352|u8.0|`we--jPr-c`|`urboot_attiny13a_pr_ee_ce.hex`| No |

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` bootloader provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
  + `e` EEPROM read/write support
  + `j` vector bootloader: applications *need to be patched externally*, eg, using `avrdude -c urclock`
  + `P` vector bootloader only: protects itself and reset vector from being overwritten
  + `r` preserves reset flags for the application in the register R2
  + `c` bootloader provides chip erase functionality (recommended for large MCUs)
  + `-` corresponding feature not present
- **Hex file:** often qualified by the MCU name and/or configuration
  + `pr` vector bootloader protecting the reset vector
  + `ee` bootloader supports EEPROM read/write
  + `ce` bootloader provides a chip erase command
