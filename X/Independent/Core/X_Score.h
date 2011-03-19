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

#include "Maths/GLToy_Maths.h"

#define X_SCORE_MAX_NOTIFICATIONS 64

class X_Score_Graphic
{
	public:
	
		X_Score_Graphic();
		
		void Update();
		void Render();

		void Activate( float fScore, GLToy_Vector_2 xPosition );
		bool IsActive() { return m_bActive; }

		GLToy_Vector_2 GetPosition() { return m_xPosition; }
		GLToy_Vector_2 GetDestPosition() { return m_xDestPosition; }
		float GetScore() { return m_fScore; }

		void Renew( float fAddScore, GLToy_Vector_2 xPosition );

	private:

		GLToy_Vector_2 m_xPosition;
		GLToy_Vector_2 m_xDestPosition;
		float m_fScore;
		float m_fLifeTime;
		bool m_bActive;
};

class X_Score
{
	public:

		static void Initialise();
		static void Shutdown();
		
		static void Update();
		static void Render();

		static float GetScore() { return s_fScore; }
		static float GetMultiplier() { return s_fMultiplier; }

		static void Add( float fScore, bool bIgnoreMultiplier = false );
		static void Add( float fScore, GLToy_Vector_2 xPosition, bool bIgnoreMultiplier = false );

	private:

		static void AddNotification( float fScore, GLToy_Vector_2 xPosition );

		static float s_fMultiplier;
		static float s_fScore;

		static float s_fChainTimer;

		static X_Score_Graphic s_xNotifications[ X_SCORE_MAX_NOTIFICATIONS ];
		static u_int s_uCurrentNotification;
};

// eof
