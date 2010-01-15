//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
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

