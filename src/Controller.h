#pragma once

class Controller {
private:
    // private constructors
    Controller();
    Controller(const Controller&);
    ~Controller() { }
    void operator=(const Controller&);

protected:
public:
    // Get singleton instance of Controller object
    static Controller& getInstance()
    {
        static Controller instance;
        return instance;
    }
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;
};