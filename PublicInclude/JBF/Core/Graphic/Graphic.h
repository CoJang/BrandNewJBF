#pragma once

#include"JBF/DXHeaders.h"
#include"JBF/Global/Global.h"

namespace JBF{
    namespace Core{
        namespace Graphic{
            extern void Init(const Global::Math::Point<USHORT>* size);
            extern void Release();

            extern HRESULT CheckState();
            extern void Present();

            extern INLINE IDirect3DDevice9* GetDevice();
            extern INLINE D3DDISPLAYMODE* GetDisplayInfo();

            extern INLINE HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
            extern INLINE HRESULT CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle);

            extern INLINE HRESULT GetViewport(D3DVIEWPORT9* pViewport);
            extern INLINE HRESULT SetViewport(const D3DVIEWPORT9* pViewport);

            extern INLINE HRESULT GetTransform(D3DTRANSFORMSTATETYPE State, Global::Math::Matrix* pMatrix);
            extern INLINE HRESULT SetTransform(D3DTRANSFORMSTATETYPE State, const Global::Math::Matrix* pMatrix);

            extern INLINE HRESULT GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue);
            extern INLINE HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);

            extern INLINE HRESULT GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride);
            extern INLINE HRESULT SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride);

            extern INLINE HRESULT GetFVF(DWORD* pFVF);
            extern INLINE HRESULT SetFVF(DWORD FVF);

            extern INLINE HRESULT GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture);
            extern INLINE HRESULT SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture);

            extern INLINE HRESULT GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
            extern INLINE HRESULT SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);

            extern INLINE HRESULT DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
            extern INLINE HRESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
            extern INLINE HRESULT DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
            extern INLINE HRESULT DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
        };
    };
};