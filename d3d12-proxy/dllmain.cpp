// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "dllmain.h"
#include "WrappedD3D12Device.h"

#define _CRT_SECURE_NO_DEPRECATE
#pragma warning (disable : 4996)

PFN_D3D12_CREATE_DEVICE createDevice;
PFN_D3D12_GET_DEBUG_INTERFACE getDebugInterface;
PFN_D3D12_ENABLE_EXPERIMENTAL_FEATURES enableExperimentalFeatures;
PFN_D3D12_GET_INTERFACE getInterface;
PFN_D3D12_SERIALIZE_ROOT_SIGNATURE serializeRootSignature;
PFN_D3D12_CREATE_ROOT_SIGNATURE_DESERIALIZER createRootSignatureDeserializer;
PFN_D3D12_SERIALIZE_VERSIONED_ROOT_SIGNATURE serializeVersionedRootSignature;
PFN_D3D12_CREATE_VERSIONED_ROOT_SIGNATURE_DESERIALIZER createVersionedRootSignatureDeserializer;

FARPROC D3D12PIXEventsReplaceBlockOrg;
FARPROC D3D12PIXGetThreadInfoOrg;
FARPROC D3D12PIXNotifyWakeFromFenceSignalOrg;
FARPROC D3D12PIXReportCounterOrg;

FARPROC GetBehaviorValueOrg;
FARPROC SetAppCompatStringPointerOrg;
FARPROC D3D12CoreCreateLayeredDeviceOrg;
FARPROC D3D12CoreGetLayeredDeviceSizeOrg;
FARPROC D3D12CoreRegisterLayersOrg;
FARPROC D3D12DeviceRemovedExtendedDataOrg;

extern FARPROC D3D12PIXEventsReplaceBlockOrg;
extern FARPROC D3D12PIXGetThreadInfoOrg;
extern FARPROC D3D12PIXNotifyWakeFromFenceSignalOrg;
extern FARPROC D3D12PIXReportCounterOrg;

extern FARPROC GetBehaviorValueOrg;
extern FARPROC SetAppCompatStringPointerOrg;
extern FARPROC D3D12CoreCreateLayeredDeviceOrg;
extern FARPROC D3D12CoreGetLayeredDeviceSizeOrg;
extern FARPROC D3D12CoreRegisterLayersOrg;
extern FARPROC D3D12DeviceRemovedExtendedDataOrg;

extern "C" void Proxy_CallOrgFcnAsm(void);
extern "C" FARPROC FcnPtrOrg = NULL;

void loadOriginalD3D12()
{
	originalD3D12 = LoadLibrary(".\\d3d12.o.dll");

	if (!originalD3D12)
	{
		char dllpath[MAX_PATH];
		GetSystemDirectory(dllpath, MAX_PATH);
		strcat(dllpath, "\\d3d12.dll");
		LOG(dllpath);
		originalD3D12 = LoadLibrary(dllpath);
	}

	if (!originalD3D12)
	{
		LOG("loadOriginalD3D12: Error loading original d3d12 dll!");
		return;
	}

	LOG("loadOriginalD3D12: d3d12 dll loaded!");

	createDevice = (PFN_D3D12_CREATE_DEVICE)GetProcAddress(originalD3D12, "D3D12CreateDevice");
	createRootSignatureDeserializer = (PFN_D3D12_CREATE_ROOT_SIGNATURE_DESERIALIZER)GetProcAddress(originalD3D12, "D3D12CreateRootSignatureDeserializer");
	createVersionedRootSignatureDeserializer = (PFN_D3D12_CREATE_VERSIONED_ROOT_SIGNATURE_DESERIALIZER)GetProcAddress(originalD3D12, "D3D12CreateVersionedRootSignatureDeserializer");
	enableExperimentalFeatures = (PFN_D3D12_ENABLE_EXPERIMENTAL_FEATURES)GetProcAddress(originalD3D12, "D3D12EnableExperimentalFeatures");
	getDebugInterface = (PFN_D3D12_GET_DEBUG_INTERFACE)GetProcAddress(originalD3D12, "D3D12GetDebugInterface");
	getInterface = (PFN_D3D12_GET_INTERFACE)GetProcAddress(originalD3D12, "D3D12GetInterface");
	serializeRootSignature = (PFN_D3D12_SERIALIZE_ROOT_SIGNATURE)GetProcAddress(originalD3D12, "D3D12SerializeRootSignature");
	serializeVersionedRootSignature = (PFN_D3D12_SERIALIZE_VERSIONED_ROOT_SIGNATURE)GetProcAddress(originalD3D12, "D3D12SerializeVersionedRootSignature");

	D3D12PIXEventsReplaceBlockOrg = GetProcAddress(originalD3D12, "D3D12PIXEventsReplaceBlock");
	D3D12PIXGetThreadInfoOrg = GetProcAddress(originalD3D12, "D3D12PIXGetThreadInfo");
	D3D12PIXNotifyWakeFromFenceSignalOrg = GetProcAddress(originalD3D12, "D3D12PIXNotifyWakeFromFenceSignal");
	D3D12PIXReportCounterOrg = GetProcAddress(originalD3D12, "D3D12PIXReportCounter");

	GetBehaviorValueOrg = GetProcAddress(originalD3D12, "GetBehaviorValue");
	SetAppCompatStringPointerOrg = GetProcAddress(originalD3D12, "SetAppCompatStringPointer");
	D3D12CoreCreateLayeredDeviceOrg = GetProcAddress(originalD3D12, "D3D12CoreCreateLayeredDevice");
	D3D12CoreGetLayeredDeviceSizeOrg = GetProcAddress(originalD3D12, "D3D12CoreGetLayeredDeviceSize");
	D3D12CoreRegisterLayersOrg = GetProcAddress(originalD3D12, "D3D12CoreRegisterLayers");
	D3D12DeviceRemovedExtendedDataOrg = GetProcAddress(originalD3D12, "D3D12DeviceRemovedExtendedData");
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
		prepareOfs(".\\d3d12-proxy-" + datetime + ".log");
		LOG("d3d12-proxy DLL_PROCESS_ATTACH");
#endif

		loadOriginalD3D12();
		break;

	case DLL_PROCESS_DETACH:
		LOG("d3d12-proxy DLL_PROCESS_DETACH");

		if (originalD3D12)
			FreeLibrary(originalD3D12);

#ifdef LOGGING_ACTIVE
		closeOfs();
#endif

		break;
	}

	return TRUE;
}

