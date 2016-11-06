# Milight Gateway with nRF51

*Originally based on the radio transmitter example, from nRF5 SDK 12.1*

This project is an attempt at controlling Milights using a easy to get nRF51 dongle. 

## Requirements

 - Building
   - nRF5 SDK 12.1 - *Version might not be required*
   - arm-none-eabi toolchain - *I'm using 5.4.1*
 - Testing
   - CMake - *Cmake is used for building the unit tests*
   - Check - *C Unit testing framework* 

## Getting Started

1. Go to the location of your nRF5 SDK and create a `projects` subfolder

    ```bash
    git clone https://github.com/NZSmartie/MilightGateway
    cd MilightGateway/src/nrf51822_xxaa
    make
    ``` 

    1.1. If you want to use your own bootloader, modify `ORIGIN` and `LENGTH` to suit in `src/nrf51822_xxaa/gcc_nrf51.ld`

    ```c
    # ...
    FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x40000
    # ...
    ```

2. Using Nordic's nRF Studio, download the application firmware using your prefered method.

   **Do Not** Use a SoftDevice as this code requreis direct access to the radio.

## Testing

Navigate to the root directory of this repository and:

```bash
mkdir build && cd build
cmake ..
make && ctest -V
```

There are 3 tests so far, TestCRC, TestMillight and TestUtil.

### TestCRC

Only tests the 16bit CRC poly functions (which is requried for milight)

### TestMilight

Tests the packet structure, integrity and auto increment of the packet counter.

### TestUtil

Since there's a 4bit "trailer" transmitted by PL1167 radios, It is nessesscery to shift all the nibbles over 1 following that trailer in order to preserve the LSB data transmitted over the air.

## TODO 

  1. Try to building using CMake, make life easier, ya'know.
