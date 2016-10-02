#ifndef UBO_H
#include "ubo.h"
#endif

template <class U>
UBO<U>::UBO() :
    VBO(traits::block_size<U>::value),
    data()
{ }

template <class U>
UBO<U>::~UBO() { }

template <class U>
void UBO<U>::upload_std140(gl::GLenum hint) {
    traits::uploadUBOStd140Impl<U, 0>::f(data, *this);
}

namespace traits {
    template <class U, unsigned int i>
    inline void uploadUBOStd140Impl<U, i>::f(U const& block, VBO& v) {
        upload_field(block, v, field_std140_offset<U, i>::value);
        std::conditional<(i+1) < n_fields<U>::value,
                         uploadUBOStd140Impl<U, i+1>,
                         no_op<U const&, VBO&>>::type::f(block, v);
    }

    template <class U, unsigned int i>
    inline void uploadUBOStd140Impl<U, i>::
    upload_field(U const& block, VBO& v, ptrdiff_t offset) {
        using T = decltype(get_field<U, i>::value(block));
        v.update_data<T>(get_field<U, i>::value(block), sizeof(T), offset);
    }
} // namespace traits
