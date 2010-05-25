//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//
// This file is part of XDM
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------------------------------------------------------
#ifndef xdm_StaticAssert_hpp
#define xdm_StaticAssert_hpp



namespace xdm {

namespace detail {
  template< bool B > struct StaticAssertion;
  template<> struct StaticAssertion< true > {};
} // namespace detail

} // namespace xdm

#define XDM_PP_JOIN(a, b) XDM_PP_JOIN_IMPL(a, b)
#define XDM_PP_JOIN_IMPL(a, b) a ## b

/// Perform a compile time check of a static boolean value.
#define XDM_STATIC_ASSERT( B ) \
  enum { XDM_PP_JOIN(STATIC_ASSERT_LINE_, __LINE__) = \
    sizeof( ::xdm::detail::StaticAssertion< (bool)(B) > ) }

#endif // xdm_StaticAssert_hpp
