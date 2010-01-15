//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmComm/ParallelizeTreeVisitor.hpp>

#include <xdmComm/MpiDatasetProxy.hpp>

#include <xdm/Dataset.hpp>
#include <xdm/UniformDataItem.hpp>

#include <mpi.h>

XDM_COMM_NAMESPACE_BEGIN

ParallelizeTreeVisitor::ParallelizeTreeVisitor( size_t bufferSize ) :
  mBufferSize( bufferSize ) {
}

ParallelizeTreeVisitor::~ParallelizeTreeVisitor() {
}

void ParallelizeTreeVisitor::apply( xdm::UniformDataItem& item ) {
  xdm::Dataset* itemDataset = item.dataset();
  xdm::RefPtr< MpiDatasetProxy > proxy( new MpiDatasetProxy(
    MPI_COMM_WORLD, itemDataset, mBufferSize ) );
  item.setDataset( proxy.get() );
}

XDM_COMM_NAMESPACE_END

