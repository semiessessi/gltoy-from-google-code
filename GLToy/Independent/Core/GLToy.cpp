/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/GLToy.h>

// GLToy headers
#include <Core/GLToy_Timer.h>
#include <Maths/GLToy_Maths.h>
#include <Render/GLToy_Render.h>

// C/C++ headers
#include <stdarg.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define GLTOY_INITIALISER_CALL( system ) GLToy_DebugOutput( "\r\n  " #system "\r\n" ); if( !system::Initialise() ) { return false; }

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy::s_bFullscreen = false;
int GLToy::s_iWidth = 320;
int GLToy::s_iHeight = 200;

static char sDebugMessageBuffer[uDEBUGOUTPUT_MAX_LENGTH];

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

int GLToy::EntryPoint()
{
    Initialise();

    GLToy_DebugOutput( "\r\nGLToy::EntryPoint() - Entering Main Loop.\r\n" );

    bool bRunning = true;
    while( bRunning )
    {
        bRunning = MainLoop();
    }

    Shutdown();

    return 0;
}

bool GLToy::Resize( const int& iWidth, const int& iHeight )
{
    if( iWidth > 0 )
    {
        s_iWidth = iWidth;
    }
    else
    {
        s_iWidth = 1;
    }

    if( iHeight > 0 )
    {
        s_iHeight = iHeight;
    }
    else
    {
        s_iHeight = 1;
    }

    return Platform_Resize( s_iWidth, s_iHeight );
}

void GLToy::UpdateBuffers()
{
    Platform_UpdateBuffers();
}

bool GLToy::Initialise()
{
    GLToy_DebugOutput( "\r\nGLToy::Initialise() - Initialising systems:\r\n" );

    GLTOY_INITIALISER_CALL( GLToy_Timer );
    GLTOY_INITIALISER_CALL( GLToy_Maths );

    if( !Platform_EarlyInitialise() )
    {
        return false;
    }

    GLTOY_INITIALISER_CALL( GLToy_Render );

    if( !Platform_LateInitialise() )
    {
        return false;
    }

    if( !Resize( s_iWidth, s_iHeight ) )
    {
        return false;
    }

    GLToy_DebugOutput( "\r\nGLToy::Initialise() - Completed successfully.\r\n" );

    return true;
}

void GLToy::Shutdown()
{
    GLToy_DebugOutput( "\r\nGLToy::Shutdown() - Shutting down systems.\r\n" );

    GLToy_Render::Shutdown();

    Platform_Shutdown();

    GLToy_Maths::Shutdown();

    GLToy_DebugOutput( "\r\nGLToy::Shutdown() - Completed successfully.\r\n" );
}

bool GLToy::MainLoop()
{
    if( !Platform_MainLoop() )
    {
        return false;
    }

    GLToy_Timer::Update();

    GLToy_Render::Render();

    return true;
}

void GLToy::DebugOutput( const char* sFormatString, ... )
{
    va_list xArgumentList;

    va_start( xArgumentList, sFormatString );
    int iMessageLength = _vscprintf( sFormatString, xArgumentList ) + 1;

    vsprintf( sDebugMessageBuffer, /* iMessageLength, */ sFormatString, xArgumentList );

    // we no longer need xArgumentList
    va_end( xArgumentList );

    Platform_DebugOutput( sDebugMessageBuffer );
}




