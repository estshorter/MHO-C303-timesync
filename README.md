# MHO-C303-timesync
This software synchronizes the time of the system with that of MHO-C303.
Tested with GCC 10 on Ubuntu 20.10 on Raspberry Pi 4 Model B.
This should work for lywsd02 too.

![mho-c303](https://raw.githubusercontent.com/custom-components/ble_monitor/master/pictures/MHO-C303.png)

(image from [ble-monitor](https://github.com/custom-components/ble_monitor))

## Dependencies
- [gattlib](https://github.com/labapart/gattlib)

## How to build
``` sh
cmake -B build -S .
cmake --build build
```

### release build
``` sh
cmake -DCMAKE_BUILD_TYPE=Release -B build -S . -GNinja
cd build && sudo ninja install
```

## References
- [lywsd02](https://github.com/h4/lywsd02)
