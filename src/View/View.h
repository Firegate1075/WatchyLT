#pragma once

class View {
private:
    // private constructors
    View();
    View(const View&);
    ~View() { }
    void operator=(const View&);

protected:
public:
    // Get singleton instance of View object
    static View& getInstance()
    {
        static View instance;
        return instance;
    }
    View(const View&) = delete;
    void operator=(const View&) = delete;

    void updateDisplay();
    void handleButtons();
};