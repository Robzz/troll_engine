#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>

namespace Engine {

class AttributeBase {
    public:
    /* Default constructor */
    AttributeBase();
    /* Destructor */
    virtual ~AttributeBase();

    virtual size_t size() const = 0;
    virtual AttributeBase* copy() const = 0;
    
    private:
    AttributeBase(AttributeBase const& other);
    AttributeBase(AttributeBase&& other);
    AttributeBase& operator=(AttributeBase const& other);
    AttributeBase& operator=(AttributeBase&& other);
};

template <class T>
class Attribute : public AttributeBase {
    public:
    /* Default constructor */
    Attribute() :
        AttributeBase(),
        m_attribData()
    { }
    /* Destructor */
    virtual ~Attribute() { }

    /* Copy constructor */
    Attribute(Attribute const& other) :
        AttributeBase(),
        m_attribData(other.m_attribData)
    { }
    /* Move constructor */
    Attribute(Attribute&& other) :
        AttributeBase(),
        m_attribData(std::move(other.m_attribData))
    { }

    /* Assignment operator */
    Attribute& operator=(Attribute const& other) {
        m_attribData = other.m_attribData;
        return *this;
    }
    /* Move-assignment operator */
    Attribute& operator=(Attribute&& other) {
        m_attribData = std::move(other.m_attribData);
        return *this;
    }

    virtual size_t size() const { return m_attribData.size(); }

    virtual AttributeBase* copy() const {
        Attribute* attr = new Attribute(*this);
        return attr;
    }

    std::vector<T>& data() { return m_attribData; }
    std::vector<T> const& data() const { return m_attribData; }

    private:
    std::vector<T> m_attribData;
};

} // namespace Engine

#endif
