// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"
#include "dllmain.h"

//#define CREATE_DX12

#define _CRT_SECURE_NO_DEPRECATE
#pragma warning (disable : 4996)

HMODULE originalD3D11 = NULL;

#pragma region  handles

D3D11_Core_Create_Device coreCreateDevice;
D3D11_Create_Device createDevice;
D3D11_Create_Device_And_Swap_Chain createDeviceAndSwapChain;
D3D11_On_12_Create_Device d3d11on12CreateDevice;

FARPROC D3D11CreateDeviceForD3D12Org;
FARPROC D3DKMTCloseAdapterOrg;
FARPROC D3DKMTDestroyAllocationOrg;
FARPROC D3DKMTDestroyContextOrg;
FARPROC D3DKMTDestroyDeviceOrg;
FARPROC D3DKMTDestroySynchronizationObjectOrg;
FARPROC D3DKMTPresentOrg;
FARPROC D3DKMTQueryAdapterInfoOrg;
FARPROC D3DKMTSetDisplayPrivateDriverFormatOrg;
FARPROC D3DKMTSignalSynchronizationObjectOrg;
FARPROC D3DKMTUnlockOrg;
FARPROC D3DKMTWaitForSynchronizationObjectOrg;
FARPROC EnableFeatureLevelUpgradeOrg;
FARPROC OpenAdapter10Org;
FARPROC OpenAdapter10_2Org;
FARPROC CreateDirect3D11DeviceFromDXGIDeviceOrg;
FARPROC CreateDirect3D11SurfaceFromDXGISurfaceOrg;
FARPROC D3D11CoreCreateLayeredDeviceOrg;
FARPROC D3D11CoreGetLayeredDeviceSizeOrg;
FARPROC D3D11CoreRegisterLayersOrg;
FARPROC D3DKMTCreateAllocationOrg;
FARPROC D3DKMTCreateContextOrg;
FARPROC D3DKMTCreateDeviceOrg;
FARPROC D3DKMTCreateSynchronizationObjectOrg;
FARPROC D3DKMTEscapeOrg;
FARPROC D3DKMTGetContextSchedulingPriorityOrg;
FARPROC D3DKMTGetDeviceStateOrg;
FARPROC D3DKMTGetDisplayModeListOrg;
FARPROC D3DKMTGetMultisampleMethodListOrg;
FARPROC D3DKMTGetRuntimeDataOrg;
FARPROC D3DKMTGetSharedPrimaryHandleOrg;
FARPROC D3DKMTLockOrg;
FARPROC D3DKMTOpenAdapterFromHdcOrg;
FARPROC D3DKMTOpenResourceOrg;
FARPROC D3DKMTQueryAllocationResidencyOrg;
FARPROC D3DKMTQueryResourceInfoOrg;
FARPROC D3DKMTRenderOrg;
FARPROC D3DKMTSetAllocationPriorityOrg;
FARPROC D3DKMTSetContextSchedulingPriorityOrg;
FARPROC D3DKMTSetDisplayModeOrg;
FARPROC D3DKMTSetGammaRampOrg;
FARPROC D3DKMTSetVidPnSourceOwnerOrg;
FARPROC D3DKMTWaitForVerticalBlankEventOrg;
FARPROC D3DPerformance_BeginEventOrg;
FARPROC D3DPerformance_EndEventOrg;
FARPROC D3DPerformance_GetStatusOrg;
FARPROC D3DPerformance_SetMarkerOrg;

