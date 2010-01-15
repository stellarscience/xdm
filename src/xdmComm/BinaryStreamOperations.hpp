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

