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

#ifndef SPI_SIMPLIST_ESP32TYPES_HPP
#define SPI_SIMPLIST_ESP32TYPES_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes


class HostToDevicePreTransactionListenerMapping {
    private:
    SpiIdentifier idHost;
    SpiIdentifier idDevice;
    transaction_cb_t listener;

    public:
    HostToDevicePreTransactionListenerMapping(SpiIdentifier idHost, SpiIdentifier idDevice, transaction_cb_t listener);
    SpiIdentifier getIdHost() { return idHost; }
    SpiIdentifier getIdDevice() { return idDevice; }
    transaction_cb_t getListener() { return listener; }
};

class SpiSimplistEsp32Builder;

/** @brief What the class is for.
 */
class SpiSimplistEsp32 {
    private:
    std::map<SpiDeviceOfHostIdentifier, spi_device_handle_t> deviceHandlesOfSpi2;

    public:
    virtual ~SpiSimplistEsp32();
    static SpiSimplistEsp32Builder *define();
    void registerDevice(SpiIdentifier idHost, SpiIdentifier idDevice, spi_device_handle_t device) {
        deviceHandlesOfSpi2[SpiIdentifierHelper::deviceOfHostIdFromIdHostIdDevice(idHost, idDevice)] = device;
    }
    spi_device_handle_t getDevice(SpiIdentifier idHost, SpiIdentifier idDevice) {
        SpiDeviceOfHostIdentifier id = SpiIdentifierHelper::deviceOfHostIdFromIdHostIdDevice(idHost, idDevice);
        return (deviceHandlesOfSpi2.count(id) > 0) ? deviceHandlesOfSpi2[id] : nullptr;
    }
};

#endif