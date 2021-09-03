/**************************************************************************/
/*!
  @file     MMC5603NJ.h
  Author: Atsushi Sasaki(https://github.com/aselectroworks)
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#ifndef _MMC5603NJ
#define _MMC5603NJ

#include <stdbool.h>
#include <stdint.h>

#define MMC5603NJ_I2C_ADDR 0b0110000

#define MMC5603NJ_ADDR_XOUT0 0x00
#define MMC5603NJ_ADDR_XOUT1 0x01
#define MMC5603NJ_ADDR_YOUT0 0x02
#define MMC5603NJ_ADDR_YOUT1 0x03
#define MMC5603NJ_ADDR_ZOUT0 0x04
#define MMC5603NJ_ADDR_ZOUT1 0x05
#define MMC5603NJ_ADDR_XOUT2 0x06
#define MMC5603NJ_ADDR_YOUT2 0x07
#define MMC5603NJ_ADDR_ZOUT2 0x08
#define MMC5603NJ_ADDR_TOUT 0x09
#define MMC5603NJ_ADDR_STATUS1 0x18
#define MMC5603NJ_ADDR_ODR 0x1A
#define MMC5603NJ_ADDR_INTCTRL0 0x1B
#define MMC5603NJ_ADDR_INTCTRL1 0x1C
#define MMC5603NJ_ADDR_INTCTRL2 0x1D
#define MMC5603NJ_ADDR_ST_X_TH 0x1E
#define MMC5603NJ_ADDR_ST_Y_TH 0x1F
#define MMC5603NJ_ADDR_ST_Z_TH 0x20
#define MMC5603NJ_ADDR_ST_X 0x27
#define MMC5603NJ_ADDR_ST_Y 0x28
#define MMC5603NJ_ADDR_ST_Z 0x29
#define MMC5603NJ_ADDR_PRODUCTID 0x39

// STatus1 REG
#define SAT_SENSOR_BIT 5
#define OTP_READ_DONE_BIT 4
#define MEAS_T_DONE_INT_BIT 1
#define MEAS_M_DONE_INT_BIT 0
typedef union {
    uint8_t raw;
    struct {
        bool meas_m_done_int : 1;
        bool maas_t_done_int : 1;
        bool mdt_flag_int : 1;
        bool st_fail : 1;
        bool otp_read_done : 1;
        bool sat_sensor : 1;
        bool meas_m_done : 1;
        bool meas_t_done : 1;
    };
} MMC5603NJ_STATUS1_REG;
// INTERNAL CONTROL 0 REG
#define TAKE_MEAS_M_BIT 0
#define TAKE_MEAS_T_BIT 1
#define START_MDT_BIT 2
#define DO_SET_BIT 3
#define DO_RESET_BIT 4
#define AUTO_SR_EN_BIT 5
#define AUTO_ST_EN_BIT 6
#define CMM_FREQ_EN_BIT 7
typedef union {
    uint8_t raw;
    struct {
        bool take_meas_m : 1;
        bool take_meas_t : 1;
        bool start_mdt : 1;
        bool do_set : 1;
        bool do_reset : 1;
        bool auto_sr_en : 1;
        bool auto_st_en : 1;
        bool cmm_freq_en : 1;
    };
} MMC5603NJ_INTCTRL0_REG;
// INTERNAL CONTROL 1 REG
#define BW_BIT_BASE 0
#define X_INHIBIT_BIT 2
#define Y_INHIBIT_BIT 3
#define Z_INHIBIT_BIT 4
#define ST_ENP_BIT 5
#define ST_ENM_BIT 6
#define SW_RESET_BIT 7
typedef union {
    uint8_t raw;
    struct {
        uint8_t bw : 2;
        bool x_inhibit : 1;
        bool y_inhibit : 1;
        bool z_inhibit : 1;
        bool st_enp : 1;
        bool st_enm : 1;
        bool sw_reset : 1;
    };
} MMC5603NJ_INTCTRL1_REG;
// INTERNAL CONTROL 2 REG
#define PRD_SET_BIT_BASE 0
#define EN_PRD_SET_BIT 3
#define CMM_EN_BIT 4
#define INT_MDT_EN_BIT 5
#define INT_MEAS_DONE_EN_BIT 6
#define HPOWER_BIT 7
typedef union {
    uint8_t raw;
    struct {
        uint8_t prd_set : 3;
        bool en_prd_set : 1;
        bool cmm_en : 1;
        bool int_mdt_en : 1;
        bool int_meas_done_en : 1;
        bool hpower : 1;
    };
} MMC5603NJ_INTCTRL2_REG;

typedef enum {
    I2C_CLOCK_100KHZ = 100000,
    I2C_CLOCK_400KHZ = 400000,
    I2C_CLOCK_1MHZ = 1000000,
} MMC5603NJ_I2C_CLOCK_SPEED;

// Uncomment to enable debug messages
//#define MMC5603NJ_DEBUG

// Define where debug output will be printed
#include <Arduino.h>
#define DEBUG_PRINTER Serial

// Setup debug printing macros
#ifdef MMC5603NJ_DEBUG
#define DEBUG_PRINT(...)                                               \
    {                                                                  \
        DEBUG_PRINTER.printf("[MMC5603NJ(0x%02x)]: ", _deviceAddress); \
        DEBUG_PRINTER.print(__VA_ARGS__);                              \
    }
#define DEBUG_PRINTLN(...)                                             \
    {                                                                  \
        DEBUG_PRINTER.printf("[MMC5603NJ(0x%02x)]: ", _deviceAddress); \
        DEBUG_PRINTER.println(__VA_ARGS__);                            \
    }
#define DEBUG_PRINTF(...)                                              \
    {                                                                  \
        DEBUG_PRINTER.printf("[MMC5603NJ(0x%02x)]: ", _deviceAddress); \
        DEBUG_PRINTER.printf(__VA_ARGS__);                             \
    }
#else
#define DEBUG_PRINT(...) \
    {}
#define DEBUG_PRINTLN(...) \
    {}
#define DEBUG_PRINTF(...) \
    {}
#endif

/**************************************************************************/
/*!
    @brief  MMC5603NJ Magnetic Sensor driver
*/
/**************************************************************************/
class MMC5603NJ {
   public:
    MMC5603NJ(uint8_t deviceAddress);
#if defined(ESP32) || defined(ESP8266)
    MMC5603NJ(int8_t sda, int8_t scl, uint8_t deviceAddress);
#endif
    virtual ~MMC5603NJ();

