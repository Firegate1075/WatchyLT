#pragma once

class BLEHandler {
private:
    // private constructors
    BLEHandler();
    BLEHandler(const BLEHandler&);
    ~BLEHandler() { }
    void operator=(const BLEHandler&);

protected:
public:
    // Get singleton instance of BLEHandler object
    static BLEHandler& getInstance()
    {
        static BLEHandler instance;
        return instance;
    }
    BLEHandler(const BLEHandler&) = delete;
    void operator=(const BLEHandler&) = delete;

    void updateDisplay();
    void handleButtons();
};