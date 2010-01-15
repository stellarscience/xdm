//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmComm_ParallelizeTreeVisitor_hpp
#define xdmComm_ParallelizeTreeVisitor_hpp

#include <xdm/ItemVisitor.hpp>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Tree operation that replaces any datasets held by a UniformDataItem with an
/// MpiDatasetProxy to handle communication between processes.
class ParallelizeTreeVisitor : public xdm::ItemVisitor {
private:
  size_t mBufferSize;

public:
  ParallelizeTreeVisitor( size_t bufferSize );
  virtual ~ParallelizeTreeVisitor();

  virtual void apply( xdm::UniformDataItem& item );
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_ParallelizeTreeVisitor_hpp

