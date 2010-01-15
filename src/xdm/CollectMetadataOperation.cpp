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
#include <xdm/CollectMetadataOperation.hpp>

#include <xdm/BinaryStreamOperations.hpp>
#include <xdm/XmlObject.hpp>

XDM_NAMESPACE_BEGIN

CollectMetadataOperation::ContextManager::ContextManager( 
  CollectMetadataOperation& op, 
  RefPtr< XmlObject > obj ) :
  mOp( op ) {
  mOp.mContextStack.push( obj );
}

CollectMetadataOperation::ContextManager::~ContextManager() {
  // save the result of the completed context.
  mOp.mResult = mOp.mContextStack.top();
  // pop the context.
  mOp.mContextStack.pop();
}

CollectMetadataOperation::CollectMetadataOperation() :
  mContextStack() {
}

CollectMetadataOperation::CollectMetadataOperation( 
  xdm::RefPtr< XmlObject > xml ) :
  mContextStack() {
  mContextStack.push( xml );
}

CollectMetadataOperation::~CollectMetadataOperation() {
}

void CollectMetadataOperation::apply( xdm::Item& item ) {
  // construct a new XmlObject for the current item and instruct the item to
  // write to it.
  RefPtr< XmlObject > itemXml( new XmlObject );
  XmlMetadataWrapper wrapper( itemXml );
  item.writeMetadata( wrapper );

  // if this is not the root item, append the new xml to the back of the stack
  if ( !mContextStack.empty() ) {
    mContextStack.top()->appendChild( itemXml );
  }

  // manage the stack in case something happens below
  ContextManager mgr( *this, itemXml );
  traverse( item );
}

void CollectMetadataOperation::captureState( BinaryOStream& ostr ) {
  ostr << *mResult;
}

void CollectMetadataOperation::restoreState( BinaryIStream& istr ) {
  RefPtr< XmlObject > resultXml( new XmlObject );
  istr >> *resultXml;

  if ( ! mContextStack.empty() ) {
    mContextStack.top()->appendChild( resultXml );
  }
}

RefPtr< XmlObject > CollectMetadataOperation::result() {
  return mResult;
}

XDM_NAMESPACE_END

