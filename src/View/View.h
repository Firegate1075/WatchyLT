#pragma once

class View {
private:
    // private constructors
    View();
    ~View() { }

protected:
public:
    // Get singleton instance of View class
    static View& getInstance()
    {
        static View instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    View(const View&) = delete;
    void operator=(const View&) = delete;

    void updateDisplay();
    void handleButtons();
};