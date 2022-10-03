#include "Ochello_host.h"
bool LitColumnsApp::isRange(const int& a, const int& b)
{
	if (a >= 0 && a < 8 && b >= 0 && b < 8)
	{
		return true;
	}
	return false;
}

// 상대로부터 상대가 놓은 위치를 전달받는 함수
void LitColumnsApp::receivedPoint(char x, char y)
{
	mOpaqueRitems[x - '0' + (y - '0') * 8]->Mat = mMaterials["user2"].get();
	initColor[x - '0'][y - '0'] = -2;
	haveControl = true;
	return;
}

//스레드 함수. 나중에 class 안에 static으로 해볼것
void fn()
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1]; // 2로 줄여볼것


	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		// 클라이언트와 데이터 통신
		while (1) {
			// 데이터 받기
			retval = recvn(client_sock, buf, 2, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// 받은 데이터 출력
			theApp->receivedPoint(buf[0], buf[1]);
			theApp->checkChangeBlock(buf[0], buf[1]);
			// 대기했다가
			while (!theApp->received) // 비효율적. 다른 방법 생각해볼것
			{

			}
			theApp->received = false;
			// 데이터 보내기
			buf[0] = theApp->getX() + '0';
			buf[1] = theApp->getY() + '0';
			theApp->checkChangeBlock(buf[0], buf[1]);

			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}

		// closesocket()
		closesocket(client_sock);
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return;

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{

		theApp = std::unique_ptr<LitColumnsApp>(new LitColumnsApp(hInstance));
		if (!theApp->Initialize())
			return 0;
		std::thread t1 = std::thread(fn);

		theApp->Run();
		t1.join();
		return 0;
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}


LitColumnsApp::LitColumnsApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
	keyboard.x = 0;
	keyboard.y = 0;
}

LitColumnsApp::~LitColumnsApp()
{
	if (md3dDevice != nullptr)
		FlushCommandQueue();
}

//다이렉트 X 초기화
bool LitColumnsApp::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	// Get the increment size of a descriptor in this heap type.  This is hardware specific, 
	// so we have to query this information.
	mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildShapeGeometry();
	BuildMaterials();
	BuildRenderItems();
	BuildFrameResources();
	BuildPSOs();

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();

	return true;
}

// 놓을 수 있는 블록 위치 보여주는 기능 (추후 개발 예정)
void LitColumnsApp::checkAvailableBlock()
{

}

