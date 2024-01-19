#pragma once

class BMA456 {
private:
    // private constructors
    BMA456();
    BMA456(const BMA456&);
    ~BMA456() { }
    void operator=(const BMA456&);

protected:
public:
    // Get singleton instance of BMA456 object
    static BMA456& getInstance()
    {
        static BMA456 instance;
        return instance;
    }
    BMA456(const BMA456&) = delete;
    void operator=(const BMA456&) = delete;

    void updateDisplay();
    void handleButtons();
};