extern FARPROC D3D11CreateDeviceForD3D12Org;
extern FARPROC D3DKMTCloseAdapterOrg;
extern FARPROC D3DKMTDestroyAllocationOrg;
extern FARPROC D3DKMTDestroyContextOrg;
extern FARPROC D3DKMTDestroyDeviceOrg;
extern FARPROC D3DKMTDestroySynchronizationObjectOrg;
extern FARPROC D3DKMTPresentOrg;
extern FARPROC D3DKMTQueryAdapterInfoOrg;
extern FARPROC D3DKMTSetDisplayPrivateDriverFormatOrg;
extern FARPROC D3DKMTSignalSynchronizationObjectOrg;
extern FARPROC D3DKMTUnlockOrg;
extern FARPROC D3DKMTWaitForSynchronizationObjectOrg;
extern FARPROC EnableFeatureLevelUpgradeOrg;
extern FARPROC OpenAdapter10Org;
extern FARPROC OpenAdapter10_2Org;
extern FARPROC CreateDirect3D11DeviceFromDXGIDeviceOrg;
extern FARPROC CreateDirect3D11SurfaceFromDXGISurfaceOrg;
extern FARPROC D3D11CoreCreateLayeredDeviceOrg;
extern FARPROC D3D11CoreGetLayeredDeviceSizeOrg;
extern FARPROC D3D11CoreRegisterLayersOrg;
extern FARPROC D3DKMTCreateAllocationOrg;
extern FARPROC D3DKMTCreateContextOrg;
extern FARPROC D3DKMTCreateDeviceOrg;
extern FARPROC D3DKMTCreateSynchronizationObjectOrg;
extern FARPROC D3DKMTEscapeOrg;
extern FARPROC D3DKMTGetContextSchedulingPriorityOrg;
extern FARPROC D3DKMTGetDeviceStateOrg;
extern FARPROC D3DKMTGetDisplayModeListOrg;
extern FARPROC D3DKMTGetMultisampleMethodListOrg;
extern FARPROC D3DKMTGetRuntimeDataOrg;
extern FARPROC D3DKMTGetSharedPrimaryHandleOrg;
extern FARPROC D3DKMTLockOrg;
extern FARPROC D3DKMTOpenAdapterFromHdcOrg;
extern FARPROC D3DKMTOpenResourceOrg;
extern FARPROC D3DKMTQueryAllocationResidencyOrg;
extern FARPROC D3DKMTQueryResourceInfoOrg;
extern FARPROC D3DKMTRenderOrg;
extern FARPROC D3DKMTSetAllocationPriorityOrg;
extern FARPROC D3DKMTSetContextSchedulingPriorityOrg;
extern FARPROC D3DKMTSetDisplayModeOrg;
extern FARPROC D3DKMTSetGammaRampOrg;
extern FARPROC D3DKMTSetVidPnSourceOwnerOrg;
extern FARPROC D3DKMTWaitForVerticalBlankEventOrg;
extern FARPROC D3DPerformance_BeginEventOrg;
extern FARPROC D3DPerformance_EndEventOrg;
extern FARPROC D3DPerformance_GetStatusOrg;
extern FARPROC D3DPerformance_SetMarkerOrg;

#pragma endregion

extern "C" void Proxy_CallOrgFcnAsm(void);
extern "C" FARPROC FcnPtrOrg = NULL;