//시간복잡도 주의. 돌이 놓여졌을때 작동. 변경되어야할 색깔들을 변경함
void LitColumnsApp::checkChangeBlock(char x, char y)
{
	int cx = x - '0';
	int cy = y - '0';

	int xp = cx, yp = cy, xm = cx, ym = cy;

	Material* colorMat;
	int color = initColor[cx][cy];

	if (color == -1)
	{
		colorMat = mMaterials["user1"].get();
	}
	else
	{
		colorMat = mMaterials["user2"].get();
	}

	for (int i = 1; i < 8; i++)
	{

		if (++xp < 8)
		{
			// 한번이라도 검정, 흰타일 나오면 끝
			if (initColor[xp][cy] < 0)
			{

				// 검색하는 동안에 흰타일 검정타일 안나왔고, 범위 내 일인데 원래 내 색이 나왔을 때
				if (initColor[xp][cy] == color)
				{

					// cx+1부터 xp-1 까지 쭉 color의 색으로로 변경
					for (int j = cx + 1; j < xp; j++)
					{
						mOpaqueRitems[j + cy * 8]->Mat = colorMat;
						initColor[j][cy] = color;
					}
					xp += 100;
				}
			}
			//한번이라도 검정 흰타일 나오면 끝
			else
			{
				xp += 100;
			}
		}
		if (--xm >= 0)
		{
			if (initColor[xm][cy] < 0)
			{
				if (initColor[xm][cy] == color)
				{
					for (int j = cx - 1; j >= xm; j--)
					{
						mOpaqueRitems[j + cy * 8]->Mat = colorMat;
						initColor[j][cy] = color;
					}
					xm -= 100;
				}
			}
			else
			{
				xm -= 100;
			}
		}
		if (++yp < 8)
		{
			// 한번이라도 검정, 흰타일 나오면 끝
			if (initColor[cx][yp] < 0)
			{

				// 검색하는 동안에 흰타일 검정타일 안나왔고, 범위 내 일인데 원래 내 색이 나왔을 때
				if (initColor[cx][yp] == color)
				{

					// cx+1부터 xp-1 까지 쭉 color의 색으로로 변경
					for (int j = cy + 1; j < yp; j++)
					{
						mOpaqueRitems[cx + j * 8]->Mat = colorMat;
						initColor[cx][j] = color;
					}
					yp += 100;
				}
			}
			//한번이라도 검정 흰타일 나오면 끝
			else
			{
				yp += 100;
			}
		}
		if (--ym >= 0)
		{
			if (initColor[cx][ym] < 0)
			{
				if (initColor[cx][ym] == color)
				{
					for (int j = cy - 1; j >= ym; j--)
					{
						mOpaqueRitems[cx + j * 8]->Mat = colorMat;
						initColor[cx][j] = color;
					}
					ym -= 100;
				}
			}
			else
			{
				ym -= 100;
			}
		}


	}


	POINT currL;
	currL.x = cx;
	currL.y = cy;
	POINT nowL = currL;

	while (isRange(++nowL.x, ++nowL.y))
	{
		if (initColor[nowL.x][nowL.y] >= 0)
		{
			break;
		}

		if (initColor[nowL.x][nowL.y] == color)
		{
			for (int i = cx + 1, j = cy + 1; i < nowL.x && j < nowL.y; i++, j++)
			{
				mOpaqueRitems[i + j * 8]->Mat = colorMat;
				initColor[i][j] = color;
			}
			break;
		}
	}

	nowL = currL;
	while (isRange(--nowL.x, ++nowL.y))
	{
		if (initColor[nowL.x][nowL.y] >= 0)
		{
			break;
		}

		if (initColor[nowL.x][nowL.y] == color)
		{
			for (int i = cx - 1, j = cy + 1; i >= nowL.x && j < nowL.y; i--, j++)
			{
				mOpaqueRitems[i + j * 8]->Mat = colorMat;
				initColor[i][j] = color;
			}
			break;
		}
	}

	nowL = currL;
	while (isRange(++nowL.x, --nowL.y))
	{
		if (initColor[nowL.x][nowL.y] >= 0)
		{
			break;
		}

		if (initColor[nowL.x][nowL.y] == color)
		{
			for (int i = cx + 1, j = cy - 1; i < nowL.x && j >= nowL.y; i++, j--)
			{
				mOpaqueRitems[i + j * 8]->Mat = colorMat;
				initColor[i][j] = color;
			}
			break;
		}
	}

	nowL = currL;
	while (isRange(--nowL.x, --nowL.y))
	{
		if (initColor[nowL.x][nowL.y] >= 0)
		{
			break;
		}

		if (initColor[nowL.x][nowL.y] == color)
		{
			for (int i = cx - 1, j = cy - 1; i >= nowL.x && j >= nowL.y; i--, j--)
			{
				mOpaqueRitems[i + j * 8]->Mat = colorMat;
				initColor[i][j] = color;
			}
			break;
		}
	}
}

// 게임이 끝났는지? 둘다 놓을수 있는 블록이 한개도 없다면 true (추후 개발 예정)
bool LitColumnsApp::isEndGame()
{
	return true;
}

int LitColumnsApp::getX()
{
	return keyboard.x;
}

int LitColumnsApp::getY()
{
	return keyboard.y;
}

// 창 크기 바뀔때
void LitColumnsApp::OnResize()
{
	D3DApp::OnResize();
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

// 
void LitColumnsApp::Update(const GameTimer& gt)
{
	OnKeyboardInput(gt);
	UpdateCamera(gt);

	// 순환적으로 자원 프레임의 다음 원소에 접근한다
	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

	/*
	GPU가 현재 프레임 자원의 명령들을 다 처리했는지 확인.
	아직 다 처리하지 않았으면 GPU가 이 울타리 지점까지의 명령들을 처리할때까지 기다린다
	*/
	if (mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	AnimateMaterials(gt);
	UpdateObjectCBs(gt);
	UpdateMaterialCBs(gt);
	UpdateMainPassCB(gt);
}

void LitColumnsApp::Draw(const GameTimer& gt)
{
	auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;

	// 명령 기록에 관련된 메모리의 재활용을 위해 명령 할당자를 재설정.
		// 재설정은 gpu가 관련 명령 목록들을 모두 처리한 후에 일어남
	ThrowIfFailed(cmdListAlloc->Reset());

	//명령목록을 executeCommandList를 통해서 명령 대기열에 추가했다면 명령목록재설정 가능
	// 명령 목록을 재설정하면 메모리가 재활용
	ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mOpaquePSO.Get()));

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	//자원 용도에 관련된 상태 전이를 direct3d에 통지
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//후면 버퍼와 깊이 버퍼를 지운다
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// 렌더링 결과가 기록될 렌더대상 버퍼들을 지정
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	auto passCB = mCurrFrameResource->PassCB->Resource();
	mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

	DrawRenderItems(mCommandList.Get(), mOpaqueRitems);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	ThrowIfFailed(mCommandList->Close());

	// 명령큐 실행
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// 스왑체인 스왑
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// 현재 fence 지점까지의 명령들을 표시하도록 fence 멤버를 전진
	mCurrFrameResource->Fence = ++mCurrentFence;
	/*
	새 fence 지점을 설정하는 signal(명령)을 명령 대기열에 추가.
	지금 우리는 GPU 시간선 (timeline) 위에 있으므로 새 fence 지점은
	GPU가 이 signal() 명령 이전까지의 모든 명령을 처리하기 전까지는 설정되지 않음
	*/
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);
}

