/**************************************************************************/
/*!
  @file     MMC5603NJ.cpp
  Author: Atsushi Sasaki(https://github.com/aselectroworks)
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#include "MMC5603NJ.h"

#include <Wire.h>

MMC5603NJ::MMC5603NJ(uint8_t deviceAddress) : _deviceAddress(deviceAddress) {
#ifdef MMC5603NJ_DEBUG
    DEBUG_PRINTER.begin(115200);
#endif
    DEBUG_PRINTLN("Call Contructor");
}
#if defined(ESP32) || defined(ESP8266)
MMC5603NJ::MMC5603NJ(int8_t sda, int8_t scl, uint8_t deviceAddress)
    : _sda(sda), _scl(scl), _deviceAddress(deviceAddress) {
#ifdef MMC5603NJ_DEBUG
    DEBUG_PRINTER.begin(115200);
#endif
    DEBUG_PRINTLN("Call Contructor");
}
#endif

MMC5603NJ::~MMC5603NJ() {}

void MMC5603NJ::begin() {
    DEBUG_PRINTLN("begin");
    // Initialize I2C
    if (_sda != -1 && _scl != -1) {
        Wire.begin(_sda, _scl);
    } else {
        Wire.begin();
    }
}

void MMC5603NJ::setClockSpeed(MMC5603NJ_I2C_CLOCK_SPEED speed) {
    Wire.setClock(speed);
}

float MMC5603NJ::getMilliGaussX(void) {
    int32_t mag = 0;
    mag |= readByte(MMC5603NJ_ADDR_XOUT0) << 12;
    mag |= readByte(MMC5603NJ_ADDR_XOUT1) << 4;
    mag |= readByte(MMC5603NJ_ADDR_XOUT2) << 0;
    return 0.0625 * (mag - 524288);
}
float MMC5603NJ::getMilliGaussY(void) {
    int32_t mag = 0;
    mag |= readByte(MMC5603NJ_ADDR_YOUT0) << 12;
    mag |= readByte(MMC5603NJ_ADDR_YOUT1) << 4;
    mag |= readByte(MMC5603NJ_ADDR_YOUT2) << 0;
    return 0.0625 * (mag - 524288);
}
float MMC5603NJ::getMilliGaussZ(void) {
    int32_t mag = 0;
    mag |= readByte(MMC5603NJ_ADDR_ZOUT0) << 12;
    mag |= readByte(MMC5603NJ_ADDR_ZOUT1) << 4;
    mag |= readByte(MMC5603NJ_ADDR_ZOUT2) << 0;
    return 0.0625 * (mag - 524288);
}

void MMC5603NJ::getMilliGauss(float *magX, float *magY, float *magZ, float *magAbs) {
    uint8_t mag[9] = {0};
    readMultiByte(MMC5603NJ_ADDR_XOUT0, 9, mag);
    *magX = ((mag[0] << 12 | mag[1] << 4 | mag[6]) - 524288) * 0.0625;
    *magY = ((mag[2] << 12 | mag[3] << 4 | mag[7]) - 524288) * 0.0625;
    *magZ = ((mag[4] << 12 | mag[5] << 4 | mag[8]) - 524288) * 0.0625;
    *magAbs = sqrt(pow(*magX, 2) + pow(*magY, 2) + pow(*magZ, 2));
}

void MMC5603NJ::setContinuousMode(uint8_t odr) {
    writeByte(MMC5603NJ_ADDR_ODR, odr);
    writeByte(MMC5603NJ_ADDR_INTCTRL0, 0b10100000);
    writeByte(MMC5603NJ_ADDR_INTCTRL1, 0b00000011);
    writeByte(MMC5603NJ_ADDR_INTCTRL2, 0b00010000);
}

void MMC5603NJ::clearContinuousMode(void) {
    writeByte(MMC5603NJ_ADDR_INTCTRL2, 0b00000000);
}

void MMC5603NJ::readMag(float *magX, float *magY, float *magZ, float *magAbs) {
    writeByte(MMC5603NJ_ADDR_INTCTRL0, 0b00100001);
    MMC5603NJ_STATUS1_REG status;
    do {
        status.raw = readByte(MMC5603NJ_ADDR_STATUS1);
    } while (status.meas_m_done == false);
    getMilliGauss(magX, magY, magZ, magAbs);
}

MMC5603NJ_INTCTRL0_REG MMC5603NJ::readControl0(void) {
    MMC5603NJ_INTCTRL0_REG ctrl;
    ctrl.raw = readWord(MMC5603NJ_ADDR_INTCTRL0);
    return ctrl;
}
void MMC5603NJ::writeControl0(MMC5603NJ_INTCTRL0_REG ctrl) {
    writeWord(MMC5603NJ_ADDR_INTCTRL0, ctrl.raw);
}
MMC5603NJ_INTCTRL1_REG MMC5603NJ::readControl1(void) {
    MMC5603NJ_INTCTRL1_REG ctrl;
    ctrl.raw = readWord(MMC5603NJ_ADDR_INTCTRL1);
    return ctrl;
}
void MMC5603NJ::writeControl1(MMC5603NJ_INTCTRL1_REG ctrl) {
    writeWord(MMC5603NJ_ADDR_INTCTRL1, ctrl.raw);
}
MMC5603NJ_INTCTRL2_REG MMC5603NJ::readControl2(void) {
    MMC5603NJ_INTCTRL2_REG ctrl;
    ctrl.raw = readWord(MMC5603NJ_ADDR_INTCTRL2);
    return ctrl;
}
void MMC5603NJ::writeControl2(MMC5603NJ_INTCTRL2_REG ctrl) {
    writeWord(MMC5603NJ_ADDR_INTCTRL2, ctrl.raw);
}

uint8_t MMC5603NJ::readProductId(void) {
    return readByte(MMC5603NJ_ADDR_PRODUCTID);
}

void MMC5603NJ::softwareReset(void) {
    writeByte(MMC5603NJ_ADDR_INTCTRL1, 0x80);
}

// Private Function
void MMC5603NJ::readMultiByte(uint8_t addr, uint8_t size, uint8_t *data) {
    Wire.beginTransmission(_deviceAddress);
    DEBUG_PRINTF("readI2C reg 0x%02x\n", addr)
    Wire.write(addr);
    Wire.endTransmission(false);
    Wire.requestFrom((uint16_t)_deviceAddress, size);
    for (uint8_t i = 0; i < size; ++i) {
        data[i] = Wire.read();
        DEBUG_PRINTF(" <- data[%d]:0x%02x\n", i, data[i])
    }
}
uint8_t MMC5603NJ::readByte(uint8_t addr) {
    uint8_t rxData;
    readMultiByte(addr, 1, &rxData);
    DEBUG_PRINTF("read byte = %d\n", rxData)
    return rxData;
}
void MMC5603NJ::writeMultiByte(uint8_t addr, uint8_t *data, uint8_t size) {
    Wire.beginTransmission(_deviceAddress);
    DEBUG_PRINTF("writeI2C reg 0x%02x\n", addr)
    Wire.write(addr);
    for (uint8_t i = 0; i < size; i++) {
        DEBUG_PRINTF(" -> data[%d]:0x%02x\n", i, data[i])
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}
void MMC5603NJ::writeByte(uint8_t addr, uint8_t data) {
    DEBUG_PRINTF("write byte = %d\n", data)
    writeMultiByte(addr, &data, 1);
}
