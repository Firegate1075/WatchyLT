#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <bma456w.h>

typedef enum {
    ORIENTATION_ERROR,
    ORIENTATION_TOP_EDGE_UP,
    ORIENTATION_BOTTOM_EDGE_UP,
    ORIENTATION_LEFT_EDGE_UP,
    ORIENTATION_RIGHT_EDGE_UP,
    ORIENTATION_DISP_UP,
    ORIENTATION_DISP_DOWN,
} orientation;

typedef enum {
    ACTIVITY_STATIONARY,
    ACTIVITY_WALKING,
    ACTIVITY_RUNNING,
    ACTIVITY_INVALID,
    ACTIVITY_ERROR
} activity;

#define BMA423x_INT1_PIN 14
#define BMA423x_INT2_PIN 12
#define BMA423x_INT1_MASK (1 << BMA423x_INT1_PIN)
#define BMA423x_INT2_MASK (1 << BMA423x_INT2_PIN)

class BMA456 {
private:
    // private constructor and destructor
    BMA456();
    ~BMA456() { }

    uint8_t __address;
    uint8_t __RTCTYPE;
    uint16_t __IRQ_MASK;
    bool __init;
    struct bma4_dev bma;

    int8_t BMA456_read_i2c(uint8_t dev_addr, uint8_t reg_addr, uint8_t* reg_data, uint16_t count);
    int8_t BMA456_write_i2c(uint8_t dev_addr, uint8_t reg_addr, uint8_t* reg_data, uint16_t count);
    static void bma4xx_hal_delay_usec(uint32_t period_us, void* intf_ptr);
    static int8_t bma4xx_hal_i2c_bus_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t length, void* intf_ptr);
    static int8_t bmi4xx_hal_i2c_bus_write(uint8_t reg_addr, const uint8_t* reg_data, uint32_t length, void* intf_ptr);

    bool setInterruptEnable(uint8_t interruptPin, uint16_t int_map, bool enable);

protected:
public:
    // Get singleton instance of BMA456 class
    static BMA456& getInstance()
    {
        static BMA456 instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    BMA456(const BMA456&) = delete;
    void operator=(const BMA456&) = delete;

    bool init(); // Same as original but requires an RTCType from WatchyRTC or SmallRTC.

    void softReset(); // Same as original.
    void enablePowerSave(); // Same as original.
    void disablePowerSave(); // Same as original.
    bool selfTest(); // Same as original.

    orientation getOrientation(); // Same as original except it is orientated to show the proper higher edge on your Watchy.
    bool IsUp(); // Returns True if your Watchy is in the Tilt position (with flexible room).

    bool setAccelerometerConfig(struct bma4_accel_config& cfg); // Same as original.
    bool getAccelerometerConfig(struct bma4_accel_config& cfg); // Same as original.
    bool getAcceleration(struct bma4_accel& acc); // Same as original with the exception that it inverts the x and y axes on the necessary RTCType.
    bool getAccelerometerEnable(); // Same as original.
    bool disableAccelerometer(); // Same as original.
    bool enableAccelerometer(); // Same as original.

    bool setINTPinConfig(struct bma4_int_pin_config config, uint8_t pinMap); // Same as original.
    bool getINT(); // Same as original.
    bool isStepCounter(); // Same as original.
    bool isDoubleClick(); // Same as original.  Can be used AFTER didBMAWakeUp(wakeupBit) to determine if this is true or not.
    bool isTilt(); // Same as original.  Can be used AFTER didBMAWakeUp(wakeupBit) to determine if this is true or not.
    bool isActivity(); // Same as original.  Can be used AFTER didBMAWakeUp(wakeupBit) to determine if this is true or not.
    bool isAnyNoMotion(); // Same as original.  Can be used AFTER didBMAWakeUp(wakeupBit) to determine if this is true or not.
    bool didBMAWakeUp(uint64_t hwWakeup); // Allows you to tell via wakeupBit, if the BMA woke the Watchy, if it did, it reads the reason so you can use the above 4 functions.

    bool setStepCounterWatermark(uint16_t level);
    bool resetStepCounter(); // Same as original.
    uint32_t getStepCounter(); // Same as original.

    float readTemperature(); // Same as original.
    float readTemperatureF(); // Fixed to allow for 0C to be an actual temperature.

    struct bma4_err_reg getErrorCode(); // Same as original.
    uint16_t getInterruptFlags(); // Same as original.
    uint32_t getSensorTime(); // Same as original.

    activity getActivity(); // Same as original.
    bool remapAxes(struct bma4_remap* remap_data);

    bool enableFeature(uint8_t feature, uint8_t enable);
    bool setStepCountInterrupt(bool en = true);
    bool setWristInterrupt(bool en = true);
    bool setAnyMotionInterrupt(bool en = true);
    bool setNoMotionInterrupt(bool en = true);
    bool setErrorInterrupt(bool en = true);
    bool setActivityInterrupt(bool en = true); // Same as original.

    bool defaultConfig(); // This is the default Configuration settings removed from Watchy::_bmaConfig(), corrected based on needs of RTCType.  _bmaConfig() should only consist of the begin() call and after that, the defaultConfig().
    bool enableDoubleClickWake(bool en = true); // Enables/Disables DoubleClick and the Wake Interrupt
    bool enableTiltWake(bool en = true); // Enables/Disables Tilt and the Wake Interrupt

    static float lsb_to_ms2(int16_t val, float g_range, uint8_t bit_width);
    int8_t bma4_interface_i2c_init(struct bma4_dev* bma);
};

// TODO: implement single/double tap interrupts in bma456w driver