// 사용자 입력. 돌을 놓거나 놓을 위치 이동
void LitColumnsApp::OnKeyboardDown(WPARAM btnState)
{
	TCHAR pressedChar = static_cast<TCHAR>(btnState);
	// 사용자가 space를 눌렀을때 그게 가능한지 판단 후 돌을 놓는다
	if (btnState == VK_SPACE && haveControl && initColor[keyboard.x][keyboard.y] >= 0)
	{
		mOpaqueRitems[keyboard.x + keyboard.y * 8]->Mat = mMaterials["user1"].get();
		initColor[keyboard.x][keyboard.y] = -1;
		received = true;
		haveControl = false;
		return;
	}

	int currMat = initColor[keyboard.x][keyboard.y];
	int currX = keyboard.x, currY = keyboard.y;

	// 키보드 커서 이동
	if (pressedChar == 'w')
	{
		keyboard.x++;
	}
	else if (pressedChar == 'd')
	{
		keyboard.y++;
	}
	else if (pressedChar == 'a')
	{
		keyboard.y--;
	}
	else if (pressedChar == 's')
	{
		keyboard.x--;
	}

	if (isRange(keyboard.x, keyboard.y))
	{
		mOpaqueRitems[keyboard.x + keyboard.y * 8]->Mat = mMaterials["stone2"].get();
		if (currMat == 1)
		{
			mOpaqueRitems[currX + currY * 8]->Mat = mMaterials["stone1"].get();
		}
		else if (currMat == 0) {
			mOpaqueRitems[currX + currY * 8]->Mat = mMaterials["stone0"].get();
		}
		else if (currMat == -1) {
			mOpaqueRitems[currX + currY * 8]->Mat = mMaterials["user1"].get();
		}
		else if (currMat == -2) {
			mOpaqueRitems[currX + currY * 8]->Mat = mMaterials["user2"].get();
		}
	}
	else
	{
		mOpaqueRitems[currX + currY * 8]->Mat = mMaterials["stone2"].get();
		keyboard.x = currX;
		keyboard.y = currY;
	}

}


void LitColumnsApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	//mOpaqueRitems[0]->Mat = mMaterials["stone2"].get();



	SetCapture(mhMainWnd);
}

void LitColumnsApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
	//mOpaqueRitems[0]->Mat = mMaterials["stone0"].get();

}

// 마우스 왼쪽으로 끌었으면 회전 오른쪽은 축소확대
void LitColumnsApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.2 unit in the scene.
		float dx = 0.05f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.05f * static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 5.0f, 150.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void LitColumnsApp::OnKeyboardInput(const GameTimer& gt)
{
}

void LitColumnsApp::UpdateCamera(const GameTimer& gt)
{
	// 구면좌표계를 직교좌표계로 변환
	mEyePos.x = mRadius * sinf(mPhi) * cosf(mTheta);
	mEyePos.z = mRadius * sinf(mPhi) * sinf(mTheta);
	mEyePos.y = mRadius * cosf(mPhi);

	// 시야 행렬 만듬
	XMVECTOR pos = XMVectorSet(mEyePos.x, mEyePos.y, mEyePos.z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);
}

void LitColumnsApp::AnimateMaterials(const GameTimer& gt)
{

}

