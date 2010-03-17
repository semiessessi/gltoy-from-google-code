/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <File/GLToy_MD2File.h>

// GLToy
#include <Model/GLToy_Model_MD2.h>
#include <Render/GLToy_Texture.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

struct GLToy_MD2_Header
{

    u_int   m_uIdentifier; // IDP2
    u_int   m_uVersion; // 8

    u_int   m_uSkinWidth;
    u_int   m_uSkinHeight;
    u_int   m_uFrameSize;

    u_int   m_uNumSkins;
    u_int   m_uNumVertices;
    u_int   m_uNumTexCoords;
    u_int   m_uNumTriangles;
    u_int   m_uNumGLCommands;
    u_int   m_uNumFrames;

    u_int   m_uOffsetSkins;
    u_int   m_uOffsetTexCoords;
    u_int   m_uOffsetTriangles;
    u_int   m_uOffsetFrames;
    u_int   m_uOffsetGLCommands;
    u_int   m_uOffsetEnd;

};

struct GLToy_MD2_Vertex
{

    u_char m_aucCoordinates[ 3 ];
    u_char m_ucNormalIndex;

};

struct GLToy_MD2_TexCoord
{

    short m_sS;
    short m_sT;

};

struct GLToy_MD2_Frame
{

    GLToy_Vector_3 m_xScale;
    GLToy_Vector_3 m_xTranslate;
    char m_szName[ 16 ];
    GLToy_MD2_Vertex m_axVertices[ 1 ];

};

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model* GLToy_MD2File::LoadModel() const
{
    if( m_pxModel )
    {
        return m_pxModel;
    }

    if( GetSize() <= sizeof( GLToy_MD2_Header ) )
    {
        return NULL;
    }

    char* pcData = new char[ GetSize() ];
    GetAllData( pcData );

    GLToy_MD2_Header* pxHeader = reinterpret_cast< GLToy_MD2_Header* >( pcData );

    if( pxHeader->m_uIdentifier != GLToy_HeaderBytes( "IDP2" ) )
    {
        delete[] pcData;
        return NULL;
    }

    if( pxHeader->m_uVersion != 8 )
    {
        delete[] pcData;
        return NULL;
    }

    GLToy_Model_MD2* pxModel = new GLToy_Model_MD2();
    m_pxModel = pxModel;

    // load frame data, which is coords and normal indices per vertex
    GLToy_MD2_Frame* pxFrame = reinterpret_cast< GLToy_MD2_Frame* >( &( pcData[ pxHeader->m_uOffsetFrames ] ) );

    pxModel->m_xVertices.Resize( pxHeader->m_uNumVertices * pxHeader->m_uNumFrames );
    pxModel->m_xNormalIndices.Resize( pxHeader->m_uNumVertices * pxHeader->m_uNumFrames );
    pxModel->SetFrameSize( pxHeader->m_uNumVertices );

    for( u_int u = 0; u < pxHeader->m_uNumFrames; ++u )
    {
        GLToy_MD2_Frame& xFrame = *pxFrame;
        for( u_int v = 0; v < pxHeader->m_uNumVertices; ++v )
        {
            const u_int uVertexIndex = u * pxHeader->m_uNumVertices + v;
            // as well as decompressing the vertex, do some component jiggling to correct the axes from the quake 2 standard
            pxModel->m_xVertices[ uVertexIndex ] =
                GLToy_Vector_3(
                    -( static_cast< float >( xFrame.m_axVertices[ v ].m_aucCoordinates[ 1 ] ) * xFrame.m_xScale[ 1 ] + xFrame.m_xTranslate[ 1 ] ),
                    static_cast< float >( xFrame.m_axVertices[ v ].m_aucCoordinates[ 2 ] ) * xFrame.m_xScale[ 2 ] + xFrame.m_xTranslate[ 2 ],
                    static_cast< float >( xFrame.m_axVertices[ v ].m_aucCoordinates[ 0 ] ) * xFrame.m_xScale[ 0 ] + xFrame.m_xTranslate[ 0 ] );
            
            pxModel->GrowBBByPoint( pxModel->m_xVertices[ uVertexIndex ] );

            pxModel->m_xNormalIndices[ uVertexIndex ] = xFrame.m_axVertices[ v ].m_ucNormalIndex;
        }

        pxFrame = reinterpret_cast< GLToy_MD2_Frame* >( reinterpret_cast< char* >( pxFrame ) + pxHeader->m_uFrameSize );
    }

    // load GL commands
    pxModel->m_xGLCommands = GLToy_PointerArray< int >(
        reinterpret_cast< int* >( &( pcData[ pxHeader->m_uOffsetGLCommands ] ) ), pxHeader->m_uNumGLCommands );

    // load remaining data
    GLToy_PointerArray< GLToy_MD2_TexCoord > xTexCoords(
        reinterpret_cast< GLToy_MD2_TexCoord* >( &( pcData[ pxHeader->m_uOffsetTexCoords ] ) ), pxHeader->m_uNumTexCoords );

    pxModel->m_xTexCoords.Resize( pxHeader->m_uNumTexCoords );

    for( u_int u = 0; u < pxHeader->m_uNumTexCoords; ++u )
    {
        pxModel->m_xTexCoords[ u ][ 0 ] = static_cast< float >( xTexCoords[ u ].m_sS ) * ( 1.0f / pxHeader->m_uSkinWidth );
        pxModel->m_xTexCoords[ u ][ 1 ] = static_cast< float >( xTexCoords[ u ].m_sT ) * ( 1.0f / pxHeader->m_uSkinHeight );
        pxModel->m_xTexCoords[ u ][ 2 ] = 0.0f;
;
    }

    pxModel->m_xTriangles = GLToy_PointerArray< GLToy_MD2_Triangle >(
        reinterpret_cast< GLToy_MD2_Triangle* >( &( pcData[ pxHeader->m_uOffsetTriangles ] ) ), pxHeader->m_uNumTriangles );

    delete[] pcData;

    // try finding a texture from the filename
    GLToy_String szBaseName = m_szFilename;
    szBaseName.RemoveAt( 0, 7 ); // Models/
    szBaseName = GLToy_String( "Skins/" ) + szBaseName;
    szBaseName.RemoveFromEnd( 4 ); // .md2

    GLToy_Texture* pxTexture = NULL;

    pxTexture = GLToy_Texture_System::LookUpTexture( szBaseName + ".png" );
    
    if( pxTexture )
    {
        GLToy_Texture_System::CreateTexture( szBaseName + ".png" );
        pxModel->SetTexture( pxTexture );
    }

    // TODO - try non .png files

    pxModel->InitialiseFirstFrameData();

    return pxModel;

}