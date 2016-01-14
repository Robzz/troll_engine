#ifndef INPUT_H
#define INPUT_H

#include <functional>
#include <map>

class InputManager {
    public:
    /* Default constructor */
    InputManager();
    /* Move constructor */
    InputManager(InputManager&& other);
    /* Destructor */
    virtual ~InputManager();

    void setKeyCallback(int key_code, std::function<void()>);
    void setMouseCallback(std::function<void(double, double)>);
    void keyCallback(int keycode, int scancode, int action, int mods) const;
    void mouseCallback(double x, double y);

    // Enable or disable Y axis inversion
    void invertY(bool invert = true);

    private:
    std::map<int, std::function<void()>> m_callbacks;    
    std::function<void(double, double)> m_mousePosCallback;
    int m_invertY;
    double m_mouseSensitivity;

    // No copy or assignment
    InputManager(InputManager const& other);
    void operator=(InputManager const& other);
};

#endif