//상수 버퍼 업데이트
void LitColumnsApp::UpdateObjectCBs(const GameTimer& gt)
{
	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	for (auto& e : mAllRitems)
	{
		/*
		상수들이 바뀌었을때만 cubuffer 자료 갱신
		이러한 갱신은 저기 매개변수 오듯 프레임 자원마다 갱신 해야함 당연한거
		*/
		if (e->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));

			currObjectCB->CopyData(e->ObjCBIndex, objConstants);

			// 다음 프레임 자원으로 넘어감
			e->NumFramesDirty--;
		}
	}
}

//Material 업데이트
void LitColumnsApp::UpdateMaterialCBs(const GameTimer& gt)
{
	auto currMaterialCB = mCurrFrameResource->MaterialCB.get();
	for (auto& e : mMaterials)
	{
		/*
		상수들이 바뀌었을때만 cubuffer 자료 갱신
		이러한 갱신은 저기 매개변수 오듯 프레임 자원마다 갱신 해야함 당연한거
		*/
		Material* mat = e.second.get();
		if (mat->NumFramesDirty > 0)
		{
			XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

			MaterialConstants matConstants;
			matConstants.DiffuseAlbedo = mat->DiffuseAlbedo;
			matConstants.FresnelR0 = mat->FresnelR0;
			matConstants.Roughness = mat->Roughness;
			XMStoreFloat4x4(&matConstants.MatTransform, XMMatrixTranspose(matTransform));

			currMaterialCB->CopyData(mat->MatCBIndex, matConstants);

			// 다음 프레임 자원으로 넘어감
			mat->NumFramesDirty--;
		}
	}
}

// 상수 버퍼 업데이트2
/*
	passCB == FrameResource.h에서 변하지 않는 상수 자료를 저장.
	ex) 시점 위치, 시야 행렬, 투영행렬 등등..
*/
void LitColumnsApp::UpdateMainPassCB(const GameTimer& gt)
{
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	mMainPassCB.EyePosW = mEyePos;
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mClientWidth, 1.0f / mClientHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = gt.TotalTime();
	mMainPassCB.DeltaTime = gt.DeltaTime();
	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[0].Strength = { 0.6f, 0.6f, 0.6f };
	mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[1].Strength = { 0.3f, 0.3f, 0.3f };
	mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	mMainPassCB.Lights[2].Strength = { 0.15f, 0.15f, 0.15f };

	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

void LitColumnsApp::BuildRootSignature()
{
	/*
	일반적으로 셰이더 프로그램은 특정 자원 (상수 버퍼, 텍스처, 표본 추출기 등)이 입력된다고 기대함
	루트 서명은 셰이더 프로그램이 기대하는 자원들을 정의한다
	셰이더 프로그램은 본질적으로 하나의 함수이고 셰이더에 입력되는 자원들은 함수의 매개변수에 해당한다
	따라서 루트서명은 곧 함수 수명을 정의하는 수단이라고 볼 수 있음
	*/

	// 루트 매개변수는 서술자 테이블이거나 루트 서술자 또는 루트 상수.
	CD3DX12_ROOT_PARAMETER slotRootParameter[3];

	// cbv 하나를 담는 서술자 테이블을 생성한다
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);
	slotRootParameter[2].InitAsConstantBufferView(2);

	//루트 서명은 루트 매개변수들의 배열. 
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(3, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//상수 버퍼 하나로 구성된 서술자 구간을 가리키는 슬롯하나로 이루어진 루트 서명 생성
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf())));
}

void LitColumnsApp::BuildShadersAndInputLayout()
{
	const D3D_SHADER_MACRO alphaTestDefines[] =
	{
		"ALPHA_TEST", "1",
		NULL, NULL
	};

	mShaders["standardVS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
}


// 체스판에 들어갈 네모의 정점과 인덱스들을 정의
void LitColumnsApp::BuildShapeGeometry()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(1.5f, 0.5f, 1.5f, 3);
	UINT boxVertexOffset = 0;
	UINT boxIndexOffset = 0;

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;
	auto totalVertexCount =
		box.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
	}
	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "shapeGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["box"] = boxSubmesh;

	mGeometries[geo->Name] = std::move(geo);
}


void LitColumnsApp::BuildPSOs()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;
	ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	opaquePsoDesc.pRootSignature = mRootSignature.Get();
	opaquePsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()),
		mShaders["standardVS"]->GetBufferSize()
	};
	opaquePsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
		mShaders["opaquePS"]->GetBufferSize()
	};
	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePsoDesc.NumRenderTargets = 1;
	opaquePsoDesc.RTVFormats[0] = mBackBufferFormat;
	opaquePsoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	opaquePsoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	opaquePsoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&mOpaquePSO)));
}

