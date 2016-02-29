#ifndef INPUT_H
#define INPUT_H

#include <functional>
#include <map>

namespace Engine {

typedef std::function<void(double,double)> MousePositionCallback;
typedef std::function<void(int,int,int)>   MouseButtonCallback;

class InputManager {
    public:
    /* Default constructor */
    InputManager();
    /* Move constructor */
    InputManager(InputManager&& other);
    /* Destructor */
    virtual ~InputManager();

    void setKeyCallback(int key_code, std::function<void()>);
    void setMousePosCallback(MousePositionCallback f);
    void setMouseButtonCallback(MouseButtonCallback f);
    void keyCallback(int keycode, int scancode, int action, int mods) const;
    void mousePosCallback(double x, double y);
    void mouseButtonCallback(int button, int action, int mods);

    // Enable or disable Y axis inversion
    void invertY(bool invert = true);

    private:
    std::map<int, std::function<void()>> m_callbacks;    
    MousePositionCallback m_mousePosCallback;
    MouseButtonCallback m_mouseButtonCallback;
    int m_invertY;
    double m_mouseSensitivity;

    // No copy or assignment
    InputManager(InputManager const& other);
    void operator=(InputManager const& other);
};

} // namespace Engine

#endif
