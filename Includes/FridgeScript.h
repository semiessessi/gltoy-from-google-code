#ifndef __FridgeScript_h
#define __FridgeScript_h

#define _import extern "C" unsigned int __declspec(dllimport) __stdcall
#define _importptr extern "C" float __declspec(dllimport)* __stdcall
#define _importval extern "C" float __declspec(dllimport) __stdcall

#define FRIDGE_API __stdcall

_import FSCreateContext();
_import FSDestroyContext( unsigned int context );

_import FSGetVariableHandle( unsigned int context, unsigned int codeHandle, char* name );
_importval FSGetVariableValue( unsigned int varHandle );
_importval FSSetVariableValue( unsigned int varHandle, float value );

_import FSRegisterAPI0f( unsigned int context, char* name, float ( FRIDGE_API * pfnCallback0f )() );
_import FSRegisterAPI1f( unsigned int context, char* name, float ( FRIDGE_API * pfnCallback1f )( float ) );
_import FSRegisterAPI2f( unsigned int context, char* name, float ( FRIDGE_API * pfnCallback2f )( float, float ) );
_import FSRegisterAPI3f( unsigned int context, char* name, float ( FRIDGE_API * pfnCallback1f )( float, float, float ) );
_import FSRegisterAPI4f( unsigned int context, char* name, float ( FRIDGE_API * pfnCallback2f )( float, float, float, float ) );
_import FSRegisterAPI( unsigned int context, char* name, void* pCallback, unsigned int uParameterCount );

_import FSCompile( unsigned int context, char* code );
_import FSExecute( unsigned int context, unsigned int codeHandle );

#undef _import
#undef _importptr
#undef _importval

#endif