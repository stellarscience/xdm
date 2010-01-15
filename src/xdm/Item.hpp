//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_Item_hpp
#define xdm_Item_hpp

#include <xdm/ReferencedObject.hpp>
#include <xdm/XmlMetadataWrapper.hpp>

#include <string>

#include <xdm/NamespaceMacro.hpp>

/// Macro to assist classes in defining boilerplate functions that are
/// helpful when subclassing Item.  This macro will provide the correct
/// defintions of className() and accept(ItemVisitor&).
#define XDM_META_ITEM( name ) \
  virtual const char* className() const { return #name; } \
  virtual void accept( xdm::ItemVisitor& iv ) { iv.apply( *this ); }

XDM_NAMESPACE_BEGIN

class ItemVisitor;

/// Base class for all elements of the data hierarchy.
class Item : public ReferencedObject {
public:
  Item();
  virtual ~Item();

  void setName( const std::string& name );
  const std::string& name() const;

  /// Subclasses should override to return their specific type.
  virtual const char * className() const;

  //-- Visitor Traversal Interface --//
  
  /// Accept a visitor, calling its apply with my type as input.
  virtual void accept( ItemVisitor& iv );
  /// Define the traversal for an item.  The visitor will choose when,
  /// subclasses of item tell it how.
  virtual void traverse( ItemVisitor& );

  //-- End Visitor Traversal Interface --//

  /// Write an Item's metadata to an XmlObject.  NOTE: implementors should *NOT*
  /// write data for child objects to the input XmlObject.  That responsibility
  /// is reserved for visitor classes so that the communication, memory
  /// referencing, and cacheing can be managed by client applications.
  virtual void writeMetadata( XmlMetadataWrapper& metadata );

private:
  std::string mName;
};

XDM_NAMESPACE_END

#endif // xdm_Item_hpp

