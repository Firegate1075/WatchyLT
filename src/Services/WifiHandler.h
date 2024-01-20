#pragma once

class WifiHandler {
private:
    // private constructor and destructor
    WifiHandler();
    ~WifiHandler() { }

protected:
public:
    // Get singleton instance of WifiHandler class
    static WifiHandler& getInstance()
    {
        static WifiHandler instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    WifiHandler(const WifiHandler&) = delete;
    void operator=(const WifiHandler&) = delete;
};