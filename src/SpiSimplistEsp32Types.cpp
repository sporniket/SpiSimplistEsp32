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
#include "SpiSimplistEsp32Types.hpp"
#include "SpiSimplistEsp32Builder.hpp"

//========================<[ SpiSimplistEsp32 ]>========================
SpiSimplistEsp32::~SpiSimplistEsp32() {}
// write code here...

SpiSimplistEsp32Builder *SpiSimplistEsp32::define() { return new SpiSimplistEsp32Builder(); }

//========================<[ HostToDevicePreTransactionListenerMapping ]>========================
HostToDevicePreTransactionListenerMapping::HostToDevicePreTransactionListenerMapping(SpiIdentifier idHost,
                                                                                     SpiIdentifier idDevice,
                                                                                     transaction_cb_t listener)
    : idHost(idHost), idDevice(idDevice), listener(listener) {}
