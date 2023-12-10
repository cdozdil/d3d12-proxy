
typedef HRESULT(WINAPI* PFN_D3D12_ENABLE_EXPERIMENTAL_FEATURES)(UINT NumFeatures,
	_In_count_(NumFeatures) const IID* pIIDs,
	_In_opt_count_(NumFeatures) void* pConfigurationStructs,
	_In_opt_count_(NumFeatures) UINT* pConfigurationStructSizes);

BOOL DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);

HMODULE originalD3D12 = NULL;