void loadOriginalD3D11()
{

	originalD3D11 = LoadLibrary(".\\d3d11.o.dll");

	if (!originalD3D11)
	{
		char dllpath[MAX_PATH];
		GetSystemDirectory(dllpath, MAX_PATH);
		strcat(dllpath, "\\d3d11.dll");
		LOG(dllpath);
		originalD3D11 = LoadLibrary(dllpath);
	}

	if (!originalD3D11)
	{
		LOG("loadOriginalD3D11: no originalD3D11");
		return;
	}

#pragma region handles

	coreCreateDevice = (D3D11_Core_Create_Device)GetProcAddress(originalD3D11, "D3D11CoreCreateDevice");
	createDevice = (D3D11_Create_Device)GetProcAddress(originalD3D11, "D3D11CreateDevice");
	createDeviceAndSwapChain = (D3D11_Create_Device_And_Swap_Chain)GetProcAddress(originalD3D11, "D3D11CreateDeviceAndSwapChain");
	d3d11on12CreateDevice = (D3D11_On_12_Create_Device)GetProcAddress(originalD3D11, "D3D11On12CreateDevice");

	D3D11CreateDeviceForD3D12Org = GetProcAddress(originalD3D11, "D3D11CreateDeviceForD3D12");
	D3DKMTCloseAdapterOrg = GetProcAddress(originalD3D11, "D3DKMTCloseAdapter");
	D3DKMTDestroyAllocationOrg = GetProcAddress(originalD3D11, "D3DKMTDestroyAllocation");
	D3DKMTDestroyContextOrg = GetProcAddress(originalD3D11, "D3DKMTDestroyContext");
	D3DKMTDestroyDeviceOrg = GetProcAddress(originalD3D11, "D3DKMTDestroyDevice");
	D3DKMTDestroySynchronizationObjectOrg = GetProcAddress(originalD3D11, "D3DKMTDestroySynchronizationObject");
	D3DKMTPresentOrg = GetProcAddress(originalD3D11, "D3DKMTPresent");
	D3DKMTQueryAdapterInfoOrg = GetProcAddress(originalD3D11, "D3DKMTQueryAdapterInfo");
	D3DKMTSetDisplayPrivateDriverFormatOrg = GetProcAddress(originalD3D11, "D3DKMTSetDisplayPrivateDriverFormat");
	D3DKMTSignalSynchronizationObjectOrg = GetProcAddress(originalD3D11, "D3DKMTSignalSynchronizationObject");
	D3DKMTUnlockOrg = GetProcAddress(originalD3D11, "D3DKMTUnlock");
	D3DKMTWaitForSynchronizationObjectOrg = GetProcAddress(originalD3D11, "D3DKMTWaitForSynchronizationObject");
	EnableFeatureLevelUpgradeOrg = GetProcAddress(originalD3D11, "EnableFeatureLevelUpgrade");
	OpenAdapter10Org = GetProcAddress(originalD3D11, "OpenAdapter10");
	OpenAdapter10_2Org = GetProcAddress(originalD3D11, "OpenAdapter10_2");
	CreateDirect3D11DeviceFromDXGIDeviceOrg = GetProcAddress(originalD3D11, "CreateDirect3D11DeviceFromDXGIDevice");
	CreateDirect3D11SurfaceFromDXGISurfaceOrg = GetProcAddress(originalD3D11, "CreateDirect3D11SurfaceFromDXGISurface");
	D3D11CoreCreateLayeredDeviceOrg = GetProcAddress(originalD3D11, "D3D11CoreCreateLayeredDevice");
	D3D11CoreGetLayeredDeviceSizeOrg = GetProcAddress(originalD3D11, "D3D11CoreGetLayeredDeviceSize");
	D3D11CoreRegisterLayersOrg = GetProcAddress(originalD3D11, "D3D11CoreRegisterLayers");
	D3DKMTCreateAllocationOrg = GetProcAddress(originalD3D11, "D3DKMTCreateAllocation");
	D3DKMTCreateContextOrg = GetProcAddress(originalD3D11, "D3DKMTCreateContext");
	D3DKMTCreateDeviceOrg = GetProcAddress(originalD3D11, "D3DKMTCreateDevice");
	D3DKMTCreateSynchronizationObjectOrg = GetProcAddress(originalD3D11, "D3DKMTCreateSynchronizationObject");
	D3DKMTEscapeOrg = GetProcAddress(originalD3D11, "D3DKMTEscape");
	D3DKMTGetContextSchedulingPriorityOrg = GetProcAddress(originalD3D11, "D3DKMTGetContextSchedulingPriority");
	D3DKMTGetDeviceStateOrg = GetProcAddress(originalD3D11, "D3DKMTGetDeviceState");
	D3DKMTGetDisplayModeListOrg = GetProcAddress(originalD3D11, "D3DKMTGetDisplayModeList");
	D3DKMTGetMultisampleMethodListOrg = GetProcAddress(originalD3D11, "D3DKMTGetMultisampleMethodList");
	D3DKMTGetRuntimeDataOrg = GetProcAddress(originalD3D11, "D3DKMTGetRuntimeData");
	D3DKMTGetSharedPrimaryHandleOrg = GetProcAddress(originalD3D11, "D3DKMTGetSharedPrimaryHandle");
	D3DKMTLockOrg = GetProcAddress(originalD3D11, "D3DKMTLock");
	D3DKMTOpenAdapterFromHdcOrg = GetProcAddress(originalD3D11, "D3DKMTOpenAdapterFromHdc");
	D3DKMTOpenResourceOrg = GetProcAddress(originalD3D11, "D3DKMTOpenResource");
	D3DKMTQueryAllocationResidencyOrg = GetProcAddress(originalD3D11, "D3DKMTQueryAllocationResidency");
	D3DKMTQueryResourceInfoOrg = GetProcAddress(originalD3D11, "D3DKMTQueryResourceInfo");
	D3DKMTRenderOrg = GetProcAddress(originalD3D11, "D3DKMTRender");
	D3DKMTSetAllocationPriorityOrg = GetProcAddress(originalD3D11, "D3DKMTSetAllocationPriority");
	D3DKMTSetContextSchedulingPriorityOrg = GetProcAddress(originalD3D11, "D3DKMTSetContextSchedulingPriority");
	D3DKMTSetDisplayModeOrg = GetProcAddress(originalD3D11, "D3DKMTSetDisplayMode");
	D3DKMTSetGammaRampOrg = GetProcAddress(originalD3D11, "D3DKMTSetGammaRamp");
	D3DKMTSetVidPnSourceOwnerOrg = GetProcAddress(originalD3D11, "D3DKMTSetVidPnSourceOwner");
	D3DKMTWaitForVerticalBlankEventOrg = GetProcAddress(originalD3D11, "D3DKMTWaitForVerticalBlankEvent");
	D3DPerformance_BeginEventOrg = GetProcAddress(originalD3D11, "D3DPerformance_BeginEvent");
	D3DPerformance_EndEventOrg = GetProcAddress(originalD3D11, "D3DPerformance_EndEvent");
	D3DPerformance_GetStatusOrg = GetProcAddress(originalD3D11, "D3DPerformance_GetStatus");
	D3DPerformance_SetMarkerOrg = GetProcAddress(originalD3D11, "D3DPerformance_SetMarker");

#pragma endregion
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
		prepareOfs(".\\d3d11-proxy-" + datetime + ".log");
		LOG("d3d11-proxy DLL_PROCESS_ATTACH");
#endif
		loadOriginalD3D11();
		break;

	case DLL_PROCESS_DETACH:
		LOG("d3d11-proxy DLL_PROCESS_DETACH");

		if (originalD3D11)
			FreeLibrary(originalD3D11);

#ifdef LOGGING_ACTIVE
		closeOfs();
#endif

		break;
	}

	return TRUE;
}

