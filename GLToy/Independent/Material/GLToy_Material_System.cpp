/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Material/GLToy_Material_System.h>

// GLToy
#include <Core/Data Structures/GLToy_HashMap.h>
#include <File/GLToy_File_System.h>
#include <File/GLToy_KeyValueFile.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Light_System.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_HashMap< GLToy_Material > GLToy_Material_System::s_xMaterials;
GLToy_Material* GLToy_Material_System::s_pxBlackMaterial = NULL;
GLToy_Material* GLToy_Material_System::s_pxWhiteMaterial = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Material::GLToy_Material( const GLToy_Hash uHash )
: GLToy_Parent()
, m_uHash( uHash )
, m_iFogParamsHandle( -1 )
, m_uShader( uGLTOY_BAD_HASH )
, m_uFlags( 0 )
{
    for( u_int u = 0; u < 4; ++u )
    {
        m_auTextures[ u ] = uGLTOY_BAD_HASH;
        m_aszTextureNames[ u ] = "";
        m_aiSamplerHandles[ u ] = -1;
    }

    m_aiViewMatrixHandles[ 0 ] =
    m_aiViewMatrixHandles[ 1 ] = -1;
}

void GLToy_Material::Bind()
{
    if( !m_bInitialised )
    {
        Initialise();
    }

    GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( m_uShader );
    if( !pxShader )
    {
        return;
    }

    pxShader->Bind();

    for( u_int u = 0; u < 4; ++u )
    {
        if( m_auTextures[ u ] )
        {
            GLToy_Texture_System::BindTexture( m_auTextures[ u ], u );
            pxShader->SetUniform( m_aiSamplerHandles[ u ], static_cast< int >( u ) );
        }
    }

    pxShader->SetViewMatrix( m_aiViewMatrixHandles[ 0 ] );
    pxShader->SetInverseViewMatrix( m_aiViewMatrixHandles[ 1 ] );

    pxShader->SetUniform( m_iFogParamsHandle, GLToy_Vector_4( GLToy_Light_System::GetFogStartDistance(), 1.0f / ( GLToy_Light_System::GetFogEndDistance() - GLToy_Light_System::GetFogStartDistance() ), 0.0f, 0.0f ) );
}

bool GLToy_Material::IsReady() const
{
    for( u_int u = 0; u < 4; ++u )
    {
        GLToy_Texture* const pxTexture = GLToy_Texture_System::FindTexture( m_auTextures[ u ] );
        if( pxTexture && !( pxTexture->IsReadyForUse() ) )
        {
            return false;
        }
    }

    return true;
}

u_int GLToy_Material::GetWidth() const
{
    // TODO: something better
    return 256;
}

u_int GLToy_Material::GetHeight() const
{
    // TODO: something better
    return 256;
}

void GLToy_Material::Initialise()
{
	if( IsReady() )
	{
		return;
	}

    for( u_int u = 0; u < 4; ++u )
    {
        GLToy_Texture_System::CreateTexture( m_auTextures[ u ] );
    }

    GLToy_ShaderProgram* const pxShader = GLToy_Shader_System::FindShader( m_uShader );
    if( !pxShader )
    {
        return;
    }

    for( u_int u = 0; u < 4; ++u )
    {
        m_aiSamplerHandles[ u ] = pxShader->GetUniformHandle( m_aszTextureNames[ u ] );
    }

    m_aiViewMatrixHandles[ 0 ] = pxShader->GetUniformHandle( "xViewMatrix" );
    m_aiViewMatrixHandles[ 1 ] = pxShader->GetUniformHandle( "xInverseViewMatrix" );

    m_iFogParamsHandle = pxShader->GetUniformHandle( "xFogParams" );

    m_bInitialised = true;
}

