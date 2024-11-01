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

// header include
#include "SpiSimplistEsp32Builder.hpp"

SpiSimplistEsp32Builder::~SpiSimplistEsp32Builder() {}
// write code here...

static const char *TAG_SPI_SIMPLIST_ESP32_BUILDER = "SpiSimplistEsp32Builder";

SpiSimplistEsp32 *SpiSimplistEsp32Builder::build() {
    SpiSimplistEsp32 *result = new SpiSimplistEsp32();
    // SPI2 aka legacy HSPI
    if (hostSpecs.count(SPI2_HOST) > 0) {
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32_BUILDER, "will build SPI2_HOST...");
        buildHost(result, SPI2_HOST);
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32_BUILDER, "DONE build SPI2_HOST...");
    }
    // SPI3 aka legacy VSPI
    if (hostSpecs.count(SPI3_HOST) > 0) {
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32_BUILDER, "will build SPI3_HOST...");
        buildHost(result, SPI3_HOST);
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32_BUILDER, "DONE build SPI3_HOST...");
    }

    return result;
}

void SpiSimplistEsp32Builder::buildHost(SpiSimplistEsp32 *spi, SpiIdentifier idHost) {
    esp_err_t ret;
    SpiHostSpecs *host = hostSpecs[idHost];
    SpiSimplistEsp32BusConfigAsHostSpecs* extra = (SpiSimplistEsp32BusConfigAsHostSpecs*) host->getExtraSpecs() ;
    spi_bus_config_t buscfg = {.mosi_io_num = host->getSerialPins()->getDataOut(),
                               .miso_io_num = host->getSerialPins()->getDataIn(),
                               .sclk_io_num = host->getSerialPins()->getClock(),
                               .quadwp_io_num = -1,
                               .quadhd_io_num = -1,
                               .data4_io_num = -1,
                               .data5_io_num = -1,
                               .data6_io_num = -1,
                               .data7_io_num = -1,
                               .max_transfer_sz = extra->getMaxTransfertSize(), // 4092
                               .flags = SPICOMMON_BUSFLAG_MASTER,
                               .isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO,
                               .intr_flags = 0};
    ESP_LOGV(TAG_SPI_SIMPLIST_ESP32_BUILDER, "will spi_bus_initialize...");
    ret = spi_bus_initialize((spi_host_device_t)idHost, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    ESP_LOGV(TAG_SPI_SIMPLIST_ESP32_BUILDER, "will browse device...");
    for (std::map<SpiIdentifier, SpiDeviceForHostSpecs *>::iterator it = host->getDevices()->begin();
         it != host->getDevices()->end(); ++it) {
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32_BUILDER, "Preparing device #%d...", it->first);
        SpiDeviceForHostSpecs *device = it->second;
        SpiSimplistEsp32DeviceInterfaceConfigSpecs *extra =
                (SpiSimplistEsp32DeviceInterfaceConfigSpecs *)device->getExtraSpecs();
        spi_device_interface_config_t *devcfg = new spi_device_interface_config_t;
        devcfg->command_bits = 0;
        devcfg->address_bits = 0;
        devcfg->dummy_bits = 0;
        devcfg->mode = device->getClockMode();
        devcfg->duty_cycle_pos = 0; // default to 50/50
        devcfg->cs_ena_pretrans = 0;
        devcfg->cs_ena_posttrans = 0;
        devcfg->clock_speed_hz = (int)device->getClockFrequency();
        devcfg->clock_source = SPI_CLK_SRC_DEFAULT ;
        devcfg->input_delay_ns = 0;
        devcfg->spics_io_num = device->getSelectPin();
        devcfg->flags = extra->getFlags() ;
        devcfg->queue_size = extra->getQueueSize() ;
        SpiDeviceOfHostIdentifier id =
                SpiIdentifierHelper::deviceOfHostIdFromIdHostIdDevice(host->getId(), device->getId());
        if (hostToDevicePreTransactionListeners.count(id) > 0) {
            devcfg->pre_cb = hostToDevicePreTransactionListeners[id];
        } else {
            devcfg->pre_cb = nullptr;
        }
        if (hostToDevicePostTransactionListeners.count(id) > 0) {
            devcfg->post_cb = hostToDevicePostTransactionListeners[id];
        } else {
            devcfg->post_cb = nullptr;
        }
        spi_device_handle_t deviceHandle;
        ret = spi_bus_add_device((spi_host_device_t)idHost, devcfg, &deviceHandle);
        spi->registerDevice(idHost, device->getId(), deviceHandle);
    }
}
