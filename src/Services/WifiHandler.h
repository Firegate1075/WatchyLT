#pragma once

class WifiHandler {
private:
    // private constructors
    WifiHandler();
    WifiHandler(const WifiHandler&);
    ~WifiHandler() { }
    void operator=(const WifiHandler&);

protected:
public:
    // Get singleton instance of WifiHandler object
    static WifiHandler& getInstance()
    {
        static WifiHandler instance;
        return instance;
    }
    WifiHandler(const WifiHandler&) = delete;
    void operator=(const WifiHandler&) = delete;

    void updateDisplay();
    void handleButtons();
};