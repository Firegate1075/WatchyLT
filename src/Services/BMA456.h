#pragma once

class BMA456 {
private:
    // private constructor and destructor
    BMA456();
    ~BMA456() { }

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
};