bool GLToy_Material_System::Initialise()
{
    s_xMaterials.Clear();

    GLToy_Array< GLToy_String > xMaterialPaths = GLToy_File_System::PathsFromFilter( "Materials/", "*.material" );

    GLToy_ConstIterate( GLToy_String, szPath, xMaterialPaths )

        GLToy_String szName = szPath;

        szName.RemoveAt( 0, 10 ); // remove "Materials/"
        szName.RemoveFromEnd( 9 ); // remove ".material"

        GLToy_DebugOutput( "   - Found material \"%S\".\r\n", szName.GetWideString() );

        GLToy_Material xMaterial( szName.GetHash() );
        GLToy_KeyValueFile xFile( szPath );
        GLToy_Array< GLToy_Pair< GLToy_String > > xValues = xFile.LoadValues();

        GLToy_ConstIterate( GLToy_Pair< GLToy_String >, xKeyValue, xValues )
            
            const GLToy_String& szKey = xKeyValue.First();
            const GLToy_String& szValue = xKeyValue.Second();

            if( szKey == "Deferred" )
            {
                if( !szValue.MeansFalse() )
                {
                    xMaterial.SetDeferred();
                }
            }
            else if( szKey == "Forward" )
            {
                if( !szValue.MeansFalse() )
                {
                    xMaterial.SetForward();
                }
            }
            else if( szKey == "Transparent" )
            {
                if( !szValue.MeansFalse() )
                {
                    xMaterial.SetTransparent();
                }
            }
            else if( szKey == "Shader" )
            {
                xMaterial.SetShaderHash( szValue.GetHash() );
            }
            else // if we don't recognise the key, assume its a texture sampler
            {
                xMaterial.AddTexture( szValue.GetHash(), szKey );
            }

        GLToy_Iterate_End;

        s_xMaterials.AddNode( xMaterial, szName.GetHash() );

    GLToy_Iterate_End;

    // now for each texture create a default material
    GLToy_ConstIterate( GLToy_Texture, xTexture, GLToy_Texture_System::GetMap() )

        // only create it if there isn't a material with the same name
        if( s_xMaterials.FindData( xTexture.GetHash() ) )
        {
            continue;
        }

        // TODO: not sure why this happens (!)
        if( xTexture.GetHash() == uGLTOY_BAD_HASH )
        {
            continue;
        }

        GLToy_Material xMaterial( xTexture.GetHash() );
        xMaterial.SetDeferred();
        xMaterial.SetShaderHash( GLToy_Hash_Constant( "Deferred_DNS" ) );
        xMaterial.AddTexture( xTexture.GetHash(), "DiffuseMap" );
        xMaterial.AddTexture( GLToy_Hash_Constant( "DefaultNormal" ), "NormalMap" );
        xMaterial.AddTexture( GLToy_Hash_Constant( "DefaultSpecular" ), "SpecularMap" );

        s_xMaterials.AddNode( xMaterial, xTexture.GetHash() );

    GLToy_Iterate_End;

    return true;
}

void GLToy_Material_System::Shutdown()
{
    s_xMaterials.Clear();
}

void GLToy_Material_System::BindMaterial( const GLToy_String& szName )
{
    BindMaterial( szName.GetHash() );
}

void GLToy_Material_System::BindMaterial( const GLToy_Hash uHash )
{
    GLToy_Material* pxMaterial = FindMaterial( uHash );
    if( pxMaterial )
    {
        if( !pxMaterial->IsReady() )
        {
            pxMaterial->Initialise();
        }

        pxMaterial->Bind();
    }
    else
    {
        pxMaterial = FindMaterial( GLToy_Hash_Constant( "Generic/BadMaterial.png" ) );
        if( pxMaterial )
        {
            if( !pxMaterial->IsReady() )
            {
                pxMaterial->Initialise();
            }

            pxMaterial->Bind();
        }
    }
}

GLToy_Material* GLToy_Material_System::FindMaterial( const GLToy_Hash uHash )
{
    return s_xMaterials.FindData( uHash );
}

GLToy_Material* GLToy_Material_System::LookUpMaterial( const GLToy_String& szName )
{    
    return FindMaterial( szName.GetHash() );
}

GLToy_Material* GLToy_Material_System::LookUpMaterialNoExt( const GLToy_String& szName )
{    
    GLToy_Material* pxMaterial = FindMaterial( szName.GetHash() );

    if( !pxMaterial )
    {
        pxMaterial = LookUpMaterial( szName + ".wal" );
    }

    if( !pxMaterial )
    {
        pxMaterial = LookUpMaterial( szName + ".tga" );
    }

    if( !pxMaterial )
    {
        pxMaterial = LookUpMaterial( szName + ".png" );
    }

    if( !pxMaterial )
    {
        pxMaterial = LookUpMaterial( szName + ".jpg" );
    }

    if( !pxMaterial )
    {
        pxMaterial = LookUpMaterial( szName + ".bmp" );
    }

    if( !pxMaterial )
    {
        pxMaterial = LookUpMaterial( szName + ".pcx" );
    }

    if( !pxMaterial )
    {
        pxMaterial = LookUpMaterial( szName + ".ptx" );
    }

    return pxMaterial;
}
