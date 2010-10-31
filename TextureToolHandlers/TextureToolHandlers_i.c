

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Oct 31 10:43:08 2010
 */
/* Compiler settings for TextureToolHandlers.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IPreview,0x06ECB85D,0x2506,0x4A49,0xA1,0xC9,0x9E,0x7D,0x8A,0x9C,0x14,0xA6);


MIDL_DEFINE_GUID(IID, IID_IThumbnail,0x4B44D4E9,0x5496,0x4B32,0xB1,0x2E,0x91,0x6F,0x32,0x17,0x69,0x3E);


MIDL_DEFINE_GUID(IID, LIBID_TextureToolHandlersLib,0x5F505CFB,0xD000,0x4540,0xBC,0xB4,0x90,0x7C,0xDA,0x29,0xA5,0xD4);


MIDL_DEFINE_GUID(CLSID, CLSID_Preview,0xDD30F1A5,0xE095,0x4558,0xAE,0x42,0x11,0x5A,0xA8,0xE5,0x80,0x2E);


MIDL_DEFINE_GUID(CLSID, CLSID_Thumbnail,0xD023ED4F,0xF1C4,0x43A1,0xBD,0xB7,0xAD,0xDC,0xA4,0x17,0x8E,0x83);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