void LitColumnsApp::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; ++i)
	{
		mFrameResources.push_back(std::make_unique<FrameResource>(md3dDevice.Get(),
			1, (UINT)mAllRitems.size(), (UINT)mMaterials.size()));
	}
}

void LitColumnsApp::BuildMaterials()
{

	auto stone0 = std::make_unique<Material>();
	stone0->Name = "stone0";
	stone0->MatCBIndex = 0;
	stone0->DiffuseSrvHeapIndex = 1;
	stone0->DiffuseAlbedo = XMFLOAT4(Colors::White);
	stone0->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	stone0->Roughness = 0.3f;

	auto stone1 = std::make_unique<Material>();
	stone1->Name = "stone1";
	stone1->MatCBIndex = 1;
	stone1->DiffuseSrvHeapIndex = 1;
	stone1->DiffuseAlbedo = XMFLOAT4(Colors::Black);
	stone1->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	stone1->Roughness = 0.3f;

	auto stone2 = std::make_unique<Material>();
	stone2->Name = "stone2";
	stone2->MatCBIndex = 2;
	stone2->DiffuseSrvHeapIndex = 1;
	stone2->DiffuseAlbedo = XMFLOAT4(Colors::Pink);
	stone2->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	stone2->Roughness = 0.3f;

	auto user1 = std::make_unique<Material>();
	user1->Name = "user1";
	user1->MatCBIndex = 3;
	user1->DiffuseSrvHeapIndex = 1;
	user1->DiffuseAlbedo = XMFLOAT4(Colors::Purple);
	user1->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	user1->Roughness = 0.3f;

	auto user2 = std::make_unique<Material>();
	user2->Name = "user2";
	user2->MatCBIndex = 4;
	user2->DiffuseSrvHeapIndex = 1;
	user2->DiffuseAlbedo = XMFLOAT4(Colors::Yellow);
	user2->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	user2->Roughness = 0.3f;




	mMaterials["stone0"] = std::move(stone0);
	mMaterials["stone1"] = std::move(stone1);
	mMaterials["stone2"] = std::move(stone2);
	mMaterials["user1"] = std::move(user1);
	mMaterials["user2"] = std::move(user2);
}

// 박스를 조합해서 체스판을 만듦
void LitColumnsApp::BuildRenderItems()
{
	UINT ObjCBind = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			auto boxRitem = std::make_unique<RenderItem>();
			// 세계 행렬 정의. 
			XMStoreFloat4x4(&boxRitem->World, XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(-10.0f + (i * 3.0f), 0.5f, -10.0f + (j * 3.0f)));
			// 크기 행렬 정의
			XMStoreFloat4x4(&boxRitem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
			boxRitem->ObjCBIndex = ObjCBind;
			ObjCBind++;

			//흰색 검정, 가운데 4타일 대로 체스판 색을 초기화
			if (initColor[i][j] == 1)
			{
				boxRitem->Mat = mMaterials["stone1"].get();
			}
			else if (initColor[i][j] == 0)
			{
				boxRitem->Mat = mMaterials["stone0"].get();
			}
			else if (initColor[i][j] == -1)
			{
				boxRitem->Mat = mMaterials["user1"].get();
			}
			else if (initColor[i][j] == -2)
			{
				boxRitem->Mat = mMaterials["user2"].get();
			}
			boxRitem->Geo = mGeometries["shapeGeo"].get();
			boxRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			boxRitem->IndexCount = boxRitem->Geo->DrawArgs["box"].IndexCount;
			boxRitem->StartIndexLocation = boxRitem->Geo->DrawArgs["box"].StartIndexLocation;
			boxRitem->BaseVertexLocation = boxRitem->Geo->DrawArgs["box"].BaseVertexLocation;
			mAllRitems.push_back(std::move(boxRitem));

		}
	}
	for (auto& e : mAllRitems)
	{
		mOpaqueRitems.push_back(e.get());
	}
}

void LitColumnsApp::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = mCurrFrameResource->ObjectCB->Resource();
	auto matCB = mCurrFrameResource->MaterialCB->Resource();

	// For each render item...
	for (size_t i = 0; i < ritems.size(); ++i)
	{
		auto ri = ritems[i];

		cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
		cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + ri->Mat->MatCBIndex * matCBByteSize;

		cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);
		cmdList->SetGraphicsRootConstantBufferView(1, matCBAddress);

		cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
	}
}