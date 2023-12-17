#include "dxgi1_6.h"

typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY)(REFIID riid, _COM_Outptr_ void** ppFactory);
typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY_2)(UINT Flags, REFIID riid, _COM_Outptr_ void** ppFactory);
typedef HRESULT(WINAPI* PFN_DECLARE_ADAPTER_REMOVAL_SUPPORT)();
typedef HRESULT(WINAPI* PFN_GET_DEBUG_INTERFACE)(UINT Flags, REFIID riid, void** ppDebug);

HMODULE originalDXGI = NULL;

BOOL DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);

VOID WINAPI AttachToFactory(IUnknown* unkFactory);
VOID WINAPI AttachToAdapter(IUnknown* unkAdapter);
