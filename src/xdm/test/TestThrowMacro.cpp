//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE ThrowMacro 
#include <boost/test/unit_test.hpp>

#include <xdm/ThrowMacro.hpp>

#include <stdexcept>

namespace {

BOOST_AUTO_TEST_CASE( throws ) {
  BOOST_CHECK_THROW( XDM_THROW( std::runtime_error( "Test exception" ) ), 
    std::runtime_error );
}

} // namespace

