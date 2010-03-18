#ifndef __GLTOY_SHADER_H_
#define __GLTOY_SHADER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_ShaderProgram
{

public:

    GLToy_ShaderProgram( const u_int uID, const u_int uFSID, const u_int uVSID );
    virtual ~GLToy_ShaderProgram();

    bool IsReady() const { return m_uID != 0; }

    void Bind() const;

	void SetUniform( const GLToy_String& szName, const int iValue );

protected:

    u_int m_uID;
    u_int m_uFSID;
    u_int m_uVSID;
};

#endif