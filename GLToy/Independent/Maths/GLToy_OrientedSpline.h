/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

#ifndef __GLTOY_ORIENTED_SPLINE_H_
#define __GLTOY_ORIENTED_SPLINE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Maths/GLToy_Spline.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_OrientedSpline
: public virtual GLToy_Spline< GLToy_Vector_3 >
{

	typedef GLToy_Spline< GLToy_Vector_3 > GLToy_Parent;

public:

	GLToy_OrientedSpline()
	: GLToy_Parent()
	, m_xOrientations()
    {
    }

	GLToy_OrientedSpline( const GLToy_OrientedSpline& xSpline )
    : GLToy_Parent( xSpline )
	, m_xOrientations( xSpline.m_xOrientations )
	{
	}

	virtual GLToy_Matrix_3 GetOrientation( const float fParameter ) const
	{
		return OrientationInterpolate( GLToy_Maths::Clamp( fParameter, 0.0f, 1.0f ) * GetLength() );
	}

	GLToy_Inline void AppendOrientation( const GLToy_Matrix_3& xOrientation )
	{
		m_xOrientations.Append( xOrientation );
	}

	GLToy_Inline void AppendOrientation( const GLToy_Vector_3& xDirection )
	{
		m_xOrientations.Append( GLToy_Maths::Orientation_FromDirection( xDirection ) );
	}

    GLToy_Inline void AppendOrientation( const GLToy_Vector_3& xDirection, const GLToy_Vector_3& xUp )
	{
		m_xOrientations.Append( GLToy_Maths::Orientation_FromDirectionAndUp( xDirection, xUp ) );
	}

	GLToy_Inline bool IsComplete() const { return m_xOrientations.GetCount() == GetCount(); }

	// TODO: if its useful implement this?
    //virtual GLToy_Matrix_3 GetOrientationDerivative( const float fParameter ) const
	//{
	//	return GetOrientationDerivativeFromScaledParameter( GLToy_Maths::Clamp( fParameter, 0.0f, 1.0f ) * GetLength() );
	//}

protected:

	virtual GLToy_Matrix_3 OrientationInterpolate( const float fParameter ) const = 0;
	GLToy_Matrix_3 LinearOrientationInterpolate( const float fParameter ) const;
	GLToy_Matrix_3 CubicOrientationInterpolate( const float fParameter ) const;
	GLToy_Matrix_3 CatmullRomOrientationInterpolate( const float fParameter ) const;
	//virtual GLToy_Matrix_3 GetOrientationDerivativeFromScaledParameter( const float fParameter ) const = 0;

	GLToy_Array< GLToy_Matrix_3 > m_xOrientations;

};

class GLToy_OrientedSpline_LinearLinear
: public GLToy_Spline_Linear< GLToy_Vector_3 >
, public GLToy_OrientedSpline
{

protected:

	virtual GLToy_Matrix_3 OrientationInterpolate( const float fParameter ) const
	{
		return LinearOrientationInterpolate( fParameter );
	}

};

class GLToy_OrientedSpline_CubicLinear
: public GLToy_Spline_Cubic< GLToy_Vector_3 >
, public GLToy_OrientedSpline
{

protected:

	virtual GLToy_Matrix_3 OrientationInterpolate( const float fParameter ) const
	{
		return LinearOrientationInterpolate( fParameter );
	}

};

class GLToy_OrientedSpline_CatmullRomLinear
: public GLToy_Spline_CatmullRom< GLToy_Vector_3 >
, public GLToy_OrientedSpline
{

protected:

	virtual GLToy_Matrix_3 OrientationInterpolate( const float fParameter ) const
	{
		return LinearOrientationInterpolate( fParameter );
	}

};

#endif