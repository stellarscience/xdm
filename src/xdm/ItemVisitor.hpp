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
#ifndef xdm_ItemVisitor_hpp
#define xdm_ItemVisitor_hpp

#include <xdm/BinaryIStream.hpp>
#include <xdm/BinaryOStream.hpp>
#include <xdm/Item.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <stdexcept>
#include <string>



namespace xdm {

// forward declarations of classes the visitor will operate on
class CompositeDataItem;
class DataItem;
class UniformDataItem;

/// Error signalling that a method is not implemented for a type.
class MethodNotImplemented : public std::runtime_error {
private:
  std::string mMethodName;
public:
  MethodNotImplemented( const std::string& methodName ) :
    std::runtime_error( methodName + ": not implemented" ),
    mMethodName( methodName ) {}
  ~MethodNotImplemented() throw() {}
};

/// Class to represent type safe operations on the data tree.  Subclasses of
/// ItemVisitor should reimplement the apply functions for the core xdm objects
/// they wish to operate on.  NOTE:  To apply a visitor to an Item, call
/// Item::accept(visitor), DO NOT call ItemVisitor::apply(item).  This is to
/// ensure the correct type information is passed on to the visitor.
class ItemVisitor : public virtual ReferencedObject {
public:
  ItemVisitor();
  virtual ~ItemVisitor();

  //-- Apply functions --//
  virtual void apply( Item& item );
  virtual void apply( DataItem& item );
  virtual void apply( CompositeDataItem& item );
  virtual void apply( UniformDataItem& item );

  /// Traverse an Item.  Implementors should call this to allow the Item's
  /// subclass to determine how traversal must occur.
  inline void traverse( Item& item ) {
    item.traverse( *this );
  }

  /// Write a snapshot of the ItemVisitor's current state to a BinaryOStream.
  virtual void captureState( BinaryOStream& );
  /// Restore from a state snapshot contained in a BinaryIStream.
  virtual void restoreState( BinaryIStream& );

  /// Reset an ItemVisitor for a clean traversal. This method should be
  /// implemented by inheritors to clear any state that may be accumulated
  /// during a tree traversal. The default implementation does nothing.
  virtual void reset();
};

/// Convenience functor for applying a visitor to an Item.
class ApplyVisitor {
public:
  /// Pass in a reference to the visitor to apply.
  ApplyVisitor( ItemVisitor& iv ) : mIV( iv ) {}
  /// Copy constructor
  ApplyVisitor( const ApplyVisitor& av ) : mIV( av.mIV ) {}

  void operator()( Item* item ) { item->accept( mIV ); }
  void operator()( RefPtr< Item > item ) { item->accept( mIV ); };

private:
  ApplyVisitor& operator=( const ApplyVisitor& ) { return *this; }
  ItemVisitor& mIV;
};

} // namespace xdm

#endif // xdm_ItemVisitor_hpp

