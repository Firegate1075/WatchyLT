#include "BMA456.h"

#define DEBUGPORT Serial
#ifdef DEBUGPORT
#define DEBUG(...) DEBUGPORT.printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

BMA456::BMA456()
{
}

/// @brief Initializes BMA456 after reset
/// @return success of initialization
/// @retval true -> initialization successfull
/// @retval false -> initialization failed
bool BMA456::init(uint8_t pinInt1, uint8_t pinInt2)
{
    PIN_BMA_INT1 = pinInt1;
    PIN_BMA_INT2 = pinInt2;

    bma.intf = BMA4_I2C_INTF;
    bma.variant = BMA45X_VARIANT;

    if (bma4_interface_i2c_init(&bma) != BMA4_OK) {
        DEBUG("BMA456 INTERFACE FAIL\n");
        return false;
    }

    if (bma456w_init(&bma) != BMA4_OK) {
        DEBUG("BMA456 INIT FAIL\n");
        return false;
    }

    softReset();

    if (bma456w_write_config_file(&bma) != BMA4_OK) {
        DEBUG("BMA456 WRITE CONFIG FAIL\n");
        return false;
    }

    bma4_delay_us_fptr_t(20);

    struct bma4_int_pin_config config;
    config.edge_ctrl = BMA4_LEVEL_TRIGGER;
    config.lvl = BMA4_ACTIVE_HIGH;
    config.od = BMA4_PUSH_PULL;
    config.output_en = BMA4_OUTPUT_ENABLE;
    config.input_en = BMA4_INPUT_DISABLE;

    if (setINTPinConfig(config, BMA4_INTR1_MAP) == false) {
        DEBUG("BMA456 INT PIN CONFIG FAIL\n");
        return false;
    }

    struct bma4_accel_config cfg;
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    cfg.range = BMA4_ACCEL_RANGE_2G;
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;

    if (setAccelerometerConfig(cfg) == false) {
        DEBUG("BMA456 ACCELEROMETER CONFIG FAIL\n");
        return false;
    }

    if (enableAccelerometer() == false) {
        DEBUG("BMA456 ACCELEROMETER ENABLE FAIL\n");
        return false;
    }

    struct bma4_remap remap_data;
    remap_data.x = BMA4_Y;
    remap_data.y = BMA4_X;
    remap_data.z = BMA4_NEG_Z;
    if (remapAxes(&remap_data) == false) {
        DEBUG("BMA456 AXES REMAP FAIL\n");
        return false;
    }

    DEBUG("BMA456 SUCCESS\n");
    return true;
}

/// @brief executes soft reset of BMA456
void BMA456::softReset()
{
    bma4_soft_reset(&bma);
}

/// @brief turns on low power mode
void BMA456::enablePowerSave()
{
    bma4_set_advance_power_save(BMA4_DISABLE, &bma);
}

/// @brief turns off low power mode
void BMA456::disablePowerSave()
{
    bma4_set_advance_power_save(BMA4_ENABLE, &bma);
}

/// @brief reads error code from BMA456
/// @return struct containing error information
struct bma4_err_reg BMA456::getErrorCode()
{
    struct bma4_err_reg error;
    bma4_get_error_status(&error, &bma);
    return error;
}

/// @brief retrieves uptime of BMA456
/// @return uptime of BMA456 (1LSB = 39.25us)
uint32_t BMA456::getSensorTime()
{
    uint32_t time;
    bma4_get_sensor_time(&time, &bma);
    return time;
}

/// @brief performs self test of BMA456
/// @return result of selftest
/// @retval true -> selftest passed
/// @retval false -> selftest failed
bool BMA456::selfTest()
{
    int8_t rslt = BMA4_SELFTEST_FAIL;
    bma4_perform_accel_selftest(&rslt, &bma);
    return (rslt == BMA4_SELFTEST_PASS);
}

/// @brief computes orientation of Watchy
/// @return orientation_t of Watchy
/// @retval ORIENTATION_ERROR -> failed to get acceleration
orientation_t BMA456::getOrientation()
{
    struct bma4_accel acc;
    if (!getAcceleration(acc))
        return ORIENTATION_ERROR;
    uint16_t absX = abs(acc.x);
    uint16_t absY = abs(acc.y);
    uint16_t absZ = abs(acc.z);

    if ((absZ > absX) && (absZ > absY)) {
        return ((acc.z < 0) ? ORIENTATION_DISP_DOWN : ORIENTATION_DISP_UP);
    } else if ((absY > absX) && (absY > absZ)) {
        return ((acc.y < 0) ? ORIENTATION_BOTTOM_EDGE_UP : ORIENTATION_TOP_EDGE_UP);
    } else {
        return ((acc.x < 0) ? ORIENTATION_LEFT_EDGE_UP : ORIENTATION_RIGHT_EDGE_UP);
    }
}

