#include <xdmGrid/Attribute.hpp>

#include <map>
#include <string>

XDM_GRID_NAMESPACE_BEGIN

namespace {
  struct AttributeTypeMapping : 
    public std::map< Attribute::Type, std::string > {
    AttributeTypeMapping() {
      using std::make_pair;
      insert( make_pair( Attribute::kScalar, "Scalar" ) );
      insert( make_pair( Attribute::kVector, "Vector" ) );
      insert( make_pair( Attribute::kTensor, "Tensor" ) );
      insert( make_pair( Attribute::kTensor6, "Tensor6" ) );
      insert( make_pair( Attribute::kMatrix, "Matrix" ) );
    }
  };
  static AttributeTypeMapping sAttributeTypeMapping;

  struct AttributeCenterMapping :
    public std::map< Attribute::Center, std::string > {
    AttributeCenterMapping() {
      using std::make_pair;
      insert( make_pair( Attribute::kNode, "Node" ) );
      insert( make_pair( Attribute::kEdge, "Edge" ) );
      insert( make_pair( Attribute::kFace, "Face" ) );
      insert( make_pair( Attribute::kCell, "Cell" ) );
      insert( make_pair( Attribute::kGrid, "Grid" ) );
    }
  };
  static AttributeCenterMapping sAttributeCenterMapping;
} // namespace anon

Attribute::Attribute( Type t, Center c ) :
  mType( t ),
  mCenter( c ) {
}

Attribute::~Attribute() {
}

void Attribute::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::CompositeDataItem::writeMetadata( xml );

  xml.setTag( "Attribute" );

  xml.setAttribute( "AttributeType", sAttributeTypeMapping[ mType ] );
  xml.setAttribute( "Center", sAttributeCenterMapping[ mCenter] );
}

XDM_GRID_NAMESPACE_END

