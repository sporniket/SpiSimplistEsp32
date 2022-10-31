// Copyright 2022 David SPORN
// ---
// This file is part of 'SpiSimplistEsp32'.
// ---
// 'SpiSimplistEsp32' is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// 'SpiSimplistEsp32' is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
// Public License for more details.

// You should have received a copy of the GNU General Public License along
// with 'SpiSimplistEsp32'. If not, see <https://www.gnu.org/licenses/>. 

#ifndef SPI_SIMPLIST_ESP32SPECS_HPP
#define SPI_SIMPLIST_ESP32SPECS_HPP

// standard includes
#include <cstdint>
#include <map>

// esp32 includes
#include "driver/spi_master.h"

// project includes
#include "SpiSimplist.hpp"
#include "SpiSimplistEsp32Types.hpp"

/**
 * @brief Specifications of ESP32 specific SPI settings in spi_bus_config_t for a host, with a fluent syntax.
 *
 * Put this specification in a SpiHostSpecs as an extra specs.
 *
 * By default, defines a maximum transfert size of 4KiB.
 *
 */
class SpiSimplistEsp32BusConfigAsHostSpecs {
    private:
    int maxTransfertSize = 0; // 4092
    public:
    virtual ~SpiSimplistEsp32BusConfigAsHostSpecs();
    SpiSimplistEsp32BusConfigAsHostSpecs *withMaxTransfertSize(int value) {
        maxTransfertSize = value;
        return this;
    }

    int getMaxTransfertSize() { return maxTransfertSize; }
};

/**
 * @brief Specifications of ESP32 specific SPI settings in spi_device_interface_config_t, with a fluent syntax.
 *
 * Put this specification in a SpiDeviceForHostSpecs as an extra specs.
 *
 */
class SpiSimplistEsp32DeviceInterfaceConfigSpecs {
    private:
    uint32_t flags = 0;
    int queueSize = 0;

    public:
    virtual ~SpiSimplistEsp32DeviceInterfaceConfigSpecs();
    SpiSimplistEsp32DeviceInterfaceConfigSpecs *withFlags(uint32_t value) {
        flags = value;
        return this;
    }
    SpiSimplistEsp32DeviceInterfaceConfigSpecs *withQueueSize(int value) {
        queueSize = value;
        return this;
    }
    uint32_t getFlags() { return flags; }
    int getQueueSize() { return queueSize; }
};

#endif