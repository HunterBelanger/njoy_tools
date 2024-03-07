#ifndef NJOY_TOOLS_RANGES_ANYITERATOR
#define NJOY_TOOLS_RANGES_ANYITERATOR

// system includes

// other includes
#include "tools/std20/iterator.hpp"

namespace njoy {
namespace tools {
namespace ranges {

// AnyIterator concepts for different iterator categories (base classes)
#include "tools/views/AnyIterator/AnyIteratorConcept.hpp"

// AnyIterator models for different iterator categories (concept implementation)
#include "tools/views/AnyIterator/AnyIteratorModel.hpp"

/**
 *  @brief The AnyIterator class to type erase iterators
 */
template < typename IteratorCategory, 
           typename ValueType,
           typename ReferenceType,
           typename PointerType >
class AnyIterator {

  AnyIteratorConcept* ptr_;

  template < typename Iterator >
  AnyIterator( const std::input_iterator_tag&, Iterator&& iter ) :
    ptr_( new AnyInputIteratorModel< typename std::decay< Iterator >::type,
                                     ValueType, ReferenceType, PointerType >( std::forward< Iterator >( iter ) ) ) {}

  template < typename Iterator >
  AnyIterator( const std::forward_iterator_tag&, Iterator&& iter ) :
    ptr_( new AnyForwardIteratorModel< typename std::decay< Iterator >::type,
                                       ValueType, ReferenceType, PointerType >( std::forward< Iterator >( iter ) ) ) {}

  template < typename Iterator >
  AnyIterator( const std::bidirectional_iterator_tag&, Iterator&& iter ) :
    ptr_( new AnyBidirectionalIteratorModel< typename std::decay< Iterator >::type,
                                             ValueType, ReferenceType, PointerType >( std::forward< Iterator >( iter ) ) ) {}

  template < typename Iterator >
  AnyIterator( const std::random_access_iterator_tag&, Iterator&& iter ) :
    ptr_( new AnyRandomAccessIteratorModel< typename std::decay< Iterator >::type,
                                            ValueType, ReferenceType, PointerType >( std::forward< Iterator >( iter ) ) ) {}

public:

  using value_type = ValueType;
  using reference = ReferenceType;
  using pointer = PointerType;
  using difference_type = std::ptrdiff_t;
  using iterator_category = IteratorCategory;

  /* constructor */

  AnyIterator() : ptr_( nullptr ) {}
  AnyIterator( const AnyIterator& iter ) : ptr_( iter.ptr_->copy() ) {}
  AnyIterator( AnyIterator&& iter ) : ptr_( iter.ptr_->copy() ) {}

  AnyIterator& operator=( const AnyIterator& right ) {

    delete this->ptr_;
    this->ptr_ = right.ptr_->copy();
    return *this;
  }

  AnyIterator& operator=( AnyIterator&& right ) {

    delete this->ptr_;
    this->ptr_ = right.ptr_->copy();
    return *this;
  }

  template < typename Iterator >
  AnyIterator( Iterator iter ) :
    AnyIterator( IteratorCategory{}, std::move( iter ) ) {}

  /* methods */

  constexpr reference operator*() const {

    using IteratorType = AnyInputIteratorConcept< ValueType, ReferenceType, PointerType >;
    return *static_cast< IteratorType& >( *this->ptr_ );
  }

  constexpr pointer operator->() const {

    using IteratorType = AnyInputIteratorConcept< ValueType, ReferenceType, PointerType >;
    return static_cast< IteratorType& >( *this->ptr_ );
  }

  constexpr bool operator==( const AnyIterator& right ) const {

    using IteratorType = AnyInputIteratorConcept< ValueType, ReferenceType, PointerType >;
    return static_cast< IteratorType& >( *this->ptr_ ) == static_cast< IteratorType& >( *right.ptr_ );
  }

  constexpr bool operator!=( const AnyIterator& right ) const {

    return !( this->operator==( right ) );
  }

  constexpr AnyIterator& operator++() {

    ++( *this->ptr_ );
    return *this;
  }

  constexpr AnyIterator operator++( int ) const {

    AnyIterator iter = *this;
    ++( *this->ptr_ );
    return iter;
  }

  constexpr AnyIterator& operator--( void ) {

//    static_assert(
//      nano::bidirectional_iterator< AnyIterator > == true,
//      "the operator-- method can only be made available for bidirectional iterators" );

    using IteratorType = AnyBidirectionalIteratorConcept< ValueType, ReferenceType, PointerType >;
    --( static_cast< IteratorType& >( *this->ptr_ ) );
    return *this;
  }

  constexpr AnyIterator operator--( int ) {

//    static_assert(
//      nano::bidirectional_iterator< AnyIterator > == true,
//      "the operator-- method can only be made available for bidirectional iterators" );

    AnyIterator iter = *this;
    --( *this );
    return iter;
  }

  constexpr AnyIterator& operator+=( difference_type i ) {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator+= method can only be made available for random access iterators" );

    using IteratorType = AnyRandomAccessIteratorConcept< ValueType, ReferenceType, PointerType >;
    static_cast< IteratorType& >( *this->ptr_ ) += i;
    return *this;
  }

  constexpr AnyIterator& operator-=( difference_type i ) {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator-= method can only be made available for random access iterators" );

    using IteratorType = AnyRandomAccessIteratorConcept< ValueType, ReferenceType, PointerType >;
    static_cast< IteratorType& >( *this->ptr_ ) -= i;
    return *this;
  }

  constexpr difference_type operator-( const AnyIterator& right ) const {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator- method can only be made available for random access iterators" );

    using IteratorType = AnyRandomAccessIteratorConcept< ValueType, ReferenceType, PointerType >;
    return static_cast< IteratorType& >( *this->ptr_ ) - static_cast< IteratorType& >( *right.ptr_ );
  }

  friend constexpr AnyIterator operator+( difference_type i, const AnyIterator& iter ) {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator+ method can only be made available for random access iterators" );

    AnyIterator result = iter;
    result += i;
    return result;
  }

  friend constexpr AnyIterator operator+( AnyIterator iter, difference_type i ) {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator+ method can only be made available for random access iterators" );

    AnyIterator result = iter;
    result += i;
    return result;
  }

  friend constexpr AnyIterator operator-( AnyIterator iter, difference_type i ) {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator+ method can only be made available for random access iterators" );

    AnyIterator result = iter;
    result -= i;
    return result;
  }

  constexpr reference operator[]( difference_type i ) const {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator[] method can only be made available for random access iterators" );

    using IteratorType = AnyRandomAccessIteratorConcept< ValueType, ReferenceType, PointerType >;
    return static_cast< IteratorType& >( *this->ptr_ )[i];
  }

  friend constexpr bool operator<( const AnyIterator& left,
                                   const AnyIterator& right) {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator< method can only be made available for random access iterators" );

    using IteratorType = AnyRandomAccessIteratorConcept< ValueType, ReferenceType, PointerType >;
    return static_cast< IteratorType& >( *left.ptr_ ) < static_cast< IteratorType& >( *right.ptr_ );
  }

  friend constexpr bool operator>( const AnyIterator& left,
                                   const AnyIterator& right ) {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator> method can only be made available for random access iterators" );

    return right < left;
  }

  friend constexpr bool operator<=( const AnyIterator& left,
                                    const AnyIterator& right ) {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator<= method can only be made available for random access iterators" );

    return !( left > right );
  }

  friend constexpr bool operator>=( const AnyIterator& left,
                                    const AnyIterator& right ) {

//    static_assert(
//      nano::random_access_iterator< AnyIterator > == true,
//      "the operator>= method can only be made available for random access iterators" );

    return !( left < right );
  }

  ~AnyIterator() {

    delete this->ptr_;
  }
};

template < typename ValueType,
           typename ReferenceType,
           typename PointerType >
using AnyInputIterator = AnyIterator< std::input_iterator_tag,
                                      ValueType,
                                      ReferenceType,
                                      PointerType >;
template < typename ValueType,
           typename ReferenceType,
           typename PointerType >
using AnyForwardIterator = AnyIterator< std::forward_iterator_tag,
                                        ValueType,
                                        ReferenceType,
                                        PointerType >;
template < typename ValueType,
           typename ReferenceType,
           typename PointerType >
using AnyBidirectionalIterator = AnyIterator< std::bidirectional_iterator_tag,
                                              ValueType,
                                              ReferenceType,
                                              PointerType >;
template < typename ValueType,
           typename ReferenceType,
           typename PointerType >
using AnyRandomAccessIterator = AnyIterator< std::random_access_iterator_tag,
                                             ValueType,
                                             ReferenceType,
                                             PointerType >;

} // ranges namespace
} // tools namespace
} // njoy namespace

#endif
