#include"pch.h"

#include<unordered_set>

#include"JBF/JBFramework.h"

#define SIZE_RESTABLE_BUCKET 1024

namespace JBF{
    namespace Core{
        namespace Graphic{
            // Inner variable(s) definition
            ///////////////////////////////////////////
            static D3DDISPLAYMODE ins_displayInfo;
            static D3DPRESENT_PARAMETERS ins_d3dpp;

            static IDirect3D9* ins_d3d = nullptr;
            static IDirect3DDevice9* ins_device = nullptr;

            static D3DCAPS9 ins_cap;

            std::unordered_set<Base::DXResource*, Global::Hash::PointerKeyHasher<Base::DXResource*>> ins_resTableManaged, ins_resTableVRAM;
            ///////////////////////////////////////////

            // Inner function(s) definition
            ///////////////////////////////////////////
            static HRESULT ins_createDevice(){
                auto hr = ins_d3d->CreateDevice(
                    D3DADAPTER_DEFAULT,
                    D3DDEVTYPE_HAL,
                    ins_d3dpp.hDeviceWindow,
                    D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                    &ins_d3dpp,
                    &ins_device
                );
                ASSERT_HRESULT(hr, DXGetErrorString(hr));
                return hr;
            }
            static INLINE HRESULT ins_resetDevice(){
                auto hr = ins_device->Reset(&ins_d3dpp);
                ASSERT_HRESULT(hr, _T("Failed to reset device."));
                return hr;
            }
            static void ins_releaseDevice(){
                if(ins_d3d)RELEASE(ins_d3d);
            }

            static INLINE HRESULT ins_validate(HRESULT res){
                HRESULT hr;

                if (res == D3DERR_DEVICENOTRESET){ // need reset
                    hr = ins_createDevice();
                    if (FAILED(hr)){
                        ASSERT_HRESULT(E_FAIL, _T("Failed to creaet device."));
                        return E_FAIL;
                    }

                    for (auto i : ins_resTableVRAM)i->Validate();
                }
                else{ // need recreate
                    if (FAILED(ins_resetDevice()))return ins_validate(D3DERR_DEVICENOTRESET);

                    for (auto i : ins_resTableVRAM)i->Validate();
                    for (auto i : ins_resTableManaged)i->Validate();
                }

                return CheckState();
            }
            static INLINE void ins_invalidate(HRESULT res){
                if (res == D3DERR_DEVICENOTRESET){ // need reset
                    for (auto i : ins_resTableVRAM)i->Invalidate();
                }
                else{ // need recreate
                    for (auto i : ins_resTableVRAM)i->Invalidate();
                    for (auto i : ins_resTableManaged)i->Invalidate();

                    ins_releaseDevice();
                }
            }
            ///////////////////////////////////////////

            void Init(const Global::Math::Point<USHORT>* size){
                {
                    ins_resTableManaged.rehash(SIZE_RESTABLE_BUCKET);
                    ins_resTableVRAM.rehash(SIZE_RESTABLE_BUCKET);
                }

                {
                    ins_displayInfo.Width = size->x;
                    ins_displayInfo.Height = size->y;
                    ins_displayInfo.Format = D3DFMT_A8R8G8B8;
                    ins_displayInfo.RefreshRate = 0;
                }

                {
                    ins_d3d = Direct3DCreate9(D3D_SDK_VERSION);
                    ASSERT(ins_d3d != nullptr, _T("Failed to create D3D9."));
                }

                {
                    ASSERT_HRESULT(
                        ins_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &ins_cap),
                        _T("Failed to get device caps.")
                    );
                }

                {
                    ASSERT(
                        ins_cap.VertexShaderVersion >= D3DVS_VERSION(2, 0),
                        _T("System does not support vertex shader version 2.0.")
                    );

                    ASSERT(
                        ins_cap.PixelShaderVersion >= D3DPS_VERSION(2, 0),
                        _T("System does not support pixel shader version 2.0.")
                    );
                }

