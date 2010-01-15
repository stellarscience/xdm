#include <xdm/CollectMetadataOperation.hpp>

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

RefPtr< XmlObject > CollectMetadataOperation::result() {
  return mResult;
}

XDM_NAMESPACE_END

