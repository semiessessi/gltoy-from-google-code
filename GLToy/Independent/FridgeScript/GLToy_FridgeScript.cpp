/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <FridgeScript/GLToy_FridgeScript.h>


// GLToy headers
#include <File/GLToy_ANSITextFile.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

u_int GLToy_FridgeScript::s_uContextID = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_FridgeScript::Initialise()
{
    s_uContextID = FSCreateContext();
    return true;
}

void GLToy_FridgeScript::Shutdown()
{
    FSDestroyContext( s_uContextID );
}

void GLToy_FridgeScript::RegisterAPI( char* szName, void* pfnCallback, u_int uParameterCount )
{
    FSRegisterAPI( s_uContextID, szName, pfnCallback, uParameterCount );
}

GLToy_FridgeScript* GLToy_FridgeScript::CreateFromFile( const char* const szFilename )
{
    GLToy_FridgeScript* pxFridgeScript = new GLToy_FridgeScript();

    pxFridgeScript->CompileFromFile( szFilename );

    return pxFridgeScript;
}

GLToy_FridgeScript::GLToy_FridgeScript()
: m_uCodeID( 0 )
{   
}

GLToy_FridgeScript::~GLToy_FridgeScript()
{
}

void GLToy_FridgeScript::CompileFromFile( const char* const szFilename )
{
    GLToy_ANSITextFile xFile = GLToy_ANSITextFile( szFilename );
    u_int uLength = xFile.GetSize();
    
    char* pcData = new char[ uLength ];
    xFile.GetString( pcData );

    m_uCodeID = FSCompile( s_uContextID, pcData );
}

void GLToy_FridgeScript::Execute()
{
    FSExecute( s_uContextID, m_uCodeID );
}