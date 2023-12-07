#include "pch.h"
#include "dllmain.h"
#include "WrappedDXGIObjects.h"

#define _CRT_SECURE_NO_DEPRECATE
#pragma warning (disable : 4996)

PFN_CREATE_DXGI_FACTORY createDxgiFactory;
PFN_CREATE_DXGI_FACTORY createDxgiFactory1;
PFN_CREATE_DXGI_FACTORY_2 createDxgiFactory2;
PFN_DECLARE_ADAPTER_REMOVAL_SUPPORT declareAdapterRemovalSupport;
PFN_GET_DEBUG_INTERFACE getDebugInterface;

void loadOriginalDXGI()
{
	originalDXGI = LoadLibrary(".\\dxgi.o.dll");

	if (!originalDXGI)
	{
		char dllpath[MAX_PATH];
		GetSystemDirectory(dllpath, MAX_PATH);
		strcat(dllpath, "\\dxgi.dll");
		LOG(dllpath);
		originalDXGI = LoadLibrary(dllpath);
	}

	if (!originalDXGI)
	{
		LOG("loadOriginalDXGI: no originalDXGI");
		return;
	}

	createDxgiFactory = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(originalDXGI, "CreateDXGIFactory");
	createDxgiFactory1 = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(originalDXGI, "CreateDXGIFactory1");
	createDxgiFactory2 = (PFN_CREATE_DXGI_FACTORY_2)GetProcAddress(originalDXGI, "CreateDXGIFactory2");
	declareAdapterRemovalSupport = (PFN_DECLARE_ADAPTER_REMOVAL_SUPPORT)GetProcAddress(originalDXGI, "DXGIDeclareAdapterRemovalSupport");
	getDebugInterface = (PFN_GET_DEBUG_INTERFACE)GetProcAddress(originalDXGI, "DXGIGetDebugInterface1");
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
#ifdef LOGGING_ACTIVE
	std::time_t t = std::time(nullptr);
	std::string datetime(100, 0);
#endif

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
#ifdef LOGGING_ACTIVE
		datetime.resize(std::strftime(&datetime[0], datetime.size(), "%d%m%Y_%H%M%OS", std::localtime(&t)));
		prepareOfs(".\\dxgi-proxy-" + datetime + ".log");
		LOG("dxgi-proxy DLL_PROCESS_ATTACH");
#endif
		loadOriginalDXGI();
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		LOG("dxgi-proxy DLL_PROCESS_DETACH");

		if (originalDXGI)
			FreeLibrary(originalDXGI);

#ifdef LOGGING_ACTIVE
		closeOfs();
#endif

		//BMF_UnInit_MinHook();

		break;
	}

	return TRUE;
}

HRESULT WINAPI CreateDXGIFactory(REFIID riid, _COM_Outptr_ void** ppFactory)
{
	LOG("CreateDXGIFactory");

	if (!originalDXGI)
	{
		LOG("CreateDXGIFactory: no originalDXGI");
		return E_NOINTERFACE;
	}

	HRESULT result = createDxgiFactory(riid, ppFactory);

	if (SUCCEEDED(result))
	{
		RefCountDXGIObject::HandleWrap("CreateDXGIFactory", riid, ppFactory);
		LOG("CreateDXGIFactory: Factory created");
	}

	return result;
}

HRESULT WINAPI CreateDXGIFactory1(REFIID riid, _COM_Outptr_ void** ppFactory)
{
	LOG("CreateDXGIFactory1");

	if (!originalDXGI)
	{
		LOG("CreateDXGIFactory1: no originalDXGI");
		return E_NOINTERFACE;
	}

	HRESULT result = createDxgiFactory1(riid, ppFactory);

	if (SUCCEEDED(result))
	{
		RefCountDXGIObject::HandleWrap("CreateDXGIFactory1", riid, ppFactory);
		LOG("CreateDXGIFactory1: Factory created");
	}

	return result;
}

HRESULT WINAPI CreateDXGIFactory2(UINT Flags, REFIID riid, _COM_Outptr_ void** ppFactory)
{
	LOG("CreateDXGIFactory2: " + ToString(riid));

	if (!originalDXGI)
	{
		LOG("CreateDXGIFactory2: no originalDXGI");
		return E_NOINTERFACE;
	}

	HRESULT result = createDxgiFactory2(Flags, riid, ppFactory);

	if (SUCCEEDED(result))
	{
		RefCountDXGIObject::HandleWrap("CreateDXGIFactory1", riid, ppFactory);
		LOG("CreateDXGIFactory2: Factory created");
	}

	return result;
}

HRESULT WINAPI DXGIDeclareAdapterRemovalSupport()
{
	LOG("DXGIDeclareAdapterRemovalSupport");

	if (!originalDXGI)
	{
		LOG("DXGIDeclareAdapterRemovalSupport: no originalDXGI");
		return E_NOINTERFACE;
	}

	return declareAdapterRemovalSupport();
}

HRESULT WINAPI DXGIGetDebugInterface1(UINT Flags, REFIID riid, void** ppDebug)
{
	LOG("DXGIGetDebugInterface1");

	if (!originalDXGI)
	{
		LOG("DXGIGetDebugInterface1: no originalDXGI");
		return E_NOINTERFACE;
	}

	return getDebugInterface(Flags, riid, ppDebug);
}
