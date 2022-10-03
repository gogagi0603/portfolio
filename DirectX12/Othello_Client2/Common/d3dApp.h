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

    D3DApp(HINSTANCE hInstance); // 뭐 그냥 익히 알고 있는 초기화
    D3DApp(const D3DApp& rhs) = delete;
    D3DApp& operator=(const D3DApp& rhs) = delete;
    virtual ~D3DApp(); // COM 인터페이스 해제, 명령대기열 비움

public:

    static D3DApp* GetApp();
    
	HINSTANCE AppInst()const; // 응용프로그램 인스턴스 핸들의 복사본을 돌려주는 접근함수
	HWND      MainWnd()const; // 매인 윈도우의 핸들 복사본을 돌려주는 접근함수
	float     AspectRatio()const; // 후면 버퍼의 종횡비 (aspect ratio) 즉 높이에 대한 너비의 비율을 돌려줌

    bool Get4xMsaaState()const; // 활성화 되어있으면 true, then false
    void Set4xMsaaState(bool value); // 4X MSAA를 활성화 혹은 비활성화

	int Run(); // 응용프로그램 메시지 루프를 감싼 매서드. windows 메시지가 전혀 없을때에도 게임의 logic 을 처리 할수 있도록 이 메서드는 win32 PeekMessage 함수를 사용한다

    virtual bool Initialize(); // 자원 할당, 장면 물체 초기화, 광원 설정 등 응용프로그램 고유의 초기화 코드를 넣음. 이 메서드의 기본구현 (D3DApp의 구현)은 InitMainWindow와 InitDirect3D를 호출하는 것.
                               // 따라서 파생 클래스의 구현에서 D3DApp의 초기화된 멤버들에 접근해야한다면 D3DApp의 기본 구현을 먼저 호출해주어야함
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // 메인 윈도우 메시지 처리부 (윈도우 프로시저 함수)를 넣음. 일반적으로 D3DApp::MsgProc이 처리하지 않거나 적합하지 않을때 이걸 재정의 하면됨

protected:
    virtual void CreateRtvAndDsvDescriptorHeaps(); // RTV 서술자와 DSV 서술자를 응용프로그램에 맞게 생성하는데 쓰임
	virtual void OnResize(); // 응용 프로그램 창이 WM_SIZE 메시지를 받으면 이걸 실행. resize를 하긴 하는데 후면버퍼, DSV를 새 클라이언트 영역에 맞게 조정해야함. RTV도 마찬가지
	virtual void Update(const GameTimer& gt)=0; // 순수가상함수. 매 프레임 호출됨. 시간의 흐름에 따른 응용프로그램 갱신
    virtual void Draw(const GameTimer& gt)=0; // 순수가상 함수. 매프레임 호출. 현재 ㅍ르ㅔ임을 후면 버퍼에 실제로 그리기 위한 렌더링 명령들을 제출. 다르기면 후면 버퍼를 화면에 제시 (IDXGISwapChain::Presend)

	// 간편한 마우스 입력 처리를 위한 가상 함수들
    virtual void OnKeyboardDown(WPARAM btnState) {};
	virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }

protected:

	bool InitMainWindow(); // 응용프로그램의 메인 윈도우를 초기화.
	bool InitDirect3D(); // Directx를 초기화.
	void CreateCommandObjects(); // 명령 대기열 하나와 명령 목록 할당자 하나, 명령 목록 하나 생성
    void CreateSwapChain(); // 스왑체인 생성

	void FlushCommandQueue(); // GPU가 명령 대기열에 있는 모든 명령의 처리를 마칠 때까지 CPU가 기다리게 만듬

	ID3D12Resource* CurrentBackBuffer()const; // 스왑체인의 현재 후면 버퍼에 대한 ID3D12Resource를 돌려줌
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const; // 현재 후면 버퍼에 대한 RTV (렌더 대상 뷰)를 돌려줌
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const; // 현재 후면 버퍼에 대한 DSV (깊이 스텐실 뷰)를 돌려줌

	void CalculateFrameStats(); // 평균초당 프레임수 (FPS)와 평균 프레임당 밀리초를 계산.

    void LogAdapters(); // 시스템의 모든 디스플레이 어댑터를 열거
    void LogAdapterOutputs(IDXGIAdapter* adapter); // 주어진 어댑터와 연관된 모든 출력 열거
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format); // 주어진 출력과 픽셀 형식의 조합이 지원하는 모든 디스플레이 모드 나열

protected:

    static D3DApp* mApp;

    HINSTANCE mhAppInst = nullptr; // 응용 프로그램 인스턴스 핸들
    HWND      mhMainWnd = nullptr; // 메인 윈도우 핸들
	bool      mAppPaused = false;  // is the application paused?
	bool      mMinimized = false;  // is the application minimized?
	bool      mMaximized = false;  // is the application maximized?
	bool      mResizing = false;   // are the resize bars being dragged?
    bool      mFullscreenState = false;// fullscreen enabled

	// Set true to use 4X MSAA (?.1.8).  The default is false.
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	// Used to keep track of the 밺elta-time?and game time (?.4).
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

	// 파생클래스들은 자신의 생성자에서 이 멤버 변수들을 자신의 목적에 맞는 초기 값들로 설정해야한다
	std::wstring mMainWndCaption = L"d3d App";
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = 800;
	int mClientHeight = 600;
};

