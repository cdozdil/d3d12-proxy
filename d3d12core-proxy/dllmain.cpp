// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "dllmain.h"

#define _CRT_SECURE_NO_DEPRECATE
#pragma warning (disable : 4996)

PFN_GET_INTERFACE getInterface;

HMODULE originalD3D12Core = NULL;

void loadOriginalD3D12Core()
{
	originalD3D12Core = LoadLibrary(".\\d3d12core.o.dll");

	if (!originalD3D12Core)
	{
		char dllpath[MAX_PATH];
		GetSystemDirectory(dllpath, MAX_PATH);
		strcat(dllpath, "\\d3d12core.dll");
		LOG(dllpath);
		originalD3D12Core = LoadLibrary(dllpath);
	}

	if (!originalD3D12Core)
	{
		char dllpath[MAX_PATH];
		GetSystemDirectory(dllpath, MAX_PATH);
		strcat(dllpath, "\\D3D12\\d3d12core.dll");
		LOG(dllpath);
		originalD3D12Core = LoadLibrary(dllpath);
	}

	if (!originalD3D12Core)
	{
		LOG("originalD3D12Core: no originalD3D12Core");
		return;
	}

	getInterface = (PFN_GET_INTERFACE)GetProcAddress(originalD3D12Core, "D3D12GetInterface");

	// Get the function address using GetProcAddress
	UINT* pD3D12SDKVersion = reinterpret_cast<UINT*>(GetProcAddress(originalD3D12Core, "D3D12SDKVersion"));

	// Check if the function address was obtained successfully
	if (!pD3D12SDKVersion) 
		LOG("originalD3D12Core: can't read D3D12SDKVersion");
	else
	{
		LOG("originalD3D12Core D3D12SDKVersion: " + std::to_string(*pD3D12SDKVersion));
		sdkVersion = *pD3D12SDKVersion;
	}
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
		prepareOfs(".\\d3d12core-proxy-" + datetime + ".log");
		LOG("d3d12core-proxy DLL_PROCESS_ATTACH");
#endif

		loadOriginalD3D12Core();
		break;

	case DLL_PROCESS_DETACH:
		LOG("d3d12core-proxy DLL_PROCESS_DETACH");

		if (originalD3D12Core)
			FreeLibrary(originalD3D12Core);

#ifdef LOGGING_ACTIVE
		closeOfs();
#endif

		break;
	}

	return TRUE;
}

HRESULT WINAPI D3D12GetInterface(REFCLSID rcslid, REFIID iid, void** debug)
{
	LOG("D3D12GetInterface");

#ifdef LOGGING_ACTIVE
	if(__uuidof(ID3D12Debug) == rcslid)
		LOG("D3D12GetInterface request of D3D12Debug");
	else if(__uuidof(ID3D12Debug1) == rcslid)
		LOG("D3D12GetInterface request of D3D12Debug1");
	else if(__uuidof(ID3D12Debug2) == rcslid)
		LOG("D3D12GetInterface request of D3D12Debug2");
	else if(__uuidof(ID3D12Debug3) == rcslid)
		LOG("D3D12GetInterface request of D3D12Debug3");
	else if(__uuidof(ID3D12Debug4) == rcslid)
		LOG("D3D12GetInterface request of D3D12Debug4");
	else if(__uuidof(ID3D12Debug5) == rcslid)
		LOG("D3D12GetInterface request of D3D12Debug5");
	else if(__uuidof(ID3D12Debug6) == rcslid)
		LOG("D3D12GetInterface request of D3D12Debug5");
	else if(__uuidof(ID3D12DeviceFactory) == rcslid)
		LOG("D3D12GetInterface request of DeviceFactory");
	else if(__uuidof(ID3D12DeviceRemovedExtendedData) == rcslid)
		LOG("D3D12GetInterface request of D3D12DeviceRemovedExtendedData");
	else if(__uuidof(ID3D12DeviceRemovedExtendedData1) == rcslid)
		LOG("D3D12GetInterface request of D3D12DeviceRemovedExtendedData1");
	else if(__uuidof(ID3D12DeviceRemovedExtendedData2) == rcslid)
		LOG("D3D12GetInterface request of D3D12DeviceRemovedExtendedData2");
	else if(__uuidof(ID3D12DeviceRemovedExtendedDataSettings) == rcslid)
		LOG("D3D12GetInterface request of D3D12DeviceRemovedExtendedDataSettings");
	else if(__uuidof(ID3D12SDKConfiguration) == rcslid)
		LOG("D3D12GetInterface request of D3D12SDKConfiguration");
	else if(__uuidof(ID3D12SDKConfiguration1) == rcslid)
		LOG("D3D12GetInterface request of D3D12SDKConfiguration1");
	else if(__uuidof(ID3D12Tools) == rcslid)
		LOG("D3D12GetInterface request of D3D12Tools");
#endif

	if (!originalD3D12Core)
	{
		LOG("D3D12GetInterface: no originalD3D12Core");
		return E_NOINTERFACE;
	}

	HRESULT result = getInterface(rcslid, iid, debug);

	LOG("D3D12GetInterface: " + int_to_hex(result));

	return result;
}

UINT D3D12SDKVersion = 610;
