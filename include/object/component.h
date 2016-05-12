#ifndef COMPONENT_H
#define COMPONENT_H

namespace Engine {

/**
  * \class ComponentBase
  * \brief Base class for components.
  */
class ComponentBase {
    public:
        /* Default constructor */
        ComponentBase();
        /* Destructor */
        virtual ~ComponentBase();

        /* Copy constructor */
        ComponentBase(ComponentBase const& other);
        /* Move constructor */
        ComponentBase(ComponentBase&& other);
        /* Assignment operator */
        ComponentBase& operator=(ComponentBase const& other);
        /* Move-assignment operator */
        ComponentBase& operator=(ComponentBase&& other);

    private:
};

} // namespace Engine

#endif
