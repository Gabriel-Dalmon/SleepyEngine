#pragma once

#include "resource.h"

#include <windows.h>
#include <wrl.h> //ComPtr
#include <dxgi1_4.h>
#include "../../utils/d3dx12.h"

#include <d3d12.h>
#include <cassert>

#include <DXGI.h>

#define MAX_LOADSTRING 100


class SleepyEngine
{
public:
    SleepyEngine(HINSTANCE hInstance);
    int Initialize();
    int Run();
private:
    void InitWindow(int nCmdShow);
    ATOM RegisterWindowClass();

    // D3DX12 Initialization
    void InitD3D();
    void EnableAdditionalD3D12Debug();
    void CreateDevice();
    void CreateFence();
    void RecoverDescriptorsSize();
    void Check4xMSAAQualitySupport();
    void CreateCommandObjects();
    void CreateSwapChain();
    void CreateDescriptorHeaps();
    void CreateRenderTargetView();
    void CreateDepthStencilView();
    void SetViewport();
    void SetScissorRect();

    // GETTERS / SETTERS
    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView()const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView()const;
private:
    WCHAR m_szTitle[MAX_LOADSTRING];                  // The title bar text
    WCHAR m_szWindowClass[MAX_LOADSTRING];            // the main window class name

    ID3D12Device* m_pDevice = nullptr;
    ID3D12Fence* m_pFence = nullptr;

    UINT m_rtvDescriptorSize = 0;
    UINT m_dsvDescriptorSize = 0;
    UINT m_cbvSrvUavDescriptorSize = 0;

    ID3D12DescriptorHeap* m_pRtvHeap = nullptr;
    ID3D12DescriptorHeap* m_pDsvHeap = nullptr;

    DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    int m_currentBackBuffer = 0;

    UINT m_4xMsaaQuality = 0;
    bool m_4xMsaaState = false;

    ID3D12Resource* m_pDepthStencilBuffer = nullptr;

    ID3D12CommandQueue* m_pCommandQueue = nullptr;
    ID3D12CommandAllocator* m_pDirectCmdListAlloc = nullptr;
    ID3D12GraphicsCommandList* m_pCommandList = nullptr;

    IDXGIFactory4* m_pDxgiFactory = nullptr;
    IDXGISwapChain* m_pSwapChain = nullptr;

    int m_clientWidth;
    int m_clientHeight;

    tagRECT m_scissorRect;

    HWND mhMainWnd = nullptr;
    HINSTANCE m_hAppInstance = nullptr;
};