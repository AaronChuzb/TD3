# ChangeLog

## v0.0.1 - 2023-08-22

### Enhancements:

* Implement the driver for the GC9B71 LCD controller
* Support SPI and QSPI interface

## v0.0.2 - 2023-10-07

### Enhancements:

* Support to use ESP-IDF release/v5.0

## v1.0.0 - 2023-11-03

### bugfix

* Fix the incompatible dependent version of ESP-IDF
* Remove LCD command `29h` from the initialization sequence
* Add parameter `max_trans_sz` in default bus configuration macro
* Add check for conflicting commands between initialization sequence and driver

## v1.0.1 - 2023-12-04

### bugfix

* Remove unused header `hal/spi_ll.h`
