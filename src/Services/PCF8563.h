#pragma once

class PCF8563 {
private:
    // private constructors
    PCF8563();
    PCF8563(const PCF8563&);
    ~PCF8563() { }
    void operator=(const PCF8563&);

protected:
public:
    // Get singleton instance of PCF8563 object
    static PCF8563& getInstance()
    {
        static PCF8563 instance;
        return instance;
    }
    PCF8563(const PCF8563&) = delete;
    void operator=(const PCF8563&) = delete;

    void updateDisplay();
    void handleButtons();
};