                {
                    ZeroMemory(&ins_d3dpp, sizeof ins_d3dpp);

                    ins_d3dpp.hDeviceWindow = JBF::GetHandle();

                    ins_d3dpp.BackBufferWidth = ins_displayInfo.Width;
                    ins_d3dpp.BackBufferHeight = ins_displayInfo.Height;
                    ins_d3dpp.BackBufferFormat = ins_displayInfo.Format;
                    ins_d3dpp.BackBufferCount = 1;

                    ins_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
                    ins_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
                    ins_d3dpp.Windowed = TRUE;

                    ins_d3dpp.EnableAutoDepthStencil = TRUE;
                    ins_d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
                }

                ASSERT_HRESULT(ins_createDevice(), _T("Failed to initialize device."));
            }
            void Release(){
                ins_releaseDevice();
            }

            HRESULT CheckState(){
                if (ins_device){
                    auto hr = ins_device->TestCooperativeLevel();
                    if (FAILED(hr)){
                        switch (hr){
                        case D3DERR_DEVICENOTRESET:
                        case D3DERR_DEVICELOST:
                            return ins_validate(hr);
                        }
                    }
                }
                else return ins_validate(D3DERR_DEVICELOST);

                return S_OK;
            }
            void Present(){
                auto hr = ins_device->Present(nullptr, nullptr, nullptr, nullptr);

                if (FAILED(hr)){
                    switch (hr){
                    case D3DERR_DEVICENOTRESET:
                    case D3DERR_DEVICELOST:
                        ins_invalidate(hr);
                        return;
                    default:
                        ASSERT_HRESULT(hr, _T("Failed to present buffer on screen."));
                        return;
                    }
                }
            }

            INLINE IDirect3DDevice9* GetDevice(){ return ins_device; }
            INLINE D3DDISPLAYMODE* GetDisplayInfo(){ return &ins_displayInfo; }

            INLINE HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle){ return ins_device->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle); }
            INLINE HRESULT CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle){ return ins_device->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle); }

            INLINE HRESULT GetViewport(D3DVIEWPORT9* pViewport){ return ins_device->GetViewport(pViewport); }
            INLINE HRESULT SetViewport(const D3DVIEWPORT9* pViewport){ return ins_device->SetViewport(pViewport); }

            INLINE HRESULT GetTransform(D3DTRANSFORMSTATETYPE State, Global::Math::Matrix* pMatrix){ return ins_device->GetTransform(State, (D3DXMATRIXA16*)pMatrix); }
            INLINE HRESULT SetTransform(D3DTRANSFORMSTATETYPE State, const Global::Math::Matrix* pMatrix){ return ins_device->SetTransform(State, (const D3DXMATRIXA16*)pMatrix); }

            INLINE HRESULT GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue){ return ins_device->GetRenderState(State, pValue); }
            INLINE HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value){ return ins_device->SetRenderState(State, Value); }

            INLINE HRESULT GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride){ return ins_device->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride); }
            INLINE HRESULT SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride){ return ins_device->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride); }

            INLINE HRESULT GetFVF(DWORD* pFVF){ return ins_device->GetFVF(pFVF); }
            INLINE HRESULT SetFVF(DWORD FVF){ return ins_device->SetFVF(FVF); }

            INLINE HRESULT GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture){ return ins_device->GetTexture(Stage, ppTexture); }
            INLINE HRESULT SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture){ return ins_device->SetTexture(Stage, pTexture); }

            INLINE HRESULT GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue){ return ins_device->GetTextureStageState(Stage, Type, pValue); }
            INLINE HRESULT SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value){ return ins_device->SetTextureStageState(Stage, Type, Value); }

            INLINE HRESULT DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount){ return ins_device->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount); }
            INLINE HRESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount){ return ins_device->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); }
            INLINE HRESULT DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride){ return ins_device->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
            INLINE HRESULT DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride){ return ins_device->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex,NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
        };
    };
};