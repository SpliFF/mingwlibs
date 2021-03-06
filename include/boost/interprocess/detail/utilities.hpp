//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2008.
// (C) Copyright Gennaro Prota 2003 - 2004.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_DETAIL_UTILITIES_HPP
#define BOOST_INTERPROCESS_DETAIL_UTILITIES_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/detail/move.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>
#include <boost/interprocess/detail/min_max.hpp>
#include <boost/interprocess/detail/type_traits.hpp>
#include <boost/interprocess/detail/transform_iterator.hpp>
#include <boost/interprocess/detail/mpl.hpp>
#include <boost/interprocess/containers/version_type.hpp>
#include <boost/interprocess/detail/move.hpp>
#include <utility>
#include <algorithm>

namespace boost {
namespace interprocess { 
namespace detail {

template<class SmartPtr>
struct smart_ptr_type
{
   typedef typename SmartPtr::value_type value_type;
   typedef value_type *pointer;
   static pointer get (const SmartPtr &smartptr)
   {  return smartptr.get();}
};

template<class T>
struct smart_ptr_type<T*>
{
   typedef T value_type;
   typedef value_type *pointer;
   static pointer get (pointer ptr)
   {  return ptr;}
};

//!Overload for smart pointers to avoid ADL problems with get_pointer
template<class Ptr>
inline typename smart_ptr_type<Ptr>::pointer
get_pointer(const Ptr &ptr)
{  return smart_ptr_type<Ptr>::get(ptr);   }

//!To avoid ADL problems with swap
template <class T>
inline void do_swap(T& x, T& y)
{
   using std::swap;
   swap(x, y);
}

//Rounds "orig_size" by excess to round_to bytes
inline std::size_t get_rounded_size(std::size_t orig_size, std::size_t round_to)
{
   return ((orig_size-1)/round_to+1)*round_to;
}

//Truncates "orig_size" to a multiple of "multiple" bytes.
inline std::size_t get_truncated_size(std::size_t orig_size, std::size_t multiple)
{
   return orig_size/multiple*multiple;
}

//Rounds "orig_size" by excess to round_to bytes. round_to must be power of two
inline std::size_t get_rounded_size_po2(std::size_t orig_size, std::size_t round_to)
{
   return ((orig_size-1)&(~(round_to-1))) + round_to;
}

//Truncates "orig_size" to a multiple of "multiple" bytes. multiple must be power of two
inline std::size_t get_truncated_size_po2(std::size_t orig_size, std::size_t multiple)
{
   return (orig_size & (~(multiple-1)));
}

template <std::size_t OrigSize, std::size_t RoundTo>
struct ct_rounded_size
{
   enum { value = ((OrigSize-1)/RoundTo+1)*RoundTo };
};

// Gennaro Prota wrote this. Thanks!
template <int p, int n = 4>
struct ct_max_pow2_less
{
   enum { c = 2*n < p };

   static const std::size_t value =
         c ? (ct_max_pow2_less< c*p, 2*c*n>::value) : n;
};

template <>
struct ct_max_pow2_less<0, 0>
{
   static const std::size_t value = 0;
};

}  //namespace detail {

//!Trait class to detect if an index is a node
//!index. This allows more efficient operations
//!when deallocating named objects.
template <class Index>
struct is_node_index
{
   enum {   value = false };
};

//!Trait class to detect if an index is an intrusive
//!index. This will embed the derivation hook in each
//!allocation header, to provide memory for the intrusive
//!container.
template <class Index>
struct is_intrusive_index
{
   enum {   value = false };
};

template <typename T> T*
addressof(T& v)
{
  return reinterpret_cast<T*>(
       &const_cast<char&>(reinterpret_cast<const volatile char &>(v)));
}

//Anti-exception node eraser
template<class Cont>
class value_eraser
{
   public:
   value_eraser(Cont & cont, typename Cont::iterator it) 
      : m_cont(cont), m_index_it(it), m_erase(true){}
   ~value_eraser()  
   {  if(m_erase) m_cont.erase(m_index_it);  }

   void release() {  m_erase = false;  }

   private:
   Cont                   &m_cont;
   typename Cont::iterator m_index_it;
   bool                    m_erase;
};

}  //namespace interprocess { 
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //#ifndef BOOST_INTERPROCESS_DETAIL_UTILITIES_HPP