HRESULT WINAPI D3D12CreateDevice(_In_opt_ IUnknown* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, _In_ REFIID riid, _COM_Outptr_opt_ void** ppDevice)
{
	LOG("D3D12CreateDevice: MinimumFeatureLevel: " + int_to_hex(MinimumFeatureLevel));

	HRESULT result;

	if (pAdapter != nullptr)
	{
		LOG("D3D12CreateDevice querying proxy adapter interface...");

		result = createDevice(pAdapter, MinimumFeatureLevel, riid, ppDevice);
		LOG("D3D12CreateDevice no proxy adapter create result: " + int_to_hex(result));

		if (result == S_OK)
		{
			LOG("D3D12CreateDevice wrapping device...");
			ID3D12Device* real = (ID3D12Device*)(*ppDevice);
			real->AddRef();
			*ppDevice = new WrappedD3D12Device(real);
		}

		LOG("D3D12CreateDevice result: " + int_to_hex(result));
		return result;
	}

	LOG("D3D12CreateDevice: pAdapter is null!");
	result = createDevice(pAdapter, MinimumFeatureLevel, riid, ppDevice);

	if (result == S_OK)
	{
		LOG("D3D12CreateDevice: OK!");
		ID3D12Device* real = (ID3D12Device*)(*ppDevice);
		real->AddRef();
		*ppDevice = new WrappedD3D12Device(real);
	}
	else
		LOG("D3D12CreateDevice: " + int_to_hex(result));

	return result;
}


HRESULT WINAPI D3D12GetDebugInterface(_In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug)
{
	LOG("D3D12GetDebugInterface");

	if (!originalD3D12)
	{
		LOG("D3D12GetDebugInterface: no originalD3D12");
		return E_POINTER;
	}

	return getDebugInterface(riid, ppvDebug);
}

HRESULT WINAPI D3D12EnableExperimentalFeatures(UINT NumFeatures, _In_count_(NumFeatures) const IID* pIIDs, _In_opt_count_(NumFeatures) void* pConfigurationStructs, _In_opt_count_(NumFeatures) UINT* pConfigurationStructSizes)
{
	LOG("D3D12EnableExperimentalFeatures");

	if (!originalD3D12)
	{
		LOG("D3D12EnableExperimentalFeatures: no originalD3D12");
		return E_POINTER;
	}

	return enableExperimentalFeatures(NumFeatures, pIIDs, pConfigurationStructs, pConfigurationStructSizes);
}

HRESULT WINAPI D3D12GetInterface(_In_ REFCLSID rclsid, _In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug)
{
	LOG("D3D12GetInterface");

	if (!originalD3D12)
	{
		LOG("D3D12GetInterface: no originalD3D12");
		return E_POINTER;
	}

	return getInterface(rclsid, riid, ppvDebug);
}

