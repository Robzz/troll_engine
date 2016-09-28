/**
  * \file include/input.h
  * \brief Contains the InputManager definition.
  * \author R.Chavignat
  */
#ifndef INPUT_H
#define INPUT_H

#include <functional>
#include <map>

namespace Engine {

/**
 * @brief Type for callbacks receiving mouse position changes
 */
typedef std::function<void(double,double)> MousePositionCallback;
/**
 * @brief Type for callbacks receiving mouse button state changes
 */
typedef std::function<void(int,int,int)>   MouseButtonCallback;

class Window;

/**
 * \class InputManager
 * \brief The InputManager allows setting callbacks to react to user input.
 */
class InputManager {
    friend class GLFWWindow;

    public:
        /**
         * @brief Default constructor.
         */
        InputManager();

        /**
         * @brief Move constructor.
         *
         * @param other InputManager object to move
         */
        InputManager(InputManager&& other);

        /**
         * @brief Destructor.
         */
        virtual ~InputManager();

        /**
         * @brief Set a callback to be called when a certain key is pressed.
         *
         * @param key_code Keycode of the key
         * @param f Callback function
         */
        void setKeyCallback(int key_code, std::function<void()> f);

        /**
         * @brief Set a callback to be called when the mouse is moved.
         *
         * @param f Callback function.
         */
        void setMousePosCallback(MousePositionCallback f);

        /**
         * @brief Set a callback to be called a mouse button state changes.
         *
         * @param f Callback function.
         */
        void setMouseButtonCallback(MouseButtonCallback f);

        /**
         * @brief Enable or disable inverted Y axis.
         *
         * @param invert true to invert Y axis
         */
        void invertY(bool invert = true);

    private:
        std::map<int, std::function<void()>> m_callbacks;
        MousePositionCallback m_mousePosCallback;
        MouseButtonCallback m_mouseButtonCallback;
        int m_invertY;
        double m_mouseSensitivity;

        void keyCallback(int keycode, int scancode, int action, int mods) const;
        void mousePosCallback(double x, double y);
        void mouseButtonCallback(int button, int action, int mods);

        /* No copy */
        InputManager(InputManager const& other) = delete;
        InputManager& operator=(InputManager const& other) = delete;
};

} // namespace Engine

#endif