HRESULT WINAPI D3D11CoreCreateDevice(IDXGIFactory* pFactory, IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel)
{
	LOG("D3D11CoreCreateDevice");

	if (!originalD3D11)
	{
		LOG("D3D11CoreCreateDevice: no originalD3D11");
		return DXGI_ERROR_NOT_FOUND;
	}

	HRESULT result;
	result = coreCreateDevice(pFactory, pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel);

	if (result != S_OK)
		LOG("D3D11CoreCreateDevice: " + int_to_hex(result));
	else
		LOG("D3D11CoreCreateDevice: OK!");

	return result;
}

HRESULT WINAPI D3D11CreateDevice(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
{
	LOG("D3D11CreateDevice, FeatureLevels: " + std::to_string(FeatureLevels));

	if (FeatureLevels > 0)
	{
		for (size_t i = 0; i < FeatureLevels; i++)
		{
			LOG("D3D11CreateDevice, pFeatureLevels[" + std::to_string(i) + "]: " + int_to_hex(pFeatureLevels[i]));
		}
	}

	if (!originalD3D11)
	{
		LOG("D3D11CreateDevice: no originalD3D11");
		return DXGI_ERROR_NOT_FOUND;
	}

	HRESULT result;

#ifndef CREATE_DX12
	result = createDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);

	if (result != S_OK)
		LOG("D3D11CreateDevice: " + int_to_hex(result));
	else
		LOG("D3D11CreateDevice: OK!");
#else
	ID3D12Device* d3d12device = nullptr;

	if (FeatureLevels > 0)
	{
		bool ok = false;

		for (size_t i = 0; i < FeatureLevels; i++)
		{
			result = D3D12CreateDevice(pAdapter, pFeatureLevels[i], IID_PPV_ARGS(&d3d12device));
			LOG("D3D11CreateDevice D3D12CreateDevice " + int_to_hex(pFeatureLevels[i]) + " result: " + int_to_hex(result));

			if (result == S_OK)
			{
				ok = true;
				break;
			}
		}

		if (!ok)
		{
			LOG("D3D11CreateDevice: " + int_to_hex(result));
			return result;
		}
	}
	else
	{
		result = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&d3d12device));
		LOG("D3D11CreateDevice D3D12CreateDevice D3D_FEATURE_LEVEL_12_0 result: " + int_to_hex(result));

		if (result != S_OK)
		{

			result = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&d3d12device));
			LOG("D3D11CreateDevice D3D12CreateDevice D3D_FEATURE_LEVEL_11_1 result: " + int_to_hex(result));

			if (result != S_OK)
			{
				result = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12device));
				LOG("D3D11CreateDevice D3D12CreateDevice D3D_FEATURE_LEVEL_11_0 result: " + int_to_hex(result));

				if (result != S_OK)
					return result;
			}
		}
	}

	if (d3d12device == nullptr)
		return E_NOINTERFACE;

	LOG("D3D11CreateDevice d3d12device created!");

	D3D12_COMMAND_QUEUE_DESC desc;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	ID3D12CommandQueue* d3d12queue;
	result = d3d12device->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12queue));

	if (result != S_OK)
	{
		LOG("D3D11CreateDevice can't CreateCommandQueue!");
		return result;
	}

	LOG("D3D11CreateDevice d3d12queue created!");
	result = D3D11On12CreateDevice(d3d12device, Flags, pFeatureLevels, FeatureLevels, (IUnknown**)&d3d12queue, 1, 0, ppDevice, ppImmediateContext, pFeatureLevel);

	//d3d12device->Release();
