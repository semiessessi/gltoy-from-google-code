/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// This file's header
#include <Core/JD1.h>

// Win32
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _CRINKLER
int WinMainCRTStartup()
#else
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
#endif
{
    return GLToy::EntryPoint();
}

// SE - 05/11/2010 - although it only saves 2-3K I've been trying to get an own entry point to work for a while... currently this breaks

// its obviously relying on something being initialised that isn't done here...

// I think these same problems make it difficult to use crinkler as well... mainly it seems like the compiler is fairly
// dependent on the entry point function

// anyway if you want to try this set the entry point to GLToy_DemoEntryPoint in the JD1 - Final (Demo) configuration and uncomment the code

/*

#ifdef GLTOY_DEMO
#undef _get_heap_handle

typedef int ( *_PVFV )();

// SE - 05/11/2010 - this is all borrowed from the crt implementation
extern "C"
{
    // for _heap_init
    HANDLE xHeap = NULL;

    // for _cinit - from cinitexe.c
    _CRTALLOC(".CRT$XIA") _PVFV __xi_a[] = { NULL };

    _CRTALLOC(".CRT$XIZ") _PVFV __xi_z[] = { NULL };

    _CRTALLOC(".CRT$XCA") _PVFV __xc_a[] = { NULL };

    _CRTALLOC(".CRT$XCZ") _PVFV __xc_z[] = { NULL };

    #pragma comment(linker, "/merge:.CRT=.rdata")

    intptr_t GLToy_ForceCDecl _get_heap_handle()
    {
        return ( intptr_t )( xHeap );
    }
}

int GLToy_ForceCDecl GLToy_DemoEntryPoint()
{
    int iReturnValue = 0;

    // this is the original init code from __tmainCRTStartup with all the bloat removed for CLR and getting WinMain parameters...
    //int initret;

    //if ( !_heap_init() )
    //    fast_error_exit(_RT_HEAPINIT);


    //if( !_mtinit() )
    //    fast_error_exit(_RT_THREAD);

    //_CrtSetCheckCount(TRUE);


    //if ( _ioinit() < 0 )
    //    _amsg_exit(_RT_LOWIOINIT);

    //initret = _cinit(TRUE);
    //if (initret != 0)
    //    _amsg_exit(initret);


    // _heap_init
    xHeap = HeapCreate( 0, 4096, 0 ); // 4K page size

    // TODO: _mtinit if we actually need it... (might explain crashes on start up if we do)

    // TODO: _ioinit is redundant since we do no file operations

    // _cinit - I'm not sure this implementation does anything valuable - really not sure
    // _cinit : iReturnValue = _initterm_e( __xi_a, __xi_z );
    _PVFV* ppfnBegin = __xi_a;
    _PVFV* ppfnEnd = __xi_z;
    while( ( ppfnBegin < ppfnEnd ) && ( iReturnValue == 0 ) )
    {
        if( *ppfnBegin != NULL )
        {
            iReturnValue = ( **ppfnBegin )();
        }
        ++ppfnBegin;
    }

    if ( iReturnValue != 0 )
    {
        return iReturnValue;
    }

    // _cinit : _initterm( __xc_a, __xc_z );
    ppfnBegin = __xc_a;
    ppfnEnd = __xc_z;
    while( ( ppfnBegin < ppfnEnd ) && ( iReturnValue == 0 ) )
    {
        if( *ppfnBegin != NULL )
        {
            iReturnValue = ( **ppfnBegin )();
        }
        ++ppfnBegin;
    }

    iReturnValue = WinMain( 0, 0, 0, SW_SHOWDEFAULT );

    // _heap_term
    HeapDestroy( xHeap );
    xHeap = NULL;

    return iReturnValue;
}

#define _get_heap_handle GLToy_CRT_Hack_1

#endif

*/
