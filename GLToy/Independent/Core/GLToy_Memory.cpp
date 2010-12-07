/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

// GLToy
#include <Core/GLToy.h>

// This file's header
#include <Core/GLToy_Memory.h>

// GLToy
#include <Core/Data Structures/GLToy_BinaryTree.h>
#include <String/GLToy_String.h>

#include <Core/GLToy_Memory_DebugOff.h>

// SE - 06/11/2010 - remove all the debug memory tree - although its fine for small projects etc its a bit dangerous and has a nasty habit
// of being enormously slow when deletions are being made - particularly when using "real world" data instead of little tests...
// tree hacks...
//#ifdef GLTOY_DEBUG
//    #define GLToy_BinaryTree GLToy_MemoryTreeBase
//    #define GLToy_BinaryTreeNode GLToy_MemoryTreeNode
//
//    #ifdef __GLTOY_BINARYTREE_H_
//        #undef __GLTOY_BINARYTREE_H_
//        #define __GLTOY_BINARYTREE_H_RESTORE
//    #endif
//
//    #include <Core/Data Structures/GLToy_BinaryTree.h>
//
//    #ifdef __GLTOY_BINARYTREE_H_RESTORE
//        #define __GLTOY_BINARYTREE_H_
//    #endif
//
//    #undef GLToy_BinaryTree
//    #undef GLToy_BinaryTreeNode
//#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

//#ifdef GLTOY_DEBUG
//struct GLToy_MemoryRecord
//{
//    u_int m_uSize;
//    GLToy_String m_szLocator;
//};
//
//typedef GLToy_MemoryTreeBase< GLToy_MemoryRecord, void* > GLToy_MemoryTree;
//#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

//#ifdef GLTOY_DEBUG
//#ifndef GLTOY_NO_MEMORY
//static GLToy_MemoryTree g_xMap;
//#endif
//#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Memory::Initialise()
{
    return true;
}

void GLToy_Memory::Shutdown()
{
//#ifdef GLTOY_DEBUG
//#ifndef GLTOY_NO_MEMORY
//    g_xMap.Clear();
//#endif
//#endif
}

void GLToy_Memory::Set( void* const pxMemory, const u_int uBytes, const u_char ucValue )
{
    if( uBytes < 8 )
    {
        for( u_int u = 0; u < uBytes; ++u )
        {
            reinterpret_cast< u_char* >( pxMemory )[ u ] = ucValue;
        }

        return;
    }

    const u_int uDwords = uBytes >> 2;
    const u_int uLeftOver = uBytes & 0x3;
    const u_int uValue = ( ( ( ( ( ( ( ucValue << 8 ) | ucValue ) << 8 ) | ucValue ) << 8 ) | ucValue ) << 8 );

    for( u_int u = 0; u < uDwords; ++u )
    {
        reinterpret_cast< u_int* >( pxMemory )[ u ] = uValue;
    }

    // fall through madness! :)
    switch( uLeftOver )
    {
        case 3:
            reinterpret_cast< u_char* >( pxMemory )[ uBytes - 3 ] = ucValue;
        case 2:
            reinterpret_cast< u_char* >( pxMemory )[ uBytes - 2 ] = ucValue;
        case 1:
            reinterpret_cast< u_char* >( pxMemory )[ uBytes - 1 ] = ucValue;
        default:
            break;
    }
}

void GLToy_Memory::Set_4Bytes( void* const pxMemory, const u_int uDWords, const u_int uValue )
{
    for( u_int u = 0; u < uDWords; ++u )
    {
        reinterpret_cast< u_int* >( pxMemory )[ u ] = uValue;
    }
}

#ifdef GLTOY_DEBUG
void* GLToy_ForceCDecl operator new( u_int uSize, const char* szFile, const int iLine )
{

//#ifndef GLTOY_NO_MEMORY
//    GLToy_MemoryRecord xRecord = { uSize, GLToy_String( szFile ) + " @ line " + iLine };
//#endif
    void* const pxAllocation = GLToy_Memory::Platform_Allocate( uSize );

//#ifndef GLTOY_NO_MEMORY
//    g_xMap.AddNode( xRecord, pxAllocation );
//#endif

    GLToy_Memory::MarkUninitialised( pxAllocation, uSize );

    return pxAllocation;
}

void* GLToy_ForceCDecl operator new[]( u_int uSize, const char* szFile, const int iLine )
{
    return ::operator new( uSize, szFile, iLine );
}

// in case exceptions get thrown... which they shouldn't - ever
void GLToy_ForceCDecl operator delete( void* pxMemory, const char* szFile, const int iLine )
{
    delete pxMemory;
}

void GLToy_ForceCDecl operator delete[]( void* pxMemory, const char* szFile, const int iLine )
{
    ::operator delete( pxMemory, szFile, iLine );
}
#endif

#ifndef GLTOY_DEMO
void* GLToy_ForceCDecl operator new( u_int uSize )
{
    return GLToy_Memory::Platform_Allocate( uSize );
}

void GLToy_ForceCDecl operator delete( void* pxMemory )
{
    // SE - 06/11/2010 - this is unreasonably slow with real world data (big ol' BSP tree for one)
//#ifdef GLTOY_DEBUG
//#ifndef GLTOY_NO_MEMORY
//    const GLToy_MemoryRecord* const pxRecord = g_xMap.FindData( pxMemory );
//    if( pxRecord )
//    {
//        GLToy_Memory::MarkDestroyed( pxMemory, pxRecord->m_uSize );
//        g_xMap.Remove( pxMemory );
//    }
//#endif
//#endif

    GLToy_Memory::Platform_Free( pxMemory );
}

void* GLToy_ForceCDecl operator new[]( u_int uSize )
{
	return ::operator new( uSize );
}

void GLToy_ForceCDecl operator delete[]( void* pxMemory )
{
	::operator delete( pxMemory );
}
#endif

#include <Core/GLToy_Memory_DebugOn.h>
