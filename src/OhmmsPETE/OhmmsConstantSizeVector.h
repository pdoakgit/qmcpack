#ifndef OHMMSCONSTANTSIZEVECTOR_H
#define OHMMSCONSTANTSIZEVECTOR_H

#include <stdexcept>

#include "OhmmsPETE/OhmmsVector.h"

namespace qmcplusplus
{
template<class T,typename Alloc = std::allocator<T>>
class ConstantSizeVector : public Vector<T, Alloc>
{
public:
  using Base = Vector<T, Alloc>;
  ConstantSizeVector(size_t n, size_t n_max, T val = T()) : n_max_(n_max)
  {
    if (n_max_ <= 0)
      throw std::runtime_error("Do not allocate a ConstantSizeVector of no size");
    Base::X = Base::mAllocator.allocate(n_max);
    if (n < n_max_)
      Base::X.resize(n);
    else
      throw std::runtime_error("ConstantSizeVector::ConstantSizevector n must be < n_max");
    std::fill_n(Base::X, n_max_, T());
    if (allocator_traits<Alloc>::is_host_accessible)
      std::fill_n(Base::X, n, val);
  }

  void resize(size_t n, T val = T())
  {
    if (n <= n_max_)
      Base::resize(n, val);
    else
      throw std::runtime_error("ConstantSizeVector cannot be resized to be larger than its initial alloc!");
  }
  
  void resize_impl(size_t n)
  {
    throw std::runtime_error("A ConstantSizeVector's resize_impl must never be called");
  }

  template<typename Allocator = Alloc, typename = IsHostSafe<Allocator>>
  T& operator()(size_t i)
  {
    return Base::X[i];
  }

  template<typename Allocator = Alloc, typename = IsHostSafe<Allocator>>
  T& operator()(size_t i) const
  {
    return Base::X[i];
  }

private:
  size_t n_max_;  
};

}

#endif /* OHMMSCONSTANTSIZEVECTOR_H */
