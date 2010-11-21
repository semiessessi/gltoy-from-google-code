/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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

// This file's header
#include <Core/GLToy.h>

// GLToy headers
#include <Core/Console/GLToy_Console.h>
#include <Core/State/GLToy_State_System.h>
#include <Core/GLToy_Timer.h>
#ifndef GLTOY_DEMO
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#endif
#ifdef GLTOY_USE_FRIDGESCRIPT
#include <FridgeScript/GLToy_FridgeScript.h>
#endif
#include <Input/GLToy_Input_System.h>
#include <Maths/GLToy_Maths.h>
#ifndef GLTOY_DEMO
#include <Model/GLToy_Model_System.h>
#include <Particle/GLToy_PFX_System.h>
#include <Physics/GLToy_Physics_System.h>
#endif
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Render.h>
#ifndef GLTOY_DEMO
#include <Sound/GLToy_Sound_System.h>
#include <Test/GLToy_Test_System.h>
#include <UI/GLToy_UI_System.h>
#endif

// C/C++ headers
#include <stdarg.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy::s_bFullscreen = false;
int GLToy::s_iWidth = 800;
int GLToy::s_iHeight = 600;

bool GLToy::s_bQuitFlag = false;
bool GLToy::s_bHasFocus = false;
bool GLToy::s_bEscapeQuits = true;

bool GLToy::s_bSilent = false;

void ( *GLToy::s_pfnDebugOutputCallback )( const char* const szMessage ) = NULL;

static char g_szDebugMessageBuffer[ uDEBUGOUTPUT_MAX_LENGTH ];

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy::SilentEntryPoint()
{
    s_bSilent = true;
    Initialise();
}

void GLToy::SilentShutdown()
{
    Shutdown();
}

int GLToy::EntryPoint()
{
    if( !Initialise() )
    {
        return 0;
    }

    GLToy_DebugOutput( "\r\nGLToy::EntryPoint() - Running tests.\r\n" );

#ifndef GLTOY_FINAL
    GLToy_Test_System::RunTests();
#endif

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
    if( GLToy_Render::IsVSyncEnabled() )
    {
        Platform_UpdateBuffers();
    }
}

bool GLToy::Initialise()
{
    GLToy_DebugOutput( "\r\nGLToy::Initialise() - Initialising systems:\r\n" );

    GLToy::GiveFocus();

    GLToy_InitialiserCall( GLToy_Memory );
    GLToy_InitialiserCall( GLToy_Maths );
    
    if( !s_bSilent )
    {
        GLToy_InitialiserCall( GLToy_Input_System );
    }

    if( !Platform_EarlyInitialise() )
    {
        return false;
    }

    GLToy_InitialiserCall( GLToy_Render );
    GLToy_InitialiserCall( GLToy_Console );

#ifndef GLTOY_DEMO
    if( !s_bSilent )
    {
        GLToy_InitialiserCall( GLToy_UI_System );
    }
#endif

    GLToy_InitialiserCall( GLToy_State_System );
#ifndef GLTOY_DEMO
    if( !s_bSilent )
    {
        GLToy_InitialiserCall( GLToy_Physics_System );
        GLToy_InitialiserCall( GLToy_Sound_System );
    }
#endif

    if( !Platform_LateInitialise() )
    {
        return false;
    }

    if( !Resize( s_iWidth, s_iHeight ) )
    {
        return false;
    }

#ifdef GLTOY_USE_FRIDGESCRIPT
    GLToy_InitialiserCall( GLToy_FridgeScript );
#endif

#ifndef GLTOY_DEMO
    GLToy_InitialiserCall( GLToy_Model_System );
    GLToy_InitialiserCall( GLToy_Environment_System );
    GLToy_InitialiserCall( GLToy_Entity_System );
    GLToy_InitialiserCall( GLToy_PFX_System );
#endif

    GLToy::ChangeWindowIcon( "Icons/GLToy_24x24.png" ); // here so the project can override immediately if it wants to

    if( !Project_Initialise() )
    {
        return false;
    }

    GLToy_InitialiserCall( GLToy_Timer ); // always initialise last

    GLToy_DebugOutput( "\r\nGLToy::Initialise() - Completed successfully.\r\n" );

    return true;
}

