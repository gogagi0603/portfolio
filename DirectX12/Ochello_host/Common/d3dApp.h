//***************************************************************************************
// d3dApp.h by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3dUtil.h"
#include "GameTimer.h"
#include <winsock.h>
#include <thread>
// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "ws2_32")

void err_quit(char* msg);
void err_display(char* msg);

class D3DApp
{
protected:

    D3DApp(HINSTANCE hInstance); // �� �׳� ���� �˰� �ִ� �ʱ�ȭ
    D3DApp(const D3DApp& rhs) = delete;
    D3DApp& operator=(const D3DApp& rhs) = delete;
    virtual ~D3DApp(); // COM �������̽� ����, ��ɴ�⿭ ���

public:

    static D3DApp* GetApp();
    
	HINSTANCE AppInst()const; // �������α׷� �ν��Ͻ� �ڵ��� ���纻�� �����ִ� �����Լ�
	HWND      MainWnd()const; // ���� �������� �ڵ� ���纻�� �����ִ� �����Լ�
	float     AspectRatio()const; // �ĸ� ������ ��Ⱦ�� (aspect ratio) �� ���̿� ���� �ʺ��� ������ ������

    bool Get4xMsaaState()const; // Ȱ��ȭ �Ǿ������� true, then false
    void Set4xMsaaState(bool value); // 4X MSAA�� Ȱ��ȭ Ȥ�� ��Ȱ��ȭ

	int Run(); // �������α׷� �޽��� ������ ���� �ż���. windows �޽����� ���� ���������� ������ logic �� ó�� �Ҽ� �ֵ��� �� �޼���� win32 PeekMessage �Լ��� ����Ѵ�

    virtual bool Initialize(); // �ڿ� �Ҵ�, ��� ��ü �ʱ�ȭ, ���� ���� �� �������α׷� ������ �ʱ�ȭ �ڵ带 ����. �� �޼����� �⺻���� (D3DApp�� ����)�� InitMainWindow�� InitDirect3D�� ȣ���ϴ� ��.
                               // ���� �Ļ� Ŭ������ �������� D3DApp�� �ʱ�ȭ�� ����鿡 �����ؾ��Ѵٸ� D3DApp�� �⺻ ������ ���� ȣ�����־����
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // ���� ������ �޽��� ó���� (������ ���ν��� �Լ�)�� ����. �Ϲ������� D3DApp::MsgProc�� ó������ �ʰų� �������� ������ �̰� ������ �ϸ��

protected:
    virtual void CreateRtvAndDsvDescriptorHeaps(); // RTV �����ڿ� DSV �����ڸ� �������α׷��� �°� �����ϴµ� ����
	virtual void OnResize(); // ���� ���α׷� â�� WM_SIZE �޽����� ������ �̰� ����. resize�� �ϱ� �ϴµ� �ĸ����, DSV�� �� Ŭ���̾�Ʈ ������ �°� �����ؾ���. RTV�� ��������
	virtual void Update(const GameTimer& gt)=0; // ���������Լ�. �� ������ ȣ���. �ð��� �帧�� ���� �������α׷� ����
    virtual void Draw(const GameTimer& gt)=0; // �������� �Լ�. �������� ȣ��. ���� ���������� �ĸ� ���ۿ� ������ �׸��� ���� ������ ��ɵ��� ����. �ٸ���� �ĸ� ���۸� ȭ�鿡 ���� (IDXGISwapChain::Presend)

	// ������ ���콺 �Է� ó���� ���� ���� �Լ���
    virtual void OnKeyboardDown(WPARAM btnState) {};
	virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }

protected:

	bool InitMainWindow(); // �������α׷��� ���� �����츦 �ʱ�ȭ.
	bool InitDirect3D(); // Directx�� �ʱ�ȭ.
	void CreateCommandObjects(); // ��� ��⿭ �ϳ��� ��� ��� �Ҵ��� �ϳ�, ��� ��� �ϳ� ����
    void CreateSwapChain(); // ����ü�� ����

	void FlushCommandQueue(); // GPU�� ��� ��⿭�� �ִ� ��� ����� ó���� ��ĥ ������ CPU�� ��ٸ��� ����

	ID3D12Resource* CurrentBackBuffer()const; // ����ü���� ���� �ĸ� ���ۿ� ���� ID3D12Resource�� ������
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const; // ���� �ĸ� ���ۿ� ���� RTV (���� ��� ��)�� ������
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const; // ���� �ĸ� ���ۿ� ���� DSV (���� ���ٽ� ��)�� ������

	void CalculateFrameStats(); // ����ʴ� �����Ӽ� (FPS)�� ��� �����Ӵ� �и��ʸ� ���.

    void LogAdapters(); // �ý����� ��� ���÷��� ����͸� ����
    void LogAdapterOutputs(IDXGIAdapter* adapter); // �־��� ����Ϳ� ������ ��� ��� ����
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format); // �־��� ��°� �ȼ� ������ ������ �����ϴ� ��� ���÷��� ��� ����

protected:

    static D3DApp* mApp;

    HINSTANCE mhAppInst = nullptr; // ���� ���α׷� �ν��Ͻ� �ڵ�
    HWND      mhMainWnd = nullptr; // ���� ������ �ڵ�
	bool      mAppPaused = false;  // is the application paused?
	bool      mMinimized = false;  // is the application minimized?
	bool      mMaximized = false;  // is the application maximized?
	bool      mResizing = false;   // are the resize bars being dragged?
    bool      mFullscreenState = false;// fullscreen enabled

	// Set true to use 4X MSAA (?.1.8).  The default is false.
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	// Used to keep track of the �delta-time?and game time (?.4).
	GameTimer mTimer;
	
    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
    UINT64 mCurrentFence = 0;
	
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

    D3D12_VIEWPORT mScreenViewport; 
    D3D12_RECT mScissorRect;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	// �Ļ�Ŭ�������� �ڽ��� �����ڿ��� �� ��� �������� �ڽ��� ������ �´� �ʱ� ����� �����ؾ��Ѵ�
	std::wstring mMainWndCaption = L"d3d App";
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = 800;
	int mClientHeight = 600;
};

