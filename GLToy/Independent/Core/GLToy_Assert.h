/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GLTOY_ASSERT_H_
#define __GLTOY_ASSERT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
    #define GLToy_Assert( condition, message, ... ) if( _GLToy_Assert( condition, __FILE__, __LINE__, message, __VA_ARGS__ ) ) { __asm int 3 } // TODO - make this platform code
#else
    #define GLToy_Assert( condition, ... )
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const unsigned int uASSERT_MAX_TITLE_LENGTH = 512;
static const unsigned int uASSERT_MAX_MESSAGE_LENGTH = 512;

/////////////////////////////////////////////////////////////////////////////////////////////
// D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool _GLToy_Assert( const bool& bCondition, const char* szFilename, const int& uLineNumber, const char* szFormatString, ... );
bool Platform_GLToy_Assert( const char* szTitle, const char* szMessage );

#endif