void GLToy::Shutdown()
{
    Project_Shutdown();

    GLToy_DebugOutput( "\r\nGLToy::Shutdown() - Shutting down systems.\r\n" );
#ifndef GLTOY_DEMO
    GLToy_PFX_System::Shutdown();
    GLToy_Entity_System::Shutdown();
    GLToy_Environment_System::Shutdown();
    GLToy_Model_System::Shutdown();
#endif

#ifdef GLTOY_USE_FRIDGESCRIPT
    GLToy_FridgeScript::Shutdown();
#endif

#ifndef GLTOY_DEMO
    if( !s_bSilent )
    {
        GLToy_Physics_System::Shutdown();
        GLToy_Sound_System::Shutdown();
    }
#endif

    GLToy_State_System::Shutdown();

#ifndef GLTOY_DEMO
    if( !s_bSilent )
    {
        GLToy_UI_System::Shutdown();
    }
#endif

    GLToy_Console::Shutdown();
    GLToy_Render::Shutdown();

    Platform_Shutdown();

    GLToy_Maths::Shutdown();

    GLToy_Memory::Shutdown();

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
    GLToy_State_System::Update();

    GLToy_Console::Update();
#ifndef GLTOY_DEMO
    GLToy_UI_System::Update();
    GLToy_Physics_System::Update();

    GLToy_Environment_System::Update();
    GLToy_Entity_System::Update();
    GLToy_PFX_System::Update();

    GLToy_Sound_System::Update();
#endif

    Project_Update();

    // hopefully this will avoid a frame of camera lag if it stays here...
    GLToy_Camera::Update();

    // Render functions
    GLToy_Render::BeginRender();

#ifndef GLTOY_DEMO
    GLToy_Environment_System::Render();
    GLToy_Entity_System::Render();
    GLToy_PFX_System::Render();
#endif

    GLToy_State_System::Render();
    GLToy_Render::Render();

    GLToy_Render::BeginRender2D();

#ifndef GLTOY_DEMO
    GLToy_Environment_System::Render2D();
#endif

    GLToy_State_System::Render2D();

#ifndef GLTOY_DEMO
    GLToy_UI_System::Render2D();
#endif

    GLToy_Render::Render2D();

#ifndef GLTOY_DEMO
	GLToy_Console::Render2D();
#endif

    GLToy_Render::EndRender();

    return true;
}

void GLToy::DebugOutput( const GLToy_String& szString )
{
#ifndef GLTOY_DEMO
    const char* const szANSI = szString.CreateANSIString();
    DebugOutput( szANSI );
    delete[] szANSI;
#endif
}

void GLToy::DebugOutput( const char* szFormatString, ... )
{
#ifndef GLTOY_DEMO
    va_list xArgumentList;

    va_start( xArgumentList, szFormatString );
    int iMessageLength = _vscprintf( szFormatString, xArgumentList ) + 1;

    if( iMessageLength >= uDEBUGOUTPUT_MAX_LENGTH )
    {
        iMessageLength = uDEBUGOUTPUT_MAX_LENGTH - 1;
        // TODO: this might get annoying, particularly with messages from shader compilers or Havok, which are often huge
        // GLToy_Assert( iMessageLength < uDEBUGOUTPUT_MAX_LENGTH, "Debug ouput message too long (%d chars)! Truncating...", iMessageLength );
    }

    vsnprintf( g_szDebugMessageBuffer, iMessageLength, szFormatString, xArgumentList );

    // we no longer need xArgumentList
    va_end( xArgumentList );

    GLToy_Console::Print( g_szDebugMessageBuffer );
    Platform_DebugOutput( g_szDebugMessageBuffer );

    if( s_pfnDebugOutputCallback )
    {
        s_pfnDebugOutputCallback( g_szDebugMessageBuffer );
    }
#endif
}

u_int GLToy::GetWindowViewportWidth()
{
    return static_cast<u_int>( s_iWidth );
}

u_int GLToy::GetWindowViewportHeight()
{
    return static_cast<u_int>( s_iHeight );
}

void GLToy::QuitDialog()
{
#ifndef GLTOY_DEMO
	GLToy_UI_System::ShowQuitDialog();
#else
    Quit();
#endif
}

void GLToy::ChangeWindowTitle( const char* const szNewTitle )
{
    Platform_ChangeWindowTitle( szNewTitle );
}

void GLToy::ChangeWindowIcon( const char* const szTextureName )
{
    Platform_ChangeWindowIcon( szTextureName );
}