bool BMA456::IsUp() // TODO!
{
    struct bma4_accel acc;
    if (!getAcceleration(acc))
        return false;
    // TODO check if this works now
    return (acc.x <= 0 && acc.x >= -700 && acc.y >= -300 && acc.y <= 300 && acc.z <= -750 && acc.z >= -1070);
}

/// @brief reads temperature data from BMA456
/// @return temperature in °C
/// @retval -105 -> no valid temperature available
float BMA456::readTemperature()
{
    int32_t tempData = 0;
    bma4_get_temperature(&tempData, BMA4_DEG, &bma);
    float temperature = (float)tempData / 1000;

    return temperature;
}

/// @brief reads temperature data from BMA456
/// @return temperature in °F
/// @retval -157 -> no valid temperature available
float BMA456::readTemperatureF()
{
    int32_t tempData = 0;
    bma4_get_temperature(&tempData, BMA4_FAHREN, &bma);
    float temperature = tempData / 1000;
    return temperature;
}

/// @brief get acceleration data
/// @param acceleration reference to bma4_accel struct to place acceleration data in
/// @return success of reading acceleration
/// @retval true -> success
/// @retval false -> error while reading acceleration
bool BMA456::getAcceleration(struct bma4_accel& acceleration)
{
    memset(&acceleration, 0, sizeof(acceleration)); // clear acceleration data
    if (bma4_read_accel_xyz(&acceleration, &bma) != BMA4_OK) {
        return false;
    }

    return true;
}

/// @brief get status of accelerometer
/// @return enable status of accelerometer
/// @retval true -> enabled
/// @retval false -> disabled
bool BMA456::getAccelerometerEnable()
{
    uint8_t en;
    bma4_get_accel_enable(&en, &bma);
    return (bool)en;
}

/// @brief disable accelerometer
/// @return success
/// @retval true -> success
/// @retval false -> error
bool BMA456::disableAccelerometer()
{
    return (BMA4_OK == bma4_set_accel_enable(BMA4_DISABLE, &bma));
}

/// @brief enable accelerometer
/// @return success
/// @retval true -> success
/// @retval false -> error
bool BMA456::enableAccelerometer()
{
    return (BMA4_OK == bma4_set_accel_enable(BMA4_ENABLE, &bma));
}

bool BMA456::enableStepCounter()
{
    return false;
}

bool BMA456::disableStepCounter()
{
    return false;
}

/// @brief upload accelerometer config to BMA456
/// @param cfg bma4_accel_config structure containing configuration data for BMA456 accelerometer
/// @return success
/// @retval true -> success
/// @retval false -> error
bool BMA456::setAccelerometerConfig(struct bma4_accel_config& cfg)
{
    return (BMA4_OK == bma4_set_accel_config(&cfg, &bma));
}

/// @brief get accelerometer config to BMA456
/// @param cfg bma4_accel_config structure where configuration data for BMA456 accelerometer is written
/// @return success
/// @retval true -> success
/// @retval false -> error
bool BMA456::getAccelerometerConfig(struct bma4_accel_config& cfg)
{
    return (BMA4_OK == bma4_get_accel_config(&cfg, &bma));
}

/// @brief remap the axes of BMA456 to align with Watchy coordinate system
/// @param remap_data bma4_remap structure structure containing data for remapping
/// @return success
/// @retval true -> success
/// @retval false -> error
bool BMA456::remapAxes(struct bma4_remap* remap_data)
{
    return (BMA4_OK == bma456w_set_remap_axes(remap_data, &bma));
}

/// @brief set the level for step counter watermark interrupt
/// @param level level of steps that triggers interrupt (1LSB = 20 steps)
/// @return success
/// @retval true -> success
/// @retval false -> error
bool BMA456::setStepCounterWatermark(uint16_t level)
{
    return (BMA4_OK == bma456w_step_counter_set_watermark(level, &bma));
}

/// @brief reset the step counter to zero
/// @return success
/// @retval true -> success
/// @retval false -> error
bool BMA456::resetStepCounter()
{
    return BMA4_OK == bma456w_reset_step_counter(&bma);
}

/// @brief get step counter value
/// @return step count
/// @retval step counter value if successfull
/// @retval returns 0 on error
uint32_t BMA456::getStepCounter()
{
    uint32_t stepCount;
    if (bma456w_step_counter_output(&stepCount, &bma) == BMA4_OK) {
        return stepCount;
    }
    return 0;
}

/**
 * @brief enable the activity detection feature of BMA456
 *
 * @return success
 * @retval true -> success
 * @retval false -> fail
 */
bool BMA456::enableActivityDetection()
{
    return setFeatureEnable(BMA456W_STEP_ACT, true);
}

/**
 * @brief disable the activity detection feature of BMA456
 *
 * @return success
 * @retval true -> success
 * @retval false -> fail
 */
