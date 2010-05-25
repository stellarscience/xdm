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
#ifndef xdmFormat_ReadResult_hpp
#define xdmFormat_ReadResult_hpp

#include <xdm/RefPtr.hpp>

#include <utility>



namespace xdm {
class Item;
}

namespace xdmFormat {

/// Data class to hold the result of reading a file containing one or more
/// series steps.
class ReadResult {
public:
  /// Constructor initializes with empty results.
  ReadResult();
  /// Constructor takes a RefPtr to the read item and the number of steps read.
  ReadResult( xdm::RefPtr< xdm::Item > item, size_t seriesSteps );
  ~ReadResult();

  /// Get the resulting item.
  xdm::RefPtr< xdm::Item > item() const;
  /// Get the number of steps that were read in.
  size_t seriesSteps() const;

private:
  xdm::RefPtr< xdm::Item > mItem;
  size_t mSeriesSteps;
};

} // namespace xdmFormat

#endif // xdmFormat_ReadResult_hpp