#endif

	return result;
}

HRESULT WINAPI D3D11CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
{
	LOG("D3D11CreateDeviceAndSwapChain");

	if (ppSwapChain && !pSwapChainDesc)
		return E_INVALIDARG;

	if (!originalD3D11)
	{
		LOG("D3D11CreateDeviceAndSwapChain: no originalD3D11");
		return DXGI_ERROR_NOT_FOUND;
	}

	HRESULT result;

#ifndef CREATE_DX12
	result = createDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);
	LOG("D3D11CreateDeviceAndSwapChain result: " + int_to_hex(result));
	return result;
#else
	if (ppSwapChain && !pSwapChainDesc)
		return E_INVALIDARG;

	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11Context;

	result = D3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, &d3d11Device, pFeatureLevel, &d3d11Context);
	LOG("D3D11CreateDeviceAndSwapChain for Dx12 result: " + int_to_hex(result));

	if (ppSwapChain)
	{
		IDXGIDevice* dxgiDevice = nullptr;
		IDXGIAdapter* dxgiAdapter = nullptr;
		IDXGIFactory* dxgiFactory = nullptr;

		result = d3d11Device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
		LOG("D3D11CreateDeviceAndSwapChain d3d11Device->QueryInterface result: " + int_to_hex(result));

		if (result != S_OK)
			return E_INVALIDARG;

		result = dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter));
		dxgiDevice->Release();
		LOG("D3D11CreateDeviceAndSwapChain dxgiDevice->GetParent result: " + int_to_hex(result));

		if (result != S_OK)
			return E_INVALIDARG;

		result = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
		dxgiAdapter->Release();
		LOG("D3D11CreateDeviceAndSwapChain dxgiAdapter->GetParent result: " + int_to_hex(result));

		if (result != S_OK)
			return E_INVALIDARG;

		DXGI_SWAP_CHAIN_DESC desc = *pSwapChainDesc;
		result = dxgiFactory->CreateSwapChain(d3d11Device, &desc, ppSwapChain);
		dxgiFactory->Release();
		LOG("D3D11CreateDeviceAndSwapChain dxgiFactory->CreateSwapChain result: " + int_to_hex(result));

		if (result != S_OK)
			return result;
	}

	if (ppDevice != nullptr)
		*ppDevice = d3d11Device;
	else
		d3d11Device->Release();

	if (ppImmediateContext != nullptr)
		*ppImmediateContext = d3d11Context;
	else
		d3d11Context->Release();

	return S_OK;
