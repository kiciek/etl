///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_MEMORY_INCLUDED
#define ETL_MEMORY_INCLUDED

#include <iterator>
#include <algorithm>

#include "platform.h"
#include "type_traits.h"

///\defgroup memory memory
///\ingroup etl
namespace etl
{
  //*****************************************************************************
  /// Gets the address of an object.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  T* addressof(T& t)
  {
      return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(t)));
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename T>
  typename etl::enable_if<etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value)
  {
    std::fill(o_begin, o_end, value);

    return o_end;
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename T>
  typename etl::enable_if<!etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type(value);
      ++o_begin;
    }

    return o_end;
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename T, typename TCounter>
  typename etl::enable_if<etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value, TCounter& count)
  {
    count += int32_t(std::distance(o_begin, o_end));

    std::fill(o_begin, o_end, value);
    
    return o_end;
  }

  //*****************************************************************************
  /// Fills uninitialised memory range with a value.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename T, typename TCounter>
  typename etl::enable_if<!etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_fill(TOutputIterator o_begin, TOutputIterator o_end, const T& value, TCounter& count)
  {
    count += int32_t(std::distance(o_begin, o_end));

    etl::uninitialized_fill(o_begin, o_end, value);

    return o_end;
  }

  //*****************************************************************************
  /// Fills uninitialised memory with N values.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename T>
   TOutputIterator uninitialized_fill_n(TOutputIterator o_begin, TSize n, const T& value)
  {
    return etl::uninitialized_fill(o_begin, o_begin + n, value);
  }

  //*****************************************************************************
  /// Fills uninitialised memory with N values.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename T, typename TCounter>
   TOutputIterator uninitialized_fill_n(TOutputIterator o_begin, TSize n, const T& value, TCounter& count)
  {
    count += n;

    return etl::uninitialized_fill(o_begin, o_begin + n, value);
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  typename etl::enable_if<etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin)
  {
    return std::copy(i_begin, i_end, o_begin);
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator>
  typename etl::enable_if<!etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    TOutputIterator o_end = o_begin;

    while (i_begin != i_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_end))) value_type(*i_begin);
      ++i_begin;
      ++o_end;
    }

    return o_end;
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TCounter>
  typename etl::enable_if<etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TCounter& count)
  {
    TOutputIterator o_end = std::copy(i_begin, i_end, o_begin);
    count += int32_t(std::distance(o_begin, o_end));

    return o_end;
  }

  //*****************************************************************************
  /// Copies a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TOutputIterator, typename TCounter>
  typename etl::enable_if<!etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_copy(TInputIterator i_begin, TInputIterator i_end, TOutputIterator o_begin, TCounter& count)
  {
    TOutputIterator o_end = etl::uninitialized_copy(i_begin, i_end, o_begin);

    count += int32_t(std::distance(o_begin, o_end));

    return o_end;
  }

  //*****************************************************************************
  /// Copies N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator>
   TOutputIterator uninitialized_copy_n(TInputIterator i_begin, TSize n, TOutputIterator o_begin)
  {
    return etl::uninitialized_copy(i_begin, i_begin + n, o_begin);
  }

  //*****************************************************************************
  /// Copies N objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TInputIterator, typename TSize, typename TOutputIterator, typename TCounter>
   TOutputIterator uninitialized_copy_n(TInputIterator i_begin, TSize n, TOutputIterator o_begin, TCounter& count)
  {
    count += n;

    return etl::uninitialized_copy(i_begin, i_begin + n, o_begin);
  }

  //*****************************************************************************
  /// Default contruct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_trivially_constructible<T>::value, void>::type
   create_default_at(T* /*p*/)
  {
  }

  //*****************************************************************************
  /// Default contruct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  typename etl::enable_if<etl::is_trivially_constructible<T>::value, void>::type
   create_default_at(T* /*p*/, TCounter& count)
  {
    ++count;
  }

  //*****************************************************************************
  /// Default contruct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  typename etl::enable_if<!etl::is_trivially_constructible<T>::value, void>::type
   create_default_at(T* p)
  {
    ::new (p) T;
  }

  //*****************************************************************************
  /// Default contruct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  typename etl::enable_if<!etl::is_trivially_constructible<T>::value, void>::type
   create_default_at(T* p, TCounter& count)
  {
    ::new (p) T;
    ++count;
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  typename etl::enable_if<etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_default_construct(TOutputIterator /*o_begin*/, TOutputIterator /*o_end*/)
  {
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  typename etl::enable_if<!etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type;
      ++o_begin;
    }
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TCounter>
  typename etl::enable_if<etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end, TCounter& count)
  {
    count = int32_t(std::distance(o_begin, o_end));
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TCounter>
  typename etl::enable_if<!etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_default_construct(TOutputIterator o_begin, TOutputIterator o_end, TCounter& count)
  {
    count += int32_t(std::distance(o_begin, o_end));

    etl::uninitialized_default_construct(o_begin, o_end);
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize>
  typename etl::enable_if<etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_default_construct_n(TOutputIterator o_begin, TSize n)
  {
    TOutputIterator o_end = o_begin + n;

    return o_end;
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize>  
  typename etl::enable_if<!etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_default_construct_n(TOutputIterator o_begin, TSize n)
  {
    TOutputIterator o_end = o_begin + n;

    etl::uninitialized_default_construct(o_begin, o_end);

    return o_end;
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename TCounter>
  typename etl::enable_if<etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_default_construct_n(TOutputIterator o_begin, TSize n, TCounter& count)
  {
    TOutputIterator o_end = o_begin + n;

    count += n;

    return o_end;
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename TCounter>
  typename etl::enable_if<!etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, TOutputIterator>::type
   uninitialized_default_construct_n(TOutputIterator o_begin, TSize n, TCounter& count)
  {
    TOutputIterator o_end = o_begin + n;

    etl::uninitialized_default_construct(o_begin, o_end);

    count += n;

    return o_end;
  }

  //*****************************************************************************
  /// Value construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
   void create_value_at(T* p)
  {
    ::new (p) T();
  }

  //*****************************************************************************
  /// Value construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
   void create_value_at(T* p, TCounter& count)
  {
    ::new (p) T();
    ++count;
  }

  //*****************************************************************************
  /// Copy construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
   void create_copy_at(T* p, const T& value)
  {
    ::new (p) T(value);
  }

  //*****************************************************************************
  /// Copy construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
   void create_copy_at(T* p, const T& value, TCounter& count)
  {
    ::new (p) T(value);
    ++count;
  }

  //*****************************************************************************
  /// Construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
   T& make_default_at(T* p)
  {
    ::new (p) T();
    return *reinterpret_cast<T*>(p);
  }

  //*****************************************************************************
  /// Construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
   T& make_default_at(T* p, TCounter& count)
  {
    ::new (p) T();
    ++count;
    return *reinterpret_cast<T*>(p);
  }

  //*****************************************************************************
  /// Construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
   T& make_copy_at(T* p, const T& other)
  {
    ::new (p) T(other);
    return *reinterpret_cast<T*>(p);
  }

  //*****************************************************************************
  /// Construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
   T& make_copy_at(T* p, const T& other, TCounter& count)
  {
    ::new (p) T(other);
    ++count;
    return *reinterpret_cast<T*>(p);
  }

  //*****************************************************************************
  /// Construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TParameter>
   T& make_value_at(T* p, const TParameter& value)
  {
    ::new (p) T(value);
    return *reinterpret_cast<T*>(p);
  }

  //*****************************************************************************
  /// Construct an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TParameter, typename TCounter>
   T& make_value_at(T* p, const TParameter& value, TCounter& count)
  {
    ::new (p) T(value);
    ++count;
    return *reinterpret_cast<T*>(p);
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  typename etl::enable_if<etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_value_construct(TOutputIterator o_begin, TOutputIterator o_end)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    std::fill(o_begin, o_end, value_type());
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator>
  typename etl::enable_if<!etl::is_trivially_constructible<typename std::iterator_traits<TOutputIterator>::value_type>::value, void>::type
   uninitialized_value_construct(TOutputIterator o_begin, TOutputIterator o_end)
  {
    typedef typename std::iterator_traits<TOutputIterator>::value_type value_type;

    while (o_begin != o_end)
    {
      ::new (static_cast<void*>(etl::addressof(*o_begin))) value_type();
      ++o_begin;
    }
  }

  //*****************************************************************************
  /// Default initialises a range of objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TCounter>
  void uninitialized_value_construct(TOutputIterator o_begin, TOutputIterator o_end, TCounter& count)
  {
    count += int32_t(std::distance(o_begin, o_end));

    etl::uninitialized_value_construct(o_begin, o_end);
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize>
  TOutputIterator uninitialized_value_construct_n(TOutputIterator o_begin, TSize n)
  {
    TOutputIterator o_end = o_begin + n;

    etl::uninitialized_value_construct(o_begin, o_end);

    return o_end;
  }

  //*****************************************************************************
  /// Default initialises N objects to uninitialised memory.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TOutputIterator, typename TSize, typename TCounter>
  TOutputIterator uninitialized_value_construct_n(TOutputIterator o_begin, TSize n, TCounter& count)
  {
    TOutputIterator o_end = o_begin + n;

    etl::uninitialized_value_construct(o_begin, o_end);

    count += n;

    return o_end;
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  typename etl::enable_if<etl::is_trivially_destructible<T>::value, void>::type
   destroy_at(T* /*p*/)
  {
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  typename etl::enable_if<!etl::is_trivially_destructible<T>::value, void>::type
   destroy_at(T* p)
  {
    p->~T();
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  typename etl::enable_if<etl::is_trivially_destructible<T>::value, void>::type
   destroy_at(T* /*p*/, TCounter& count)
  {
    --count;
  }

  //*****************************************************************************
  /// Destroys an item at address p.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T, typename TCounter>
  typename etl::enable_if<!etl::is_trivially_destructible<T>::value, void>::type
   destroy_at(T* p, TCounter& count)
  {
    p->~T();
    --count;
  }

  //*****************************************************************************
  /// Destroys a range of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator>
  typename etl::enable_if<etl::is_trivially_destructible<typename std::iterator_traits<TIterator>::value_type>::value, void>::type
   destroy(TIterator /*i_begin*/, TIterator /*i_end*/)
  {
  }

  //*****************************************************************************
  /// Destroys a range of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator>
  typename etl::enable_if<!etl::is_trivially_destructible<typename std::iterator_traits<TIterator>::value_type>::value, void>::type
   destroy(TIterator i_begin, TIterator i_end)
  {
    while (i_begin != i_end)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
    }
  }

  //*****************************************************************************
  /// Destroys a range of items.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TCounter>
  typename etl::enable_if<etl::is_trivially_destructible<typename std::iterator_traits<TIterator>::value_type>::value, void>::type
   destroy(TIterator i_begin, TIterator i_end, TCounter& count)
  {
    count -= int32_t(std::distance(i_begin, i_end));
  }

  //*****************************************************************************
  /// Destroys a range of items.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TCounter>
  typename etl::enable_if<!etl::is_trivially_destructible<typename std::iterator_traits<TIterator>::value_type>::value, void>::type
   destroy(TIterator i_begin, TIterator i_end, TCounter& count)
  {
    count -= int32_t(std::distance(i_begin, i_end));

    while (i_begin != i_end)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
    }
  }

  //*****************************************************************************
  /// Destroys a number of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize>
  typename etl::enable_if<etl::is_trivially_destructible<typename std::iterator_traits<TIterator>::value_type>::value, TIterator>::type
   destroy_n(TIterator i_begin, TSize n)
  {
    return i_begin + n;
  }

  //*****************************************************************************
  /// Destroys a number of items.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize>
  typename etl::enable_if<!etl::is_trivially_destructible<typename std::iterator_traits<TIterator>::value_type>::value, TIterator>::type
   destroy_n(TIterator i_begin, TSize n)
  {
    while (n > 0)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
      --n;
    }

    return i_begin;
  }

  //*****************************************************************************
  /// Destroys a number of items.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize, typename TCounter>
  typename etl::enable_if<etl::is_trivially_destructible<typename std::iterator_traits<TIterator>::value_type>::value, TIterator>::type
   destroy_n(TIterator i_begin, TSize n, TCounter& count)
  {
    count -= n;
    return i_begin + n;
  }

  //*****************************************************************************
  /// Destroys a number of items.
  /// Debug counter version.
  ///\ingroup memory
  //*****************************************************************************
  template <typename TIterator, typename TSize, typename TCounter>
  typename etl::enable_if<!etl::is_trivially_destructible<typename std::iterator_traits<TIterator>::value_type>::value, TIterator>::type
   destroy_n(TIterator i_begin, TSize n, TCounter& count)
  {
    count -= n;

    while (n > 0)
    {
      etl::destroy_at(etl::addressof(*i_begin));
      ++i_begin;
      --n;
    }

    return i_begin;
  }

  //*****************************************************************************
  /// Copy constructs a derived class to an address.
  ///\tparam T The derived type.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  struct create_copy
  {
    void create_copy_at(void* p)
    {
      new (p) T(static_cast<const T&>(*this));
    }

    template <typename TCounter>
    void create_copy_at(void* p, TCounter& count)
    {
      new (p) T(static_cast<const T&>(*this));
      ++count;
    }

    T& make_copy_at(void* p)
    {
      new (p) T(static_cast<const T&>(*this));
      return *reinterpret_cast<T*>(p);
    }

    template <typename TCounter>
    T& make_copy_at(void* p, TCounter& count)
    {
      new (p) T(static_cast<const T&>(*this));
      ++count;
      return *reinterpret_cast<T*>(p);
    }
  };

  //*****************************************************************************
  /// Unique pointer.
  ///\tparam T The pointed to type type.
  ///\ingroup memory
  //*****************************************************************************
  template <typename T>
  class unique_ptr 
  {
  public:

      typedef T  element_type;
      typedef T* pointer;
      typedef T& reference;

      ETL_CONSTEXPR unique_ptr()
      	: p(nullptr)
      {
      }

      ETL_CONSTEXPR explicit unique_ptr (pointer p_)
        : p(p_)
      {
      }

#if ETL_CPP11_SUPPORTED
      unique_ptr (unique_ptr&& p_)
        : p(p_.release())
      {
      }
#endif
      
      ~unique_ptr()
      {
        delete p;
      }

      ETL_CONSTEXPR pointer	get() const
      {
        return p;
      }
      
      pointer	release()
      { 
        pointer value = p;
        p = nullptr;
        
        return value;
      }

      void reset(pointer p_ = pointer())
      {
        assert(p_ != p);

        pointer value = p;
        p = p_;
        delete value;
      }
      
      void swap(unique_ptr& value)
      { 
        std::swap(p, value.p);
      }
      
      ETL_CONSTEXPR explicit	operator bool() const
      {
        return (p != nullptr);
      }
      
      unique_ptr&	operator =(pointer p_)
      {
        reset(p_);
        
        return *this;
      }

#if ETL_CPP11_SUPPORTED
      unique_ptr&	operator =(unique_ptr&& p_)
      {
        reset(p_.release());
        
        return *this; 
      }
#endif
     
      ETL_CONSTEXPR reference	operator *() const
      {
        return *get();
      }
      
      ETL_CONSTEXPR pointer	operator ->() const
      {
        return get();
      }
      
      ETL_CONSTEXPR reference	operator [](size_t i) const
      {
        return get()[i];
      }
      
      ETL_CONSTEXPR bool operator== (const pointer p_) const
      {
        return p == p_;
      }
      
      ETL_CONSTEXPR bool operator== (const unique_ptr& p_) const
      {
        return p == p_.p;
      }
      
      ETL_CONSTEXPR bool operator< (const unique_ptr& p_) const
      {
        return p < p_.p;
      }

  private:

    // Deleted.
    unique_ptr(const unique_ptr&);
    unique_ptr&	operator =(const unique_ptr&);

    pointer	p;
  };


  //*****************************************************************************
  /// Unique pointer for arrays.
  ///\tparam T The pointed to type type.
  ///\ingroup memory
  //*****************************************************************************
  template<typename T>
  class unique_ptr<T[]> 
  {
  public:

    typedef T  element_type;
    typedef T* pointer;
    typedef T& reference;
  
    ETL_CONSTEXPR		unique_ptr() 
      : p(nullptr)
    {
    }

    ETL_CONSTEXPR explicit unique_ptr(pointer p_)
      : p(p_)
    {
    }
    
#if ETL_CPP11_SUPPORTED
    unique_ptr(unique_ptr&& p_)
      : p(p_.release())
    {
    }
#endif
    
    ~unique_ptr() 
    {
      delete[] p;
    }
    
    ETL_CONSTEXPR pointer	get() const
    {
      return p;
    }
    
    pointer	release() 
    {
      pointer value = p;
      p = nullptr;
      return value;
    }
    
    void reset(pointer p_)
    {
      assert(p_ != p);

      pointer value = p; 
      p = p_; 
      delete[] value; 
    }

    void swap(unique_ptr& v)
    {
      std::swap(p, v.p); 
    }

    ETL_CONSTEXPR explicit operator bool() const
    {
      return (p != nullptr);
    }

    unique_ptr& operator =(pointer p_)
    {
      reset(p_);
      
      return *this; 
    }

#if ETL_CPP11_SUPPORTED
    unique_ptr& operator =(unique_ptr&& p_)
    {
      reset(p_.release());

      return *this; 
    }
#endif

    ETL_CONSTEXPR reference	operator *() const
    {
      return *p; 
    }

    ETL_CONSTEXPR pointer	operator ->() const
    {
      return p; 
    }

    ETL_CONSTEXPR reference	operator [](size_t i) const
    {
      return p[i]; 
    }

    ETL_CONSTEXPR bool operator ==(const pointer p_) const
    {
      return (p == p_); 
    }

    ETL_CONSTEXPR bool operator ==(const unique_ptr& p_) const 
    {
      return (p == p_.p);
    }

    ETL_CONSTEXPR bool operator <(const unique_ptr& p_) const
    {
      return (p < p_.p);
    }
  
  private:

    // Deleted.
    unique_ptr(const unique_ptr&);
    unique_ptr&	operator =(const unique_ptr&);

    pointer			p;
  };
}

#endif
