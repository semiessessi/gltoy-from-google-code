/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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
#include <Core/SRC.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>

// SRC
#include <Entity/SRC_EntityTypes.h>
#include <Entity/Robot/SRC_Entity_Robot.h>
#include <Environment/SRC_Environment.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void SRC::CreateTestEnvironment()
{
    GLToy_Entity_System::DestroyEntities();
    GLToy_Environment_System::SwitchEnvironment( GLToy_Environment_System::CreateEnvironmentFromType( uSRC_ENVIRONMENT_TYPE ) );
}

bool SRC::Initialise()
{
    GLToy::ChangeWindowTitle( "Super Robo Cross" );

    GLToy_Entity_System::SetProjectEntityCreateCallback( SRC_CreateEntity );
    GLToy_Environment_System::SetProjectCreateCallback( SRC_CreateEnvironment );

    GLToy_Console::RegisterCommand( "spawnrobot", SRC_Entity_Robot::SpawnRobot_Console );
	GLToy_Console::RegisterCommand( "worldtest", SRC::CreateTestEnvironment );

    GLToy_Console::RegisterCommand( "spawncube", SRC_Console_SpawnCube );

	GLToy_Environment* pxEnvironment = GLToy_Environment_System::CreateEnvironmentFromType( uSRC_ENVIRONMENT_TYPE );
	GLToy_Environment_System::SetCurrentEnvironment( pxEnvironment );
    return true;
}

void SRC::Shutdown()
{
}

void SRC::Update()
{
}