#endif

}

HRESULT WINAPI D3D11On12CreateDevice(IUnknown* pDevice, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, IUnknown* const* ppCommandQueues, UINT NumQueues, UINT NodeMask, ID3D11Device** ppDevice, ID3D11DeviceContext** ppImmediateContext, D3D_FEATURE_LEVEL* pChosenFeatureLevel)
{
	LOG("D3D11On12CreateDevice");

	if (!originalD3D11)
	{
		LOG("D3D11On12CreateDevice: no originalD3D11");
		return DXGI_ERROR_NOT_FOUND;
	}

	auto result = d3d11on12CreateDevice(pDevice, Flags, pFeatureLevels, FeatureLevels, ppCommandQueues, NumQueues, NodeMask, ppDevice, ppImmediateContext, pChosenFeatureLevel);

	if (result == S_OK)
		LOG("D3D11On12CreateDevice: " + int_to_hex(result));
	else
		LOG("D3D11On12CreateDevice: OK!");

	return result;
}

extern "C" {
	void D3D11CreateDeviceForD3D12() { FcnPtrOrg = D3D11CreateDeviceForD3D12Org; Proxy_CallOrgFcnAsm(); }
	void D3DKMTCloseAdapter() { FcnPtrOrg = D3DKMTCloseAdapterOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTDestroyAllocation() { FcnPtrOrg = D3DKMTDestroyAllocationOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTDestroyContext() { FcnPtrOrg = D3DKMTDestroyContextOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTDestroyDevice() { FcnPtrOrg = D3DKMTDestroyDeviceOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTDestroySynchronizationObject() { FcnPtrOrg = D3DKMTDestroySynchronizationObjectOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTPresent() { FcnPtrOrg = D3DKMTPresentOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTQueryAdapterInfo() { FcnPtrOrg = D3DKMTQueryAdapterInfoOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTSetDisplayPrivateDriverFormat() { FcnPtrOrg = D3DKMTSetDisplayPrivateDriverFormatOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTSignalSynchronizationObject() { FcnPtrOrg = D3DKMTSignalSynchronizationObjectOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTUnlock() { FcnPtrOrg = D3DKMTUnlockOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTWaitForSynchronizationObject() { FcnPtrOrg = D3DKMTWaitForSynchronizationObjectOrg; Proxy_CallOrgFcnAsm(); }
	void EnableFeatureLevelUpgrade() { FcnPtrOrg = EnableFeatureLevelUpgradeOrg; Proxy_CallOrgFcnAsm(); }
	void OpenAdapter10() { FcnPtrOrg = OpenAdapter10Org; Proxy_CallOrgFcnAsm(); }
	void OpenAdapter10_2() { FcnPtrOrg = OpenAdapter10_2Org; Proxy_CallOrgFcnAsm(); }
	void CreateDirect3D11DeviceFromDXGIDevice() { FcnPtrOrg = CreateDirect3D11DeviceFromDXGIDeviceOrg; Proxy_CallOrgFcnAsm(); }
	void CreateDirect3D11SurfaceFromDXGISurface() { FcnPtrOrg = CreateDirect3D11SurfaceFromDXGISurfaceOrg; Proxy_CallOrgFcnAsm(); }
	void D3D11CoreCreateLayeredDevice() { FcnPtrOrg = D3D11CoreCreateLayeredDeviceOrg; Proxy_CallOrgFcnAsm(); }
	void D3D11CoreGetLayeredDeviceSize() { FcnPtrOrg = D3D11CoreGetLayeredDeviceSizeOrg; Proxy_CallOrgFcnAsm(); }
	void D3D11CoreRegisterLayers() { FcnPtrOrg = D3D11CoreRegisterLayersOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTCreateAllocation() { FcnPtrOrg = D3DKMTCreateAllocationOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTCreateContext() { FcnPtrOrg = D3DKMTCreateContextOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTCreateDevice() { FcnPtrOrg = D3DKMTCreateDeviceOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTCreateSynchronizationObject() { FcnPtrOrg = D3DKMTCreateSynchronizationObjectOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTEscape() { FcnPtrOrg = D3DKMTEscapeOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTGetContextSchedulingPriority() { FcnPtrOrg = D3DKMTGetContextSchedulingPriorityOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTGetDeviceState() { FcnPtrOrg = D3DKMTGetDeviceStateOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTGetDisplayModeList() { FcnPtrOrg = D3DKMTGetDisplayModeListOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTGetMultisampleMethodList() { FcnPtrOrg = D3DKMTGetMultisampleMethodListOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTGetRuntimeData() { FcnPtrOrg = D3DKMTGetRuntimeDataOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTGetSharedPrimaryHandle() { FcnPtrOrg = D3DKMTGetSharedPrimaryHandleOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTLock() { FcnPtrOrg = D3DKMTLockOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTOpenAdapterFromHdc() { FcnPtrOrg = D3DKMTOpenAdapterFromHdcOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTOpenResource() { FcnPtrOrg = D3DKMTOpenResourceOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTQueryAllocationResidency() { FcnPtrOrg = D3DKMTQueryAllocationResidencyOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTQueryResourceInfo() { FcnPtrOrg = D3DKMTQueryResourceInfoOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTRender() { FcnPtrOrg = D3DKMTRenderOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTSetAllocationPriority() { FcnPtrOrg = D3DKMTSetAllocationPriorityOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTSetContextSchedulingPriority() { FcnPtrOrg = D3DKMTSetContextSchedulingPriorityOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTSetDisplayMode() { FcnPtrOrg = D3DKMTSetDisplayModeOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTSetGammaRamp() { FcnPtrOrg = D3DKMTSetGammaRampOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTSetVidPnSourceOwner() { FcnPtrOrg = D3DKMTSetVidPnSourceOwnerOrg; Proxy_CallOrgFcnAsm(); }
	void D3DKMTWaitForVerticalBlankEvent() { FcnPtrOrg = D3DKMTWaitForVerticalBlankEventOrg; Proxy_CallOrgFcnAsm(); }
	void D3DPerformance_BeginEvent() { FcnPtrOrg = D3DPerformance_BeginEventOrg; Proxy_CallOrgFcnAsm(); }
	void D3DPerformance_EndEvent() { FcnPtrOrg = D3DPerformance_EndEventOrg; Proxy_CallOrgFcnAsm(); }
	void D3DPerformance_GetStatus() { FcnPtrOrg = D3DPerformance_GetStatusOrg; Proxy_CallOrgFcnAsm(); }
	void D3DPerformance_SetMarker() { FcnPtrOrg = D3DPerformance_SetMarkerOrg; Proxy_CallOrgFcnAsm(); }
}