bool BMA456::disableActivityDetection()
{
    return setFeatureEnable(BMA456W_STEP_ACT, false);
}

/**
 * @brief enable the wrist wear wakeup feature of BMA456
 *
 * @return success
 * @retval true -> success
 * @retval false -> fail
 */
bool BMA456::enableWristWearDetection()
{
    return setFeatureEnable(BMA456W_WRIST_WEAR_WAKEUP, true);
}

/**
 * @brief disable the wrist wear wakeup feature of BMA456
 *
 * @return success
 * @retval true -> success
 * @retval false -> fail
 */
bool BMA456::disableWristWearDetection()
{
    return setFeatureEnable(BMA456W_WRIST_WEAR_WAKEUP, false);
}

/// @brief configure BMA456's interrupt pin
/// @param config stucture containing configuration data for interrupt pin
/// @param interruptPin specifies interrupt pin of BMA456. Use bma4 macros.
/// @return success
/// @retval true -> success
/// @retval false -> error
bool BMA456::setINTPinConfig(struct bma4_int_pin_config config, uint8_t interruptPin)
{
    return BMA4_OK == bma4_set_int_pin_config(&config, interruptPin, &bma);
}

/**
 * @brief retrieves the cause for BMA456 interrupt
 *
 * @return bma456_interrupt_t stores the reason for the triggered interrupt
 * @retval BMA456_INT_ERROR -> error while retrieving interrupt
 * @retval BMA456_INT_ERROR_INT -> interrupt due to BMA456 error
 * @retval BMA456_INT_NONE -> no interrupt triggered by BMA456
 */
bma456_interrupt_t BMA456::getInterrupt()
{
    uint16_t interruptMask;
    if (bma456w_read_int_status(&interruptMask, &bma) != BMA4_OK) {
        return BMA456_INT_ERROR;
    }

    if (interruptMask & BMA456W_STEP_CNTR_INT) {
        return BMA456_INT_STEP_CNTR;
    }
    if (interruptMask & BMA456W_ACTIVITY_INT) {
        return BMA456_INT_ACTIVITY;
    }
    if (interruptMask & BMA456W_WRIST_WEAR_WAKEUP_INT) {
        return BMA456_INT_WRIST_WEAR;
    }
    if (interruptMask & BMA456W_ANY_MOT_INT) {
        return BMA456_INT_ANY_MOT;
    }
    if (interruptMask & BMA456W_NO_MOT_INT) {
        return BMA456_INT_NO_MOT;
    }
    if (interruptMask & BMA456W_ERROR_INT) {
        return BMA456_INT_ERROR_INT;
    }

    return BMA456_INT_NONE;
}

bool BMA456::setInterruptEnable(uint8_t interruptPin, uint16_t int_map, bool enable)
{
    if (enable == true) {
        return (BMA4_OK == bma456w_map_interrupt(BMA4_INTR1_MAP, int_map, BMA4_ENABLE, &bma));
    } else {
        return (BMA4_OK == bma456w_map_interrupt(BMA4_INTR1_MAP, int_map, BMA4_DISABLE, &bma));
    }
}

bool BMA456::setFeatureEnable(uint8_t feature, bool enable)
{
    if (enable == true) {
        return (BMA4_OK == bma456w_feature_enable(feature, BMA4_ENABLE, &bma));
    } else {
        return (BMA4_OK == bma456w_feature_enable(feature, BMA4_DISABLE, &bma));
    }
}

bool BMA456::setStepCountInterruptEnable(bool en)
{
    if (enableStepCounter() == false) {
        return false;
    }

    return setInterruptEnable(BMA4_INTR1_MAP, BMA456W_STEP_CNTR_INT, en);
}

bool BMA456::setActivityInterruptEnable(bool en)
{
    if (enableActivityDetection() == false) {
        return false;
    }

    return setInterruptEnable(BMA4_INTR1_MAP, BMA456W_ACTIVITY_INT, en);
}

bool BMA456::setWristInterruptEnable(bool en)
{
    if (enableWristWearDetection() == false) {
        return false;
    }

    return setInterruptEnable(BMA4_INTR1_MAP, BMA456W_WRIST_WEAR_WAKEUP_INT, en);
}

bool BMA456::setAnyMotionInterruptEnable(bool en)
{
    return setInterruptEnable(BMA4_INTR1_MAP, BMA456W_ANY_MOT_INT, en);
}

bool BMA456::setNoMotionInterruptEnable(bool en)
{
    return setInterruptEnable(BMA4_INTR1_MAP, BMA456W_NO_MOT_INT, en);
}

bool BMA456::setErrorInterruptEnable(bool en)
{
    return setInterruptEnable(BMA4_INTR1_MAP, BMA456W_ERROR_INT, en);
}

