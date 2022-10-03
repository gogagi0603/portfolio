#include "Othello_Client2.h"
bool LitColumnsApp::isRange(const int& a, const int& b)
{
	if (a >= 0 && a < 8 && b >= 0 && b < 8)
	{
		return true;
	}
	return false;
}

void LitColumnsApp::receivedPoint(char x, char y)
{
	mOpaqueRitems[x - '0' + (y - '0') * 8]->Mat = mMaterials["user1"].get();
	initColor[x - '0'][y - '0'] = -1;
	sended = false;
	haveControl = true;
	return;
}

void fn()
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE];
	int len;

	// 서버와 데이터 통신
	while (1) {
		// 데이터 입력
		while (!theApp->sended) //비효율적이므로 다른거 생각해보기
		{

		}
		buf[0] = theApp->getX() + '0';
		buf[1] = theApp->getY() + '0';

		theApp->checkChangeBlock(buf[0], buf[1]);
		// 데이터 보내기
		retval = send(sock, buf, 2, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// 데이터 받기
		retval = recvn(sock, buf, 2, 0);

		theApp->receivedPoint(buf[0], buf[1]);
		theApp->checkChangeBlock(buf[0], buf[1]);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 처리
	}

	// closesocket()
	closesocket(sock);

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
void LitColumnsApp::checkAvailableBlock()
{

}

//시간복잡도 주의
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

bool LitColumnsApp::isEndGame()
{
	return true;
}
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

int LitColumnsApp::getX()
{
	return keyboard.x;
}

int LitColumnsApp::getY()
{
	return keyboard.y;
}

void LitColumnsApp::OnResize()
{
	D3DApp::OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void LitColumnsApp::Update(const GameTimer& gt)
{
	OnKeyboardInput(gt);
	UpdateCamera(gt);

	// Cycle through the circular frame resource array.
	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

	// Has the GPU finished processing the commands of the current frame resource?
	// If not, wait until the GPU has completed commands up to this fence point.
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

	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(cmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mOpaquePSO.Get()));

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	auto passCB = mCurrFrameResource->PassCB->Resource();
	mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

	DrawRenderItems(mCommandList.Get(), mOpaqueRitems);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Advance the fence value to mark commands up to this fence point.
	mCurrFrameResource->Fence = ++mCurrentFence;

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);
}


void LitColumnsApp::OnKeyboardDown(WPARAM btnState)
{
	TCHAR pressedChar = static_cast<TCHAR>(btnState);
	if (btnState == VK_SPACE && haveControl && initColor[keyboard.x][keyboard.y] >= 0)
	{
		mOpaqueRitems[keyboard.x + keyboard.y * 8]->Mat = mMaterials["user2"].get();
		initColor[keyboard.x][keyboard.y] = -2;
		sended = true;
		haveControl = false;
		return;
	}

	int currMat = initColor[keyboard.x][keyboard.y];
	int currX = keyboard.x, currY = keyboard.y;

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
	// Convert Spherical to Cartesian coordinates.
	mEyePos.x = mRadius * sinf(mPhi) * cosf(mTheta);
	mEyePos.z = mRadius * sinf(mPhi) * sinf(mTheta);
	mEyePos.y = mRadius * cosf(mPhi);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(mEyePos.x, mEyePos.y, mEyePos.z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);
}

void LitColumnsApp::AnimateMaterials(const GameTimer& gt)
{

}

void LitColumnsApp::UpdateObjectCBs(const GameTimer& gt)
{
	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	for (auto& e : mAllRitems)
	{
		// Only update the cbuffer data if the constants have changed.  
		// This needs to be tracked per frame resource.
		if (e->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));

			currObjectCB->CopyData(e->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			e->NumFramesDirty--;
		}
	}
}

void LitColumnsApp::UpdateMaterialCBs(const GameTimer& gt)
{
	auto currMaterialCB = mCurrFrameResource->MaterialCB.get();
	for (auto& e : mMaterials)
	{
		// Only update the cbuffer data if the constants have changed.  If the cbuffer
		// data changes, it needs to be updated for each FrameResource.
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

			// Next FrameResource need to be updated too.
			mat->NumFramesDirty--;
		}
	}
}

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
	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[3];

	// Create root CBV.
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);
	slotRootParameter[2].InitAsConstantBufferView(2);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(3, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
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

void LitColumnsApp::BuildShapeGeometry()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(1.5f, 0.5f, 1.5f, 3);

	//
	// We are concatenating all the geometry into one big vertex/index buffer.  So
	// define the regions in the buffer each submesh covers.
	//

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	UINT boxVertexOffset = 0;

	// Cache the starting index for each object in the concatenated index buffer.
	UINT boxIndexOffset = 0;

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;


	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

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

	//
	// PSO for opaque objects.
	//
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

void LitColumnsApp::BuildRenderItems()
{
	UINT ObjCBind = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			auto boxRitem = std::make_unique<RenderItem>();
			XMStoreFloat4x4(&boxRitem->World, XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(-10.0f + (i * 3.0f), 0.5f, -10.0f + (j * 3.0f)));
			XMStoreFloat4x4(&boxRitem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
			boxRitem->ObjCBIndex = ObjCBind;
			ObjCBind++;
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