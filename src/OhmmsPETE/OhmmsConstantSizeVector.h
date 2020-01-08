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

  /**@{*/
  /** Methods for assignment or copy of identically sized or smaller
   *  ConstantSizeVector<T, Alloc>.
   *
   *  They all are cast to use Vector's PETE assign,
   */
  void copy(const Vector<T, Alloc>& rhs)
  {
    if (this->size() != rhs.size())
      throw std::runtime_error("copy from Matrix called, Protected Matrix may not be resized");
    assign(static_cast<Vector<T, Alloc>&>(*this), rhs);
  }
  void copy(const ConstantSizeVector& rhs)
  {
    if (this->size() != rhs.size())
      throw std::runtime_error("copy from Protected Matrix called, Protected Matrix may not be resized");
    assign(static_cast<Vector<T, Alloc>&>(*this), rhs);
  }
  ConstantSizeVector& operator=(const ConstantSizeVector& rhs)
  {
    if (this->size() != rhs.size())
      throw std::runtime_error("operator= called, Protected Matrix may not be resized");
    return static_cast<ConstantSizeVector&>(assign(static_cast<ConstantSizeVector<T, Alloc>&>(*this), rhs));
  }
  template<class RHS, typename Allocator = Alloc, typename = IsHostSafe<Allocator>>
  // This probably should get some sort of SFINAE protection
  ConstantSizeVector& operator=(const RHS& rhs) {
    if (this->size() != rhs.size())
      throw std::runtime_error("templated operator= called, Protected Matrix may not be resized");
    return static_cast<ConstantSizeVector&>(assign(static_cast<RHS&>(*this), rhs));
  }
  /**}@*/
  
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
