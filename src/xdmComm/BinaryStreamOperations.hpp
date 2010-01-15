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
#ifndef xdmComm_BinaryStreamOperations_hpp
#define xdmComm_BinaryStreamOperations_hpp

#include <xdmComm/BinaryIStream.hpp>
#include <xdmComm/BinaryOStream.hpp>
#include <xdmComm/ReceiveBufferArray.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelection.hpp>
#include <xdm/DataSelectionMap.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/StructuredArray.hpp>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

BinaryIStream& operator>>( BinaryIStream& istr, xdm::AllDataSelection& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::AllDataSelection& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::DataSelectionMap& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::DataSelectionMap& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::DataShape<>& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::DataShape<>& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::HyperSlab<>& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::HyperSlab<>& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::HyperslabDataSelection& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::HyperslabDataSelection& v );

BinaryIStream& operator>>( BinaryIStream& istr, xdm::primitiveType::Value& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::primitiveType::Value& v );

/// This inserter assumes that the output StructuredArray already has at least
/// enough space allocated to hold the data.
BinaryIStream& operator>>( BinaryIStream& istr, xdmComm::ReceiveBufferArray& v );
BinaryOStream& operator<<( BinaryOStream& ostr, const xdm::StructuredArray& v );

XDM_COMM_NAMESPACE_END

#endif // xdmComm_BinaryStreamOperations_hpp

