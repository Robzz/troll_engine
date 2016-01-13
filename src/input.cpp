#include <GLFW/glfw3.h>

#include "input.h"

InputManager::InputManager() :
    m_callbacks()
{ }

InputManager::InputManager(InputManager&& other) :
    m_callbacks(other.m_callbacks)
{ }

InputManager::~InputManager() {

}

void InputManager::registerCallback(int key_code, std::function<void()> f) {
    m_callbacks[key_code] = f;
}

void InputManager::inputCallback(int key_code, int scan_code, int action, int mods) {
    if(action == GLFW_PRESS) {
        std::map<int, std::function<void()>>::const_iterator it = m_callbacks.find(key_code);
        if(it != m_callbacks.end()) {
            (*it).second();
        }        
    }
}

