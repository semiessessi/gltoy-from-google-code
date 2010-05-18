/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLTOY_RENDERABLE_H_
#define __GLTOY_RENDERABLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Renderable
{

public:

    GLToy_Renderable() {}
    virtual ~GLToy_Renderable() {}

    virtual void Initialise() {}
    virtual void Shutdown() {}

    // "normal" render function - must be overloaded
    virtual void Render() const = 0;

    // 2d render function
    virtual void Render2D() const {}
    
    // for deferred shading
    virtual void RenderDepth() const {}
    virtual void RenderNormals() const {}
    virtual void RenderSpecular() const {}

    // transparent render pass
    virtual void RenderTransparent() const {}

};

#endif
