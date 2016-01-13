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

    void registerCallback(int key_code, std::function<void()>);
    void inputCallback(int keycode, int scancode, int action, int mods);

    private:
    std::map<int, std::function<void()>> m_callbacks;    

    // No copy or assignment
    InputManager(InputManager const& other);
    void operator=(InputManager const& other);
};

#endif
