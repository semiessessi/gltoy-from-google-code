/////////////////////////////////////////////////////////////////////////////////////////////
// A S S E R T
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLTOY_ASSERT_H_
#define __GLTOY_ASSERT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// D E F I N E S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
    #define GLToy_Assert(condition, message, ...) if( _GLToy_Assert( condition, __FILE__, __LINE__, message, __VA_ARGS__ ) ) { __asm int 3 }
#else
    #define GLToy_Assert(condition, ...) ;
#endif

static const unsigned int ASSERT_MAX_TITLE_LENGTH = 512;
static const unsigned int ASSERT_MAX_MESSAGE_LENGTH = 512;

/////////////////////////////////////////////////////////////////////////////////////////////
// D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool _GLToy_Assert( const bool& bCondition, const char* sFileName, const int& uLineNumber, const char* sFormatString, ... );
bool Platform_GLToy_Assert( const char* sTitle, const char* sMessage );

#endif