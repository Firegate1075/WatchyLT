#pragma once

class BLEHandler {
private:
    // private constructor and destructor
    BLEHandler();
    ~BLEHandler() { }

protected:
public:
    // Get singleton instance of BLEHandler class
    static BLEHandler& getInstance()
    {
        static BLEHandler instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    BLEHandler(const BLEHandler&) = delete;
    void operator=(const BLEHandler&) = delete;
};