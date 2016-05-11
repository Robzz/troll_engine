template <typename T>
AttributeArrayInstance<T>* AttributeArray::downcast() {
    return dynamic_cast<AttributeArrayInstance<T>*>(this);
}

template <typename T>
const AttributeArrayInstance<T>* AttributeArray::downcast() const {
    return dynamic_cast<const AttributeArrayInstance<T>*>(this);
}

template <class T>
AttributeArrayInstance<T>::AttributeArrayInstance() :
    AttributeArray(traits::attribute_type_enumerator<T>::value),
    m_attribData() { }

template <class T>
AttributeArrayInstance<T>::~AttributeArrayInstance() { }

template <class T>
AttributeArrayInstance<T>::AttributeArrayInstance(AttributeArrayInstance const& other) :
    AttributeArray(traits::attribute_type_enumerator<T>::value),
    m_attribData(other.m_attribData) { }

template <class T>
AttributeArrayInstance<T>::AttributeArrayInstance(AttributeArrayInstance&& other) :
    AttributeArray(),
    m_attribData(std::move(other.m_attribData)) { }

template <class T>
AttributeArrayInstance<T>& AttributeArrayInstance<T>::operator=(AttributeArrayInstance<T> const& other) {
    m_attribData = other.m_attribData;
    return *this;
}

template <class T>
AttributeArrayInstance<T>& AttributeArrayInstance<T>::operator=(AttributeArrayInstance<T>&& other) {
    m_attribData = std::move(other.m_attribData);
    return *this;
}

template <class T>
size_t AttributeArrayInstance<T>::size() const { return m_attribData.size(); }

template <class T>
AttributeArray* AttributeArrayInstance<T>::clone() const {
    AttributeArrayInstance* attr = new AttributeArrayInstance(*this);
    return attr;
}

template <class T>
constexpr size_t AttributeArrayInstance<T>::dimension() const {
    return traits::dimension<T>::value;
}

template <class T>
std::vector<T>& AttributeArrayInstance<T>::data() { return m_attribData; }

template <class T>
std::vector<T> const& AttributeArrayInstance<T>::data() const { return m_attribData; }

template <class T>
ElementArray<T>::ElementArray() : AttributeArrayInstance<T>() { }

template <class T>
ElementArray<T>::~ElementArray() { }
