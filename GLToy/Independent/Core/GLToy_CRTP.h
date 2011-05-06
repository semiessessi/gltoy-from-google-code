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

#ifndef __GLTOY_CRTP_H_
#define __GLTOY_CRTP_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Data Structures/GLToy_BitStream.h>
#include <Core/Data Structures/GLToy_HashMap.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

//class GLToy_BitStream;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

template< class T >
class GLToy_Destroyable
{

public:

    ~GLToy_Destroyable() { static_cast< T* const >( this )->Destroy(); }

};


template< class T >
class GLToy_Factory
{

public:

    template< class U >
    static T* Create( const U& xConstructorParameter ) { return new T( xConstructorParameter ); }
    
    template< class U, class V >
    static T* Create( const U& xConstructorParameter1, const V& xConstructorParameter2 ) { return new T( xConstructorParameter1, xConstructorParameter2 ); }
    
    template< class U, class V, class W >
    static T* Create( const U& xConstructorParameter1, const V& xConstructorParameter2, const W& xConstructorParameter3 ) { return new T( xConstructorParameter1, xConstructorParameter2, xConstructorParameter3 ); }

};

//template< class T, class Derived >
//class GLToy_HashMap_System
//{
//
//public:
//
//    bool Initialise()
//    {
//        return Derived::InitialiseSystem();
//    }
//
//    void Shutdown()
//    {
//        Derived::ShutdownSystem();
//
//        s_xHashMap.DeleteAll();
//    }
//
//private:
//
//    static GLToy_HashMap< T > s_xHashMap;
//
//};
//
//template< class T >
//GLToy_HashMap< T* > GLToy_HashMap_System::s_xHashMap;

#define GLToy_MemoryEqual( T ) \
    bool operator ==( const T& xStruct ) const \
    { \
        for( u_int u = 0; u < ( sizeof( T ) >> 2 ); ++u ) \
        { \
            if( reinterpret_cast< const u_int* const >( this )[ u ] \
                != reinterpret_cast< const u_int* const >( &xStruct )[ u ] ) \
            { \
                return false; \
            } \
        } \
         \
        for( u_int u = sizeof( T ) - ( sizeof( T ) & 3 ); u < sizeof( T ); ++u ) \
        { \
            if( reinterpret_cast< const u_char* const >( this )[ u ] \
                != reinterpret_cast< const u_char* const >( &xStruct )[ u ] ) \
            { \
                return false; \
            } \
        } \
         \
        return true; \
    } \
     \
    bool operator !=( const T& xStruct ) const { return !( *this == xStruct ); } \

#define GLToy_CopyableStruct_NoInitialise( T ) \
    T() \
    { \
    } \
     \
    T( const T& xStruct ) \
    { \
        GLToy_Memory::FixedCopy< sizeof( T ) >( this, &xStruct ); \
    } \
     \
    T& operator =( const T& xStruct ) \
    { \
        GLToy_Memory::FixedCopy< sizeof( T ) >( this, &xStruct ); \
        return *this; \
    } \
     \
    void ReadFromBitStream( const GLToy_BitStream& xStream ) \
	{ \
		u_char* const pucThis = reinterpret_cast< u_char* >( this ); \
		for( u_int u = 0; u < sizeof( T ); ++u ) \
		{ \
			xStream >> pucThis[ u ]; \
		} \
	} \
     \
    void WriteToBitStream( GLToy_BitStream& xStream ) const \
	{ \
		const u_char* const pucThis = reinterpret_cast< const u_char* >( this ); \
		for( u_int u = 0; u < sizeof( T ); ++u ) \
		{ \
			xStream << pucThis[ u ]; \
		} \
	} \

#define GLToy_CopyableStruct( T ) \
    T( const T& xStruct ) \
    { \
        GLToy_Memory::FixedCopy< sizeof( T ) >( this, &xStruct ); \
    } \
     \
    T& operator =( const T& xStruct ) \
    { \
        GLToy_Memory::FixedCopy< sizeof( T ) >( this, &xStruct ); \
        return *this; \
    } \
     \
    void ReadFromBitStream( const GLToy_BitStream& xStream ) \
	{ \
		u_char* const pucThis = reinterpret_cast< u_char* >( this ); \
		for( u_int u = 0; u < sizeof( T ); ++u ) \
		{ \
			xStream >> pucThis[ u ]; \
		} \
	} \
     \
    void WriteToBitStream( GLToy_BitStream& xStream ) const \
	{ \
		const u_char* const pucThis = reinterpret_cast< const u_char* >( this ); \
		for( u_int u = 0; u < sizeof( T ); ++u ) \
		{ \
			xStream << pucThis[ u ]; \
		} \
	} \

#define GLToy_CopyableStruct_OwnSerialise( T ) \
    T( const T& xStruct ) \
    { \
        GLToy_Memory::FixedCopy< sizeof( T ) >( this, &xStruct ); \
    } \
     \
    T& operator =( const T& xStruct ) \
    { \
        GLToy_Memory::FixedCopy< sizeof( T ) >( this, &xStruct ); \
        return *this; \
    } \

#endif