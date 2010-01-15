#include <xdmFormat/VirtualDataset.hpp>

#include <xdmFormat/XdmfHelpers.hpp>

#include <xdm/CollectMetadataOperation.hpp>
#include <xdm/SerializeDataOperation.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/UpdateVisitor.hpp>
#include <xdm/XmlObject.hpp>
#include <xdm/XmlOutputStream.hpp>

#include <xdmGrid/Domain.hpp>
#include <xdmGrid/CollectionGrid.hpp>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

XDM_FORMAT_NAMESPACE_BEGIN

VirtualDataset::VirtualDataset( const std::string& metadataBaseName ) :
  mBaseName( metadataBaseName ),
  mTimeStep( 0 )
{
}

VirtualDataset::~VirtualDataset()
{
}

void VirtualDataset::open() 
{
  mTimeStep = 0;
}

void VirtualDataset::updateGrid( xdm::RefPtr< xdmGrid::Grid > grid ) {
  // update the grid for the new timestep.
  xdm::UpdateVisitor update;
  grid->accept( update );
}

void VirtualDataset::writeGridMetadata( xdm::RefPtr< xdmGrid::Grid > grid ) {
  // add the grid to a new domain for a valid layout
  xdm::RefPtr< xdmGrid::Domain > domain( new xdmGrid::Domain );
  domain->addGrid( grid );
  
  // open a new xml stream to write this timestep to
  std::stringstream outputName;
  outputName << mBaseName << ".";
  outputName << std::setfill('0') << std::setw( 7 ) << mTimeStep;
  outputName << ".xmf";
  std::fstream ostr( outputName.str().c_str(), std::ios::out );
  xdm::XmlOutputStream xml( ostr );

  // write the timestep to the xml stream
  xdm::CollectMetadataOperation collect;
  domain->accept( collect );
  
  xdm::RefPtr< xdm::XmlObject > xdmf = xdmFormat::createXdmfRoot();
  xdmf->appendChild( collect.result() );
  
  xml.writeObject( xdmf );

  xml.closeStream();

}

void VirtualDataset::writeGridData( xdm::RefPtr< xdmGrid::Grid > grid )
{
  // serialize the heavy data
  xdm::SerializeDataOperation serializer;
  grid->accept( serializer ); 
  mTimeStep++;
}

void VirtualDataset::close()
{
}

XDM_FORMAT_NAMESPACE_END

