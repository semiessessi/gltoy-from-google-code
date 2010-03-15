/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/FSDemo.h>

// This file's header
#include <FridgeScript/FSDemo_FridgeScript.h>


// GLToy headers
#include <File/GLToy_ANSITextFile.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

u_int FSDemo_FridgeScript::s_uContextID = 0;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool FSDemo_FridgeScript::Initialise()
{
    s_uContextID = FSCreateContext();
    return true;
}

void FSDemo_FridgeScript::Shutdown()
{
    FSDestroyContext( s_uContextID );
}

void FSDemo_FridgeScript::RegisterAPI( char* szName, void* pfnCallback, u_int uParameterCount )
{
    FSRegisterAPI( s_uContextID, szName, pfnCallback, uParameterCount );
}

FSDemo_FridgeScript* FSDemo_FridgeScript::CreateFromFile( const char* const szFilename )
{
    FSDemo_FridgeScript* pxFridgeScript = new FSDemo_FridgeScript();

    pxFridgeScript->CompileFromFile( szFilename );

    return pxFridgeScript;
}

FSDemo_FridgeScript::FSDemo_FridgeScript()
: m_uCodeID( 0 )
{   
}

FSDemo_FridgeScript::~FSDemo_FridgeScript()
{
}

void FSDemo_FridgeScript::CompileFromFile( const char* const szFilename )
{
    GLToy_ANSITextFile xFile = GLToy_ANSITextFile( szFilename );
    u_int uLength = xFile.GetSize();
    
    char* pcData = new char[ uLength ];
    xFile.GetRawString( pcData );

    m_uCodeID = FSCompile( s_uContextID, pcData );
}

void FSDemo_FridgeScript::Execute()
{
    FSExecute( s_uContextID, m_uCodeID );
}