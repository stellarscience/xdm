#include <gtest/gtest.h>

#include <xdm/HyperSlabBlockIterator.hpp>

#include <algorithm>

TEST( HyperSlabBlockIterator, constructSmall ) {
  // construct a 5x5 slab
  xdm::HyperSlab<> total( xdm::makeShape( 5, 5 ) );
  std::fill( total.beginStart(), total.endStart(), 0 );
  std::fill( total.beginStride(), total.endStride(), 1 );
  std::fill( total.beginCount(), total.endCount(), 5 );

  // construct a block iterator with size 2x6
  xdm::DataShape<> blockSize = xdm::makeShape( 2, 6 );
  xdm::HyperSlabBlockIterator<> test( total, blockSize );

  ASSERT_EQ( 2, test->shape().rank() );
  ASSERT_EQ( 0, test->start(0) );
  ASSERT_EQ( 0, test->start(1) );
  ASSERT_EQ( 1, test->stride(0) );
  ASSERT_EQ( 1, test->stride(1) );
  ASSERT_EQ( 2, test->count(0) );
  ASSERT_EQ( 5, test->count(1) );
}

TEST( HyperSlabBlockIterator, iterate ) {
  // construct a 5x5 slab
  xdm::HyperSlab<> total( xdm::makeShape( 5, 5 ) );
  std::fill( total.beginStart(), total.endStart(), 0 );
  std::fill( total.beginStride(), total.endStride(), 1 );
  std::fill( total.beginCount(), total.endCount(), 5 );

  // construct a block iterator with size 3x2
  xdm::DataShape<> blockSize = xdm::makeShape( 3, 2 );
  xdm::HyperSlabBlockIterator<> test( total, blockSize );

  ASSERT_EQ( 0, test->start(0) );
  ASSERT_EQ( 0, test->start(1) );
  ASSERT_EQ( 3, test->count(0) );
  ASSERT_EQ( 2, test->count(1) );

  ++test;
  ASSERT_EQ( 3, test->start(0) );
  ASSERT_EQ( 0, test->start(1) );
  ASSERT_EQ( 2, test->count(0) );
  ASSERT_EQ( 2, test->count(1) );
  
  ++test;
  ASSERT_EQ( 0, test->start(0) );
  ASSERT_EQ( 2, test->start(1) );
  ASSERT_EQ( 3, test->count(0) );
  ASSERT_EQ( 2, test->count(1) );
  
  ++test;
  ASSERT_EQ( 3, test->start(0) );
  ASSERT_EQ( 2, test->start(1) );
  ASSERT_EQ( 2, test->count(0) );
  ASSERT_EQ( 2, test->count(1) );

  ++test;
  ASSERT_EQ( 0, test->start(0) );
  ASSERT_EQ( 4, test->start(1) );
  ASSERT_EQ( 3, test->count(0) );
  ASSERT_EQ( 1, test->count(1) );
  
  ++test;
  ASSERT_EQ( 3, test->start(0) );
  ASSERT_EQ( 4, test->start(1) );
  ASSERT_EQ( 2, test->count(0) );
  ASSERT_EQ( 1, test->count(1) );

  ++test;
  ASSERT_TRUE( test == xdm::HyperSlabBlockIterator<>() );
}

TEST( HyperSlabBlockIterator, iterateStride ) {
  // construct a 5x5 slab with stride 2
  xdm::HyperSlab<> total( xdm::makeShape( 5, 5 ) );
  std::fill( total.beginStart(), total.endStart(), 0 );
  std::fill( total.beginStride(), total.endStride(), 2 );
  std::fill( total.beginCount(), total.endCount(), 3 );

  // construct a block iterator with size 3x2
  xdm::DataShape<> blockSize = xdm::makeShape( 3, 2 );
  xdm::HyperSlabBlockIterator<> test( total, blockSize );

  ASSERT_EQ( 0, test->start(0) );
  ASSERT_EQ( 0, test->start(1) );
  ASSERT_EQ( 3, test->count(0) );
  ASSERT_EQ( 2, test->count(1) );
  
  ++test;
  ASSERT_EQ( 0, test->start(0) );
  ASSERT_EQ( 4, test->start(1) );
  ASSERT_EQ( 3, test->count(0) );
  ASSERT_EQ( 1, test->count(1) );

  ++test;
  ASSERT_TRUE( test == xdm::HyperSlabBlockIterator<>() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

