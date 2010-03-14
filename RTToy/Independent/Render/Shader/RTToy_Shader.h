#ifndef __RTTOY_SHADER_H_
#define __RTTOY_SHADER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Core/GLToy_Hash.h>
#include <String/GLToy_String.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class RTToy_Shader
{

public:

    RTToy_Shader() {}
    virtual ~RTToy_Shader() {}

protected:

    union
    {
        int m_iID;
        u_int m_uUID;
    };
};

class RTToy_ShaderProgram
{

public:

    RTToy_ShaderProgram( const GLToy_String& szName )
    : m_szName( szName )
    {
    }

    virtual ~RTToy_ShaderProgram() {}

    GLToy_Inline GLToy_Hash GetHash() const { return m_szName.GetHash(); }
    GLToy_Inline const GLToy_String& GetName() const { return m_szName; }

    virtual void Bind() {}

protected:

    GLToy_String m_szName;
    union
    {
        int m_iID;
        u_int m_uUID;
    };
};

#endif