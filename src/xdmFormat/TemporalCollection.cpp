#include <xdmFormat/TemporalCollection.hpp>

#include <xdmFormat/XdmfHelpers.hpp>

#include <xdm/CollectMetadataOperation.hpp>
#include <xdm/SerializeDataOperation.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/UpdateVisitor.hpp>
#include <xdm/XmlObject.hpp>

#include <xdmGrid/Domain.hpp>
#include <xdmGrid/CollectionGrid.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

TemporalCollection::TemporalCollection( const std::string& metadataFile ) :
  mFileStream( (metadataFile + ".xmf").c_str(), std::ios::out ),
  mXmlStream( mFileStream )
{
}

TemporalCollection::~TemporalCollection()
{
}

void TemporalCollection::open() 
{
  xdm::RefPtr< xdmGrid::Domain > domain( new xdmGrid::Domain );
  xdm::RefPtr< xdmGrid::CollectionGrid > temporalCollection(
    new xdmGrid::CollectionGrid( xdmGrid::CollectionGrid::kTemporal ) );
  domain->addGrid( temporalCollection );

  // open a context within the xml stream to begin the temporal collection
  xdm::CollectMetadataOperation collect;
  domain->accept( collect );
  xdm::RefPtr< xdm::XmlObject > xdmf = xdmFormat::createXdmfRoot();
  xdmf->appendChild( collect.result() );
  mXmlStream.openContext( xdmf );
}

void TemporalCollection::writeTimestepGrid( xdm::RefPtr< xdmGrid::Grid > grid )
{
  // update the data tree for a new timestep.
  xdm::UpdateVisitor update;
  grid->accept( update );

  // serialize the heavy data
  xdm::SerializeDataOperation serializer;
  grid->accept( serializer );

  // write the metadata to the stream
  xdm::CollectMetadataOperation collect;
  grid->accept( collect );
  xdm::RefPtr< xdm::XmlObject > xml( collect.result() );
  mXmlStream.writeObject( xml );
}

void TemporalCollection::close()
{
  mXmlStream.closeStream();
}

XDM_FORMAT_NAMESPACE_END

