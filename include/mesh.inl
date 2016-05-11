template <typename T>
MeshBuilder& MeshBuilder::_initAttribArray(std::vector<T> vec, std::unique_ptr<AttributeArrayInstance<T>>& ptr,
                                           std::string const& errMsg) {
    if(ptr)
        throw std::runtime_error(errMsg);
    ptr = std::make_unique<AttributeArrayInstance<T>>();
    ptr->data() = std::move(vec);
    return *this;
}
