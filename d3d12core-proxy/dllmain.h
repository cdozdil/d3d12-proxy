#include <d3d12.h>

typedef HRESULT(WINAPI* PFN_GET_INTERFACE)(REFCLSID rcslid, REFIID iid, void** debug);

UINT sdkVersion = 2;

MIDL_INTERFACE("DFAFDD2C-355F-4CB3-A8B2-EA7F9260148B")
ID3D12CoreModule : public IUnknown
{
public:
  virtual DWORD STDMETHODCALLTYPE LOEnter(void) = 0;
  virtual DWORD STDMETHODCALLTYPE LOLeave(void) = 0;
  virtual DWORD STDMETHODCALLTYPE LOTryEnter(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Initialize(DWORD, LPCSTR) = 0;
  virtual DWORD STDMETHODCALLTYPE GetSDKVersion(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDllExports(void*) = 0;
};

struct WrappedCoreModule : public ID3D12CoreModule
{
    unsigned int m_iRefcount = 0;
    ID3D12CoreModule* m_pReal = NULL;

    WrappedCoreModule(ID3D12CoreModule* real) : m_iRefcount(1), m_pReal(real) {}
    ~WrappedCoreModule() { SAFE_RELEASE(m_pReal); }

    //////////////////////////////
    // implement IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) { return E_NOINTERFACE; }
    ULONG STDMETHODCALLTYPE AddRef()
    {
        InterlockedIncrement(&m_iRefcount);
        return m_iRefcount;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
        unsigned int ret = InterlockedDecrement(&m_iRefcount);
        if (ret == 0)
            delete this;
        return ret;
    }
    //////////////////////////////
    // implement ID3D12CoreModule
    virtual DWORD STDMETHODCALLTYPE LOEnter(void) 
    { 
        LOG("WrappedCoreModule LOEnter");
        return m_pReal->LOEnter(); 
    }
    
    virtual DWORD STDMETHODCALLTYPE LOLeave(void) 
    { 
        LOG("WrappedCoreModule LOLeave");
        return m_pReal->LOLeave(); 
    }

    virtual DWORD STDMETHODCALLTYPE LOTryEnter(void) 
    { 
        LOG("WrappedCoreModule LOTryEnter");
        return m_pReal->LOTryEnter();
    }

    virtual HRESULT STDMETHODCALLTYPE Initialize(DWORD version, LPCSTR unknown)
    {
        LOG("WrappedCoreModule Initialize");
        return m_pReal->Initialize(sdkVersion, unknown);
    }

    virtual DWORD STDMETHODCALLTYPE GetSDKVersion(void) 
    { 
        LOG("WrappedCoreModule GetSDKVersion");
        return sdkVersion; 
    }

    virtual HRESULT STDMETHODCALLTYPE GetDllExports(void* a) 
    { 
        LOG("WrappedCoreModule GetDllExports");
        return m_pReal->GetDllExports(a); 
    }
};