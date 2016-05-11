template <typename T>
MeshBuilder& MeshBuilder::faces(std::vector<T>&& indices) {
    if(m_mesh->m_faces)
        throw std::runtime_error("Mesh already has indices");
    auto u = std::make_unique<ElementArray<T>>();
    u->data() = std::move(indices);
    m_mesh->m_faces = std::move(u);
    return *this;
}

template <typename T>
MeshBuilder& MeshBuilder::_initAttribArray(std::vector<T> vec, std::unique_ptr<AttributeArrayInstance<T>>& ptr,
                                           std::string const& errMsg) {
    if(ptr)
        throw std::runtime_error(errMsg);
    ptr = std::make_unique<AttributeArrayInstance<T>>();
    ptr->data() = std::move(vec);
    return *this;
}
