#include "MMC5603NJ.h"

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

MMC5603NJ MMC5603NJ(I2C_SDA_PIN, I2C_SCL_PIN, MMC5603NJ_I2C_ADDR);

void setup() {
    Serial.begin(115200);
    delay(10);
    // Begin MMC5603NJ
    MMC5603NJ.begin();
    // Read Product ID
    Serial.printf("Product ID: 0x%x\n", MMC5603NJ.readProductId());
    // Reset
    MMC5603NJ.softwareReset();

    delay(500);

    // Set Continuous Mode
    MMC5603NJ.setContinuousMode(255);
}

float magX, magY, magZ, magAbs; 
void loop() {
    //MMC5603NJ.readMag(&magX, &magY, &magZ, &magAbs);
    MMC5603NJ.getMilliGauss(&magX, &magY, &magZ, &magAbs);
    Serial.printf("Gauss X : %+9.2f, Gauss Y : %+9.2f, Gauss Z : %+9.2f, Gauss : %+9.2f\n", magX, magY, magZ, magAbs);
    delay(10);
}
