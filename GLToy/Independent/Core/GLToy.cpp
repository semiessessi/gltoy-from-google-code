/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/GLToy.h>

// GLToy headers
#include <Core/Console/GLToy_Console.h>
#include <Core/State/GLToy_State.h>
#include <Core/GLToy_Timer.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#include <FridgeScript/GLToy_FridgeScript.h>
#include <Input/GLToy_Input.h>
#include <Maths/GLToy_Maths.h>
#include <Model/GLToy_Model_System.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>

// C/C++ headers
#include <stdarg.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy::s_bFullscreen = false;
int GLToy::s_iWidth = 640;
int GLToy::s_iHeight = 480;

bool GLToy::s_bQuitFlag = false;
bool GLToy::s_bHasFocus = false;

static char szDebugMessageBuffer[ uDEBUGOUTPUT_MAX_LENGTH ];

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

int GLToy::EntryPoint()
{
    Initialise();

    GLToy_Console::RegisterCommand( "quit", Quit );

    GLToy_DebugOutput( "\r\nGLToy::EntryPoint() - Entering Main Loop.\r\n" );

    bool bRunning = true;
    while( bRunning && !s_bQuitFlag )
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

    GLToy_Render::SetViewport( 0, 0, s_iWidth, s_iHeight );

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
    GLTOY_INITIALISER_CALL( GLToy_Input_System );
    GLTOY_INITIALISER_CALL( GLToy_State_System );

    if( !Platform_EarlyInitialise() )
    {
        return false;
    }

    GLTOY_INITIALISER_CALL( GLToy_Render );
    GLTOY_INITIALISER_CALL( GLToy_Console );

    if( !Platform_LateInitialise() )
    {
        return false;
    }

    if( !Resize( s_iWidth, s_iHeight ) )
    {
        return false;
    }

#ifdef GLTOY_USE_FRIDGESCRIPT
    GLTOY_INITIALISER_CALL( GLToy_FridgeScript );
#endif

    GLTOY_INITIALISER_CALL( GLToy_Model_System );
    GLTOY_INITIALISER_CALL( GLToy_Environment_System );
    GLTOY_INITIALISER_CALL( GLToy_Entity_System );

    if( !Project_Initialise() )
    {
        return false;
    }

    GLToy_DebugOutput( "\r\nGLToy::Initialise() - Completed successfully.\r\n" );

    return true;
}

void GLToy::Shutdown()
{
    Project_Shutdown();

    GLToy_DebugOutput( "\r\nGLToy::Shutdown() - Shutting down systems.\r\n" );

    GLToy_Entity_System::Shutdown();
    GLToy_Environment_System::Shutdown();
    GLToy_Model_System::Shutdown();

#ifdef GLTOY_USE_FRIDGESCRIPT
    GLToy_FridgeScript::Shutdown();
#endif

    GLToy_Console::Shutdown();
    GLToy_Render::Shutdown();

    Platform_Shutdown();

    GLToy_State_System::Shutdown();
    GLToy_Maths::Shutdown();

    GLToy_DebugOutput( "\r\nGLToy::Shutdown() - Completed successfully.\r\n" );
}

bool GLToy::MainLoop()
{
    if( !Platform_MainLoop() )
    {
        return false;
    }

    // Update functions
    GLToy_Timer::Update();
    GLToy_Input_System::Update();
    GLToy_Camera::Update();

    GLToy_Environment_System::Update();
    GLToy_Entity_System::Update();

    GLToy_Console::Update();

    // Render functions
    GLToy_Render::BeginRender();

    GLToy_Environment_System::Render();
    GLToy_Entity_System::Render();

    GLToy_Render::Render();

    GLToy_Render::BeginRender2D();

    GLToy_Render::Render2D();
    GLToy_Console::Render();

    GLToy_Render::EndRender();

    return true;
}

void GLToy::DebugOutput( const char* szFormatString, ... )
{
    va_list xArgumentList;

    va_start( xArgumentList, szFormatString );
    int iMessageLength = _vscprintf( szFormatString, xArgumentList ) + 1;

    vsprintf( szDebugMessageBuffer, /* iMessageLength, */ szFormatString, xArgumentList );

    // we no longer need xArgumentList
    va_end( xArgumentList );

    GLToy_Console::Print( szDebugMessageBuffer );
    Platform_DebugOutput( szDebugMessageBuffer );
}

u_int GLToy::GetWindowViewportWidth()
{
    return static_cast<u_int>( s_iWidth );
}

u_int GLToy::GetWindowViewportHeight()
{
    return static_cast<u_int>( s_iHeight );
}