    void begin();

    void setClockSpeed(MMC5603NJ_I2C_CLOCK_SPEED speed);

    float getMilliGaussX(void);
    float getMilliGaussY(void);
    float getMilliGaussZ(void);
    void getMilliGauss(float *magX, float *magY, float *magZ, float *magAbs);
    
    void setContinuousMode(uint8_t odr);
    void clearContinuousMode(void); 

    void readMag(float *magX, float *magY, float *magZ, float *magAbs); 
    float readMagX(void); 

    MMC5603NJ_INTCTRL0_REG readControl0(void);
    void writeControl0(MMC5603NJ_INTCTRL0_REG ctrl);
    MMC5603NJ_INTCTRL1_REG readControl1(void);
    void writeControl1(MMC5603NJ_INTCTRL1_REG ctrl);
    MMC5603NJ_INTCTRL2_REG readControl2(void);
    void writeControl2(MMC5603NJ_INTCTRL2_REG ctrl);

    uint8_t readProductId(void);

    void softwareReset();

   private:
    int8_t _sda = -1;
    int8_t _scl = -1;
    uint8_t _deviceAddress;  // 7-bit address

    void readMultiByte(uint8_t addr, uint8_t size, uint8_t *data);
    uint16_t readWord(uint8_t addr);
    uint8_t readByte(uint8_t addr);
    void writeMultiByte(uint8_t addr, uint8_t *data, uint8_t size);
    void writeWord(uint8_t addr, uint16_t data);
    void writeByte(uint8_t addr, uint8_t data);
};

#endif
