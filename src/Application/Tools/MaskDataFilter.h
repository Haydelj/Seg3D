/*
 For more information, please see: http://software.sci.utah.edu
 
 The MIT License
 
 Copyright (c) 2009 Scientific Computing and Imaging Institute,
 University of Utah.
 
 
 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */

#ifndef APPLICATION_TOOLS_MASKDATAFILTER_H
#define APPLICATION_TOOLS_MASKDATAFILTER_H

#include <Application/Tool/Tool.h>

namespace Seg3D
{

class MaskDataFilter : public Tool
{

SEG3D_TOOL(
SEG3D_TOOL_NAME( "MaskDataFilter", "Cut a masked region out of a data layer" )
SEG3D_TOOL_MENULABEL( "Mask Data" )
SEG3D_TOOL_MENU( "Filters" )
SEG3D_TOOL_SHORTCUT_KEY( "Alt+J" )
SEG3D_TOOL_URL( "http://seg3d.org/" )
)

public:
  MaskDataFilter( const std::string& toolid );
  virtual ~MaskDataFilter();

  // -- constraint parameters --

  // Constrain viewer to right painting tool when layer is selected
  void target_constraint( std::string layerid );

  // -- activate/deactivate tool --

  virtual void activate();
  virtual void deactivate();
  
private:
  // -- handle updates from layermanager --
  void handle_layers_changed();

  // -- state --
public:

  // Layerid of the target layer
  Core::StateStringHandle target_layer_state_;

  Core::StateStringHandle mask_layer_state_;

  Core::StateOptionHandle replace_with_state_;

  Core::StateBoolHandle replace_state_;

};

} // end namespace

#endif
