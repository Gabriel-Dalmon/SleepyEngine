// SleepyEngine.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SleepyEngine.h"


#define SWAP_CHAIN_BUFFER_COUNT 2

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

// Global Variables:

// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



SleepyEngine::SleepyEngine(HINSTANCE hInstance)
{
    m_hAppInstance = hInstance;
}

void SleepyEngine::InitD3D()
{
#if defined(DEBUG) || defined(_DEBUG)
    EnableAdditionalD3D12Debug();
#endif
    CreateDevice();
    CreateFence();
    RecoverDescriptorsSize();
    Check4xMSAAQualitySupport();
    CreateCommandObjects();
    CreateSwapChain();
    CreateDescriptorHeaps();
    CreateRenderTargetView();
    CreateDepthStencilView();
    SetViewport();
    SetScissorRect();
}
#include <iostream>
void SleepyEngine::EnableAdditionalD3D12Debug()
{
    ID3D12Debug* pDebugController;
    int hr = D3D12GetDebugInterface(__uuidof(ID3D12Device), (void**) & pDebugController); //should throw error
    pDebugController->EnableDebugLayer();
}

void SleepyEngine::CreateDevice()
{
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void**)&m_pDevice);
    //if failed, check book for "WARP_Adapters".

}

void SleepyEngine::CreateFence()
{
    int iResult;
    iResult = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Device), (void**)&m_pFence);
}

void SleepyEngine::RecoverDescriptorsSize()
{
    m_rtvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_dsvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    m_cbvSrvUavDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void SleepyEngine::Check4xMSAAQualitySupport()
{
    int iResult;
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels; //will likely be moved to private member attribute
    msQualityLevels.Format = m_backBufferFormat;
    msQualityLevels.SampleCount = 4;
    msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    msQualityLevels.NumQualityLevels = 0;
    iResult = m_pDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels));
    assert(m_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
}

void SleepyEngine::CreateCommandObjects()
{
    int iResult;
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    iResult = m_pDevice->CreateCommandQueue(&queueDesc, __uuidof(ID3D12Device), (void**) &m_pCommandQueue);
    iResult = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12Device), (void**)&m_pDirectCmdListAlloc);
    iResult = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pDirectCmdListAlloc, nullptr, __uuidof(ID3D12Device), (void**)&m_pCommandList);
    m_pCommandList->Close();
}

void SleepyEngine::CreateSwapChain()
{
    delete m_pSwapChain;
    DXGI_SWAP_CHAIN_DESC swapChainDescriptor;
    swapChainDescriptor.BufferDesc.Width = m_clientWidth;
    swapChainDescriptor.BufferDesc.Height = m_clientHeight;
    swapChainDescriptor.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDescriptor.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDescriptor.BufferDesc.Format = m_backBufferFormat;
    swapChainDescriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDescriptor.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
    swapChainDescriptor.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
    swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
    swapChainDescriptor.OutputWindow = mhMainWnd;
    swapChainDescriptor.Windowed = true;
    swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDescriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    m_pDxgiFactory->CreateSwapChain(m_pCommandList, &swapChainDescriptor, &m_pSwapChain);
}

void SleepyEngine::CreateDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;
    m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pRtvHeap);

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    m_pDevice->CreateDescriptorHeap(&dsvHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pDsvHeap);
}

void SleepyEngine::CreateRenderTargetView()
{
    ID3D12Resource* m_pSwapChainBuffer[SWAP_CHAIN_BUFFER_COUNT]; // must be freed when finished
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
    {
		m_pSwapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&m_pSwapChainBuffer);
		m_pDevice->CreateRenderTargetView(m_pSwapChainBuffer[i], nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
	}
}

void SleepyEngine::CreateDepthStencilView()
{
    D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = m_clientWidth;
	depthStencilDesc.Height = m_clientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    // Describes a value used to optimize clear operations for a particular resource.
    // https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_clear_value
	D3D12_CLEAR_VALUE optClear;
	optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

    CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
    m_pDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		__uuidof(ID3D12Resource),
		(void**)&m_pDepthStencilBuffer
    );

	/*D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.Texture2D.MipSlice = 0;*/
	m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, GetDepthStencilView()); //nullptr for default mip level 0

    CD3DX12_RESOURCE_BARRIER ressourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_pDepthStencilBuffer,
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_DEPTH_WRITE);
    m_pCommandList->ResourceBarrier(
        1,
        &ressourceBarrier
    );
}

void SleepyEngine::SetViewport()
{
    D3D12_VIEWPORT viewPort;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = static_cast<float>(m_clientWidth);
	viewPort.Height = static_cast<float>(m_clientHeight);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	D3D12_RECT scissorRect;
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = m_clientWidth;
	scissorRect.bottom = m_clientHeight;

	m_pCommandList->RSSetViewports(1, &viewPort);
	m_pCommandList->RSSetScissorRects(1, &scissorRect);
}

// Scissor rect is used to clip pixels to a certain area of the render target and avoid rendering them (ex: pixels below UI).
void SleepyEngine::SetScissorRect()
{
    m_scissorRect = { 0, 0, m_clientWidth / 2, m_clientHeight / 2 };
    m_pCommandList->RSSetScissorRects(1, &m_scissorRect);
}

int SleepyEngine::Initialize()
{
    LoadStringW(m_hAppInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
    LoadStringW(m_hAppInstance, IDC_SLEEPYENGINE, m_szWindowClass, MAX_LOADSTRING);
    RegisterWindowClass();
    InitWindow(SW_SHOW);
    InitD3D();
    return 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE SleepyEngine::GetCurrentBackBufferView()const
{
    // CD3DX12 constructor to offset to the RTV of the current back buffer.
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),// handle start
        m_currentBackBuffer, // index to offset
        m_rtvDescriptorSize); // byte size of descriptor
}

D3D12_CPU_DESCRIPTOR_HANDLE SleepyEngine::GetDepthStencilView()const
{
    return m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

/*int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SLEEPYENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SLEEPYENGINE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
*/

ATOM SleepyEngine::RegisterWindowClass()
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = m_hAppInstance;
    wcex.hIcon          = LoadIcon(m_hAppInstance, MAKEINTRESOURCE(IDI_SLEEPYENGINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SLEEPYENGINE);
    wcex.lpszClassName  = m_szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void SleepyEngine::InitWindow(int nCmdShow)
{
   mhMainWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hAppInstance, nullptr);

   ShowWindow(mhMainWnd, nCmdShow);
   UpdateWindow(mhMainWnd);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            /*case IDM_ABOUT:
                DialogBox((HINSTANCE)wParam, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;*/
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/*
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}*/
