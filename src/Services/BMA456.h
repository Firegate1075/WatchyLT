#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <bma456w.h>

enum class BMA456_orientation {
    ERROR,
    TOP_EDGE_UP,
    BOTTOM_EDGE_UP,
    LEFT_EDGE_UP,
    RIGHT_EDGE_UP,
    DISP_UP,
    DISP_DOWN,
};

enum class BMA456_activity {
    STATIONARY,
    WALKING,
    RUNNING,
    INVALID,
    ERROR
};

enum class BMA456_interrupt { // does not handle simultanious interrupts
    NONE,
    STEP_CNTR,
    ACTIVITY,
    WRIST_WEAR,
    ANY_MOT,
    NO_MOT,
    ERROR_INT, // bma456 error interrupt
    ERROR, // interrupt while retrieving interrupt
};

class BMA456 {
private:
    // private constructor and destructor
    BMA456();
    ~BMA456() { }

    struct bma4_dev bma;
    uint8_t PIN_BMA_INT1;
    uint8_t PIN_BMA_INT2;
    uint8_t BMA456_DEVICE_ADDR;

    int8_t BMA456_read_i2c(uint8_t dev_addr, uint8_t reg_addr, uint8_t* reg_data, uint16_t count);
    int8_t BMA456_write_i2c(uint8_t dev_addr, uint8_t reg_addr, uint8_t* reg_data, uint16_t count);
    static void bma4xx_hal_delay_usec(uint32_t period_us, void* intf_ptr);
    static int8_t bma4xx_hal_i2c_bus_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t length, void* intf_ptr);
    static int8_t bmi4xx_hal_i2c_bus_write(uint8_t reg_addr, const uint8_t* reg_data, uint32_t length, void* intf_ptr);

    bool setInterruptEnable(uint8_t interruptPin, uint16_t int_map, bool enable);
    bool setFeatureEnable(uint8_t feature, bool enable);

    bool setAccelerometerConfig(struct bma4_accel_config& cfg);
    bool getAccelerometerConfig(struct bma4_accel_config& cfg);

    bool remapAxes(struct bma4_remap* remap_data);
    bool setINTPinConfig(struct bma4_int_pin_config config, uint8_t pinMap);

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

    void softReset();
    void enablePowerSave();
    void disablePowerSave();
    bool selfTest();

    BMA456_orientation getOrientation(); // Same as original except it is orientated to show the proper higher edge on your Watchy.
    bool IsUp(); // Returns True if your Watchy is in the Tilt position (with flexible room).

    bool getAcceleration(struct bma4_accel& acc);
    bool getAccelerometerEnable();
    bool disableAccelerometer();
    bool enableAccelerometer();

    bool enableStepCounter();
    bool disableStepCounter();
    bool setStepCounterWatermark(uint16_t level);
    bool resetStepCounter();
    uint32_t getStepCounter();

    bool enableActivityDetection();
    bool disableActivityDetection();

    bool enableWristWearDetection();
    bool disableWristWearDetection();

    float readTemperature(); // Same as original.
    float readTemperatureF(); // Fixed to allow for 0C to be an actual temperature.

    struct bma4_err_reg getErrorCode();
    uint32_t getSensorTime();

    BMA456_activity getActivity();

    BMA456_interrupt getInterrupt();
    bool setStepCountInterruptEnable(bool en = true);
    bool setWristInterruptEnable(bool en = true);
    bool setAnyMotionInterruptEnable(bool en = true);
    bool setNoMotionInterruptEnable(bool en = true);
    bool setErrorInterruptEnable(bool en = true);
    bool setActivityInterruptEnable(bool en = true);

    static float lsb_to_ms2(int16_t val, float g_range, uint8_t bit_width);
    int8_t bma4_interface_i2c_init(struct bma4_dev* bma);
};

// TODO: implement single/double tap interrupts in bma456w driver