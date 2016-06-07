#ifndef GLCONTEXT_H
#define GLCONTEXT_H

namespace Engine {

class GLContext {
    public:
        /* Default constructor */
        GLContext();
        /* Destructor */
        virtual ~GLContext();

        /* Move constructor */
        GLContext(GLContext&& other);
        /* Move-assignment operator */
        GLContext& operator=(GLContext&& other);

        /* No copy */
        GLContext(GLContext const& other) = delete;
        GLContext& operator=(GLContext const& other) = delete;

        virtual bool debug() const = 0;

    private:
};

} // namespace Engine

#endif