HRESULT WINAPI D3D12SerializeRootSignature(_In_ const D3D12_ROOT_SIGNATURE_DESC* pRootSignature, _In_ D3D_ROOT_SIGNATURE_VERSION Version, _Out_ ID3DBlob** ppBlob, _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob)
{
	LOG("D3D12SerializeRootSignature");

	if (!originalD3D12)
	{
		LOG("D3D12SerializeRootSignature: no originalD3D12");
		return E_POINTER;
	}

	return serializeRootSignature(pRootSignature, Version, ppBlob, ppErrorBlob);
}

HRESULT WINAPI D3D12CreateRootSignatureDeserializer(_In_reads_bytes_(SrcDataSizeInBytes) LPCVOID pSrcData, _In_ SIZE_T SrcDataSizeInBytes, _In_ REFIID pRootSignatureDeserializerInterface, _Out_ void** ppRootSignatureDeserializer)
{
	LOG("D3D12CreateRootSignatureDeserializer");

	if (!originalD3D12)
	{
		LOG("D3D12CreateRootSignatureDeserializer: no originalD3D12");
		return E_POINTER;
	}

	return createRootSignatureDeserializer(pSrcData, SrcDataSizeInBytes, pRootSignatureDeserializerInterface, ppRootSignatureDeserializer);
}

HRESULT WINAPI D3D12SerializeVersionedRootSignature(_In_ const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignature, _Out_ ID3DBlob** ppBlob, _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob)
{
	LOG("D3D12SerializeVersionedRootSignature");

	if (!originalD3D12)
	{
		LOG("D3D12SerializeVersionedRootSignature: no originalD3D12");
		return E_POINTER;
	}


	auto result = serializeVersionedRootSignature(pRootSignature, ppBlob, ppErrorBlob);
	LOG("D3D12SerializeVersionedRootSignature result: " + int_to_hex(result) + ", ppErrorBlob: " + std::to_string(ppErrorBlob == nullptr));
	return result;
}

HRESULT WINAPI D3D12CreateVersionedRootSignatureDeserializer(_In_reads_bytes_(SrcDataSizeInBytes) LPCVOID pSrcData, _In_ SIZE_T SrcDataSizeInBytes, _In_ REFIID pRootSignatureDeserializerInterface, _Out_ void** ppRootSignatureDeserializer)
{
	LOG("D3D12CreateVersionedRootSignatureDeserializer");

	if (!originalD3D12)
	{
		LOG("D3D12CreateVersionedRootSignatureDeserializer: no originalD3D12");
		return E_POINTER;
	}

	return createVersionedRootSignatureDeserializer(pSrcData, SrcDataSizeInBytes, pRootSignatureDeserializerInterface, ppRootSignatureDeserializer);
}

extern "C" {
	void D3D12PIXEventsReplaceBlock() { FcnPtrOrg = D3D12PIXEventsReplaceBlockOrg; Proxy_CallOrgFcnAsm(); }
	void D3D12PIXGetThreadInfo() { FcnPtrOrg = D3D12PIXGetThreadInfoOrg; Proxy_CallOrgFcnAsm(); }
	void D3D12PIXNotifyWakeFromFenceSignal() { FcnPtrOrg = D3D12PIXNotifyWakeFromFenceSignalOrg; Proxy_CallOrgFcnAsm(); }
	void D3D12PIXReportCounter() { FcnPtrOrg = D3D12PIXReportCounterOrg; Proxy_CallOrgFcnAsm(); }
	void GetBehaviorValue() { FcnPtrOrg = GetBehaviorValueOrg; Proxy_CallOrgFcnAsm(); }
	void SetAppCompatStringPointer() { FcnPtrOrg = SetAppCompatStringPointerOrg; Proxy_CallOrgFcnAsm(); }
	void D3D12CoreCreateLayeredDevice() { FcnPtrOrg = D3D12CoreCreateLayeredDeviceOrg; Proxy_CallOrgFcnAsm(); }
	void D3D12CoreGetLayeredDeviceSize() { FcnPtrOrg = D3D12CoreGetLayeredDeviceSizeOrg; Proxy_CallOrgFcnAsm(); }
	void D3D12CoreRegisterLayers() { FcnPtrOrg = D3D12CoreRegisterLayersOrg; Proxy_CallOrgFcnAsm(); }
	void D3D12DeviceRemovedExtendedData() { FcnPtrOrg = D3D12DeviceRemovedExtendedDataOrg; Proxy_CallOrgFcnAsm(); }
}

