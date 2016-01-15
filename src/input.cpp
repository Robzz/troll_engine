#include <GLFW/glfw3.h>

#include "input.h"

InputManager::InputManager() :
    m_callbacks(),
    m_mousePosCallback(),
    m_invertY(-1),
    m_mouseSensitivity(1)
{ }

InputManager::InputManager(InputManager&& other) :
    m_callbacks(other.m_callbacks),
    m_mousePosCallback(other.m_mousePosCallback),
    m_invertY(other.m_invertY),
    m_mouseSensitivity(other.m_mouseSensitivity)
{ }

InputManager::~InputManager() {

}

void InputManager::setKeyCallback(int key_code, std::function<void()> f) {
    m_callbacks[key_code] = f;
}

void InputManager::setMouseCallback(std::function<void(double, double)> f) {
    m_mousePosCallback = f;
}

void InputManager::keyCallback(int key_code, int scan_code, int action, int mods) const {
    if(action == GLFW_PRESS) {
        std::map<int, std::function<void()>>::const_iterator it = m_callbacks.find(key_code);
        if(it != m_callbacks.end()) {
            (*it).second();
        }        
    }
}

void InputManager::mouseCallback(double x, double y) {
    m_mousePosCallback(x * m_mouseSensitivity, y * m_mouseSensitivity * -m_invertY); 
}

void InputManager::invertY(bool invert) {
    m_invertY = invert ? -1 : 1;
}
