//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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
#ifndef xdm_Forward_hpp
#define xdm_Forward_hpp

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class AllDataSelection;
class ArrayAdapter;
class BasicBinaryStreamBuffer;
class BasicItemUpdateCallback;
class BinaryIOStream;
class BinaryIStream;
class BinaryOStream;
class ByteArray;
class CompositeDataItem;
class DataItem;
class DataSelection;
class DataSelectionMap;
class DataSelectionVisitor;
template< typename T > class DataShape;
class Dataset;
class FileSystemPath;
template< typename T > class HyperSlab;
class Item;
template< typename T > class ItemUpdateCallback;
class ItemVisitor;
class MemoryAdapter;
class ProxyDataset;
template< typename T > class RefPtr;
class ReferencedObject;
class SerializeDataOperation;
class StructuredArray;
template< typename T > class TypedStructuredArray;
class UniformDataItem;
class UpdateVisitor;
template< typename T > class VectorRef;
template< typename T > class ConstVectorRef;
template< typename T > class VectorRefImp;
template< typename T > class VectorStructuredArray;
class XmlError;
class XmlMetadataWrapper;
class XmlObject;
class XmlOutputStream;
class XmlTextContent;

XDM_NAMESPACE_END

#endif // xdm_Forward_hpp

