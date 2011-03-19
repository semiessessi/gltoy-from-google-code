/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Thomas Young
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

#ifndef __X_WAVE_MANAGER_H_
#define __X_WAVE_MANAGER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
// #include <Entity/GLToy_Entity.h>

// GLToy
// #include <Core/Data Structures/GLToy_Array.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////////////////////

#define X_WAVE_MANAGER_MAX_TIMED_SPAWNERS 32

/////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
/////////////////////////////////////////////////////////////////////////////////////////////

// class X_Enemy_Brain;
// class X_Enemy_Spawner;
class X_Timed_Spawner;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class X_Wave_Manager
{
	public:

		static void Inititalise();
		static void Shutdown();
		static void Update();
		static void Render();

	private:

		static void NextWave();

		static X_Timed_Spawner* s_apxTimedSpawners[ X_WAVE_MANAGER_MAX_TIMED_SPAWNERS ];
		static u_int s_uWave;
		static float s_fWaveTimer;
		static bool s_bRestPeriod;
};

#endif