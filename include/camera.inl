template <class T>
Camera<T>::Camera() : m_camToWorld() { }

template <class T>
Camera<T>::Camera(Camera const& other) :
    m_camToWorld(other.m_camToWorld)
{ }

template <class T>
Camera<T>::~Camera() { }

template <class T>
Camera<T>& Camera<T>::operator=(Camera const& other) {
    m_camToWorld = other.m_camToWorld;
    return *this;
}

/*
template <class T>
void Camera<T>::look_at(glm::vec3 const& pos, glm::vec3 const& target, glm::vec3 const& up) {
    glm::mat4 m = glm::lookAtRH(pos, target, up);
    m_camToWorld.set_position(-m[3].xyz());
    m_camToWorld.set_orientation(glm::mat3(m));
    std::cout << "Camera vectors :" << std::endl
              << "X : " << m_camToWorld.x() << std::endl
              << "Y : " << m_camToWorld.y() << std::endl
              << "Z : " << m_camToWorld.z() << std::endl;
}*/

template <class T>
void Camera<T>::translate(glm::vec3 const& v) {
    m_camToWorld.translate(v);
}

template <class T>
void Camera<T>::translate_local(Direction dir, float f) {
    m_camToWorld.translate_local(dir, f);
    /*std::cout << "Camera vectors :" << std::endl
              << "Camera pos : " << m_camToWorld.position() << std::endl
              << "X : " << m_camToWorld.x() << std::endl
              << "Y : " << m_camToWorld.y() << std::endl
              << "Z : " << m_camToWorld.z() << std::endl;*/
}

template <class T>
void Camera<T>::rotate(glm::vec3 const& axis, float angle) {
    /* -angle because we store camToWorld, ie world orientation
     * relative to camera. Rotating camera left means rotating the
     * world right. Or something. */
    m_camToWorld.rotate(axis, -angle);
    /*std::cout << "Camera vectors :" << std::endl
              << "X : " << m_camToWorld.x() << std::endl
              << "Y : " << m_camToWorld.y() << std::endl
              << "Z : " << m_camToWorld.z() << std::endl;*/
}

template <class T>
void Camera<T>::rotate_local(Axis axis, float angle) {
    m_camToWorld.rotate_local(axis, -angle);
    /*std::cout << "Camera vectors :" << std::endl
              << "X : " << m_camToWorld.x() << std::endl
              << "Y : " << m_camToWorld.y() << std::endl
              << "Z : " << m_camToWorld.z() << std::endl;*/
}

template <class T>
glm::mat4 Camera<T>::world_to_camera() const {
    return glm::inverse(m_camToWorld.matrix());
}

template <class T>
T const& Camera<T>::transform() const {
    return m_camToWorld; 
}