/**
 * @brief retrieves user activity
 *
 * @return activity_t current activity
 * @retval ACTIVITY_ERROR -> error retrieving activity
 */
activity_t BMA456::getActivity()
{
    uint8_t userActivity = -1;
    bma456w_activity_output(&userActivity, &bma);
    switch (userActivity) {
    case BMA456W_USER_STATIONARY:
        return ACTIVITY_STATIONARY;
        break;
    case BMA456W_USER_WALKING:
        return ACTIVITY_WALKING;
        break;
    case BMA456W_USER_RUNNING:
        return ACTIVITY_RUNNING;
        break;
    case BMA456W_STATE_INVALID:
        return ACTIVITY_INVALID;
        break;
    default:
        return ACTIVITY_ERROR;
        break;
    }
}

void BMA456::bma4xx_hal_delay_usec(uint32_t period_us, void* intf_ptr)
{
    delayMicroseconds(period_us);
}

/*! This API is used to perform I2C read operation with sensor */
int8_t BMA456::bma4xx_hal_i2c_bus_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t length, void* intf_ptr)
{
    BMA456& bma = getInstance();
    int8_t rslt = 0;
    // uint8_t dev_id = 0x68;
    uint8_t* dev_id = (uint8_t*)intf_ptr;

    rslt = bma.BMA456_read_i2c(*dev_id, reg_addr, reg_data, length);

    return rslt;
}

/*! This API is used to perform I2C write operations with sensor */
int8_t BMA456::bmi4xx_hal_i2c_bus_write(uint8_t reg_addr, const uint8_t* reg_data, uint32_t length, void* intf_ptr)
{
    BMA456& bma = getInstance();
    int8_t rslt = 0;
    //    uint8_t dev_id = 0x68;

    uint8_t* dev_id = (uint8_t*)intf_ptr;
    rslt = bma.BMA456_write_i2c(*dev_id, reg_addr, (uint8_t*)reg_data, length);

    return rslt;
}

/// @brief Private I2C helper function. Reads specified number of bytes from I2C device into array
/// @param dev_addr I2C address of BMA456
/// @param reg_addr starting address of data to read
/// @param reg_data pointer to array where read data is placed
/// @param count number of bytes to be read
/// @return 0: success, any other value: communication failure
int8_t BMA456::BMA456_read_i2c(uint8_t dev_addr, uint8_t reg_addr, uint8_t* reg_data, uint16_t count)
{
    // Begin I2C communication with provided I2C address
    Wire.beginTransmission(dev_addr);
    Wire.write(reg_addr);
    // Done writting, end the transmission
    int8_t status = Wire.endTransmission();

    if (status != 0) {
        return status;
    }

    // Requests the required number of bytes from the sensor
    Wire.requestFrom((int)dev_addr, (int)count);

    uint16_t i;
    // Reads the requested number of bytes into the provided array
    for (i = 0; (i < count) && Wire.available(); i++) {
        reg_data[i] = Wire.read(); // This is for the modern Wire library
    }

    // This must return 0 on success, any other value will be interpreted as a communication failure.
    return 0;
}

/// @brief Private I2C helper function. Writes specified number of bytes from array to I2C
/// @param dev_addr I2C address of BMA456
/// @param reg_addr starting address where data is written in BMA456
/// @param reg_data pointer to array where data to be written is stored
/// @param count number of bytes to be written
/// @return 0: success, any other value: communication failure
int8_t BMA456::BMA456_write_i2c(uint8_t dev_addr, uint8_t reg_addr, uint8_t* reg_data, uint16_t count)
{
    // Begin I2C communication with provided I2C address
    Wire.beginTransmission(dev_addr);
    Wire.write(reg_addr);

    uint16_t i;
    // Writes the requested number of bytes from the provided array
    for (i = 0; i < count; i++) {
        Wire.write(reg_data[i]); // This is for the modern Wire library
    }
    // Done writting, end the transmission
    int8_t returned = Wire.endTransmission();

    return returned;
}

int8_t BMA456::bma4_interface_i2c_init(struct bma4_dev* bma)
{
    int8_t rslt = BMA4_OK;

    if (bma != NULL) {

        /* Bus configuration : I2C */
        bma->intf = BMA4_I2C_INTF;
        bma->bus_read = bma4xx_hal_i2c_bus_read;
        bma->bus_write = bmi4xx_hal_i2c_bus_write;

        /* Assign device address to interface pointer */
        bma->intf_ptr = &BMA456_DEVICE_ADDR;

        /* Assign Variant */
        bma->variant = BMA45X_VARIANT;

        /* Configure delay in microseconds */
        bma->delay_us = bma4xx_hal_delay_usec;

        /* Configure max read/write length (in bytes) (Supported length depends on target machine) */
        bma->read_write_len = 16;

        /* Set Performance mode status */
        bma->perf_mode_status = BMA4_DISABLE;
    } else {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}
