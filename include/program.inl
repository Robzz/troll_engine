template <class T>
Uniform<T>::~Uniform() { }

template <class T>
void Uniform<T>::set(T const& value) {
    m_value = value;
    m_clean = false;
}

template <class T>
Uniform<T>::Uniform(GLint location, std::string const& name) :
    UniformBase(location, name),
    m_value()
{ }

template <class T>
void Uniform<T>::upload() {
    if(!m_clean) {
        upload_uniform<T>(m_location, m_value);
    }
}