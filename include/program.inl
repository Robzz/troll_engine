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

template <Shader::Type T>
Shader* ProgramBuilder::compileShader(std::string const& file) {
    Shader* s;
    auto it = m_manager.m_shaderCache.find(file);
    if(it != m_manager.m_shaderCache.end()) {
        s = it->second;
    }
    else {
        s = new Shader(file, T);
        if(!*s) {
            std::ostringstream ss;
            ss << "Shader compilation error in " << file << std::endl
               << "Info:" << std::endl << s->info_log();
            delete s;
            throw std::runtime_error(ss.str());
        }
        m_manager.m_shaderCache.insert(std::pair<std::string, Shader*>(file, s));
    }
    m_shaders.push_back(s);
    return s;
}
