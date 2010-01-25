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
#ifndef xdm_CollectMetadataOperation_hpp
#define xdm_CollectMetadataOperation_hpp

#include <xdm/ItemVisitor.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <stack>

XDM_NAMESPACE_BEGIN

class XmlObject;

/// Encapsulation of xml writing operation on a data tree.  This class
/// implements the ItemVisitor interface to traverse a data tree in memory and
/// build an XmlObject containing the full metadata description of the data
/// tree.
class CollectMetadataOperation : public ItemVisitor {
public:
  /// Empty Constructor begins with new XmlObject.
  CollectMetadataOperation();
  /// Constructor to start with an existing XmlObject.
  CollectMetadataOperation( RefPtr< XmlObject > xml );

  virtual ~CollectMetadataOperation();

  /// Apply the operation to a generic item, collecting it's metadata into an
  /// XmlObject that is then appended within the object's parent context.
  virtual void apply( Item& item );

  virtual void captureState( BinaryOStream& ostr );
  virtual void restoreState( BinaryIStream& istr );
  virtual void reset();

  /// Get the result of applying this operation to a data tree.  This method
  /// will return the XmlObject for last completed item.
  RefPtr< XmlObject > result();

private:
  typedef std::stack< RefPtr< XmlObject > > ContextStack;
  ContextStack mContextStack;
  RefPtr< XmlObject > mResult;

  // RAII helper for exception safe stack management.
  friend class ContextManager;
  class ContextManager {
  public:
    CollectMetadataOperation& mOp;
    ContextManager( CollectMetadataOperation& op, RefPtr< XmlObject > obj );
    ~ContextManager();
  };
};

XDM_NAMESPACE_END

#endif // xdm_CollectMetadataOperation_hpp

