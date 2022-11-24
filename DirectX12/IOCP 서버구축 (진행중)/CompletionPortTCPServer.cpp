#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#define SERVERPORT 9000
#define BUFSIZE    512

struct SOCKETINFO
{
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
	int channel;
};

std::unordered_map<int, std::vector<SOCKETINFO*>> channel;
std::unordered_map<int, SOCKETINFO*> turn;
int channelNum = 0;

// 작업자 스레드 함수
DWORD WINAPI WorkerThread(LPVOID arg);
// 오류 출력 함수
void err_quit(char *msg);
void err_display(char *msg);

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) return 1;

	// 입출력 완료 포트 생성
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(hcp == NULL) return 1;

	// CPU 개수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// (CPU 개수 * 2)개의 작업자 스레드 생성
	HANDLE hThread;
	for(int i=0; i<(int)si.dwNumberOfProcessors*2; i++){
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if(hThread == NULL) return 1;
		CloseHandle(hThread);
	}

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");
	
	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", 
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 소켓과 입출력 완료 포트 연결
		CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);
		printf("%d 소켓 연결\n", client_sock);


		// 소켓 정보 구조체 할당
		SOCKETINFO *ptr = new SOCKETINFO;
		if(ptr == NULL) break;
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->sock = client_sock;
		ptr->recvbytes = ptr->sendbytes = 0;
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;

		// 비동기 입출력 시작
		flags = 0;
		retval = WSARecv(client_sock, &ptr->wsabuf, 1, &recvbytes,
			&flags, &ptr->overlapped, NULL);
		if(retval == SOCKET_ERROR){
			if(WSAGetLastError() != ERROR_IO_PENDING){
				err_display("WSARecv()");
			}
			continue;
		}
	}

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 작업자 스레드 함수
DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;
	while(1){
		// 비동기 입출력 완료 기다리기
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO *ptr;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(LPDWORD)&client_sock, (LPOVERLAPPED *)&ptr, INFINITE);
		
		// 클라이언트 정보 얻기
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
		// 비동기 입출력 결과 확인
		if(retval == 0 || cbTransferred == 0){
			if(retval == 0){
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped,
					&temp1, FALSE, &temp2);
				err_display("WSAGetOverlappedResult()");
			}
			
			printf("%d 소켓 해제\n", ptr->sock);
			printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			

			for (int i = 0; i < channel[ptr->channel].size(); i++)
			{
				if (ptr->sock == channel[ptr->channel][i]->sock)
				{
					channel[ptr->channel].erase(channel[ptr->channel].begin() + i);
					break;
				}
			}

			closesocket(ptr->sock);
			delete ptr;
			continue;
		}
		
		

		// 참가 요청일때
		if (ptr->buf[0] == '-')
		{
			printf("-----------------------%d\n", ptr->sock);
			DWORD recvbytes;
			DWORD flags = 0;
			ptr->recvbytes = 0;
			ptr->sendbytes = 0;
			ptr->buf[0] = ' ';
			
			retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					err_display("WSARecv()");
				}
				continue;
			}
		}

		if (ptr->buf[0] == '0')
		{
			int chan = ptr->buf[1] - '0';
			ptr->channel = chan;
			channel[chan].push_back(ptr);
			printf("현재 채널의 인원 %d 명, %d 채널에 추가시도.", channel[chan].size(), chan);
			if (channel[chan].size() == 2)
			{
				printf("매칭 완료\n");
				turn[chan] = channel[chan][channel[chan].size() - 2];
				for (SOCKETINFO* inChannelSocketInfo : channel[chan])
				{
					ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
					inChannelSocketInfo->wsabuf.buf[0] = '-';
					inChannelSocketInfo->wsabuf.len = 1;
					DWORD sendbytes;
					retval = WSASend(inChannelSocketInfo->sock, &inChannelSocketInfo->wsabuf, 1, &sendbytes, 0, &inChannelSocketInfo->overlapped, NULL);
					if (retval == SOCKET_ERROR) {
						if (WSAGetLastError() != WSA_IO_PENDING) {
							err_display("WSASend()");
						}
						continue;
					}
				}
			}
			else if (channel[chan].size() < 2)
			{
				printf("%d 소켓 추가 완료\n", ptr->sock);
			}
			else
			{
				printf("매칭 인원 초과 \n");
				channel[chan].pop_back();
				
				if (channel[chan][0]->sock != ptr->sock && channel[chan][1]->sock != ptr->sock)
				{
					closesocket(ptr->sock);
				}
				
				delete ptr;

			}
			continue;
		}

		printf("%d 소켓 입출력 요청\n", ptr->sock);
		printf("소켓 정보 %d %d\n", ptr->recvbytes, ptr->sendbytes);

		// 만약 현재 소켓의 recebyte가 0이라면 recvbyte에 cbTransferred 더한 후 WSARecv을 호출한다
		

		//만약 현재소켓의 recvbyte가 0보다 크다면 sendbyte에 cbTransferr더한다


		// 그넫 만약에 recvByte가 sendbyte를 역전했다 그러면 이제 데이터를 다른쪽으로 보내야한다

		// 근데 만약에 sendbyte가 recvbyte를 역전했다 이러면 받아야함


		
		// 데이터 전송량 갱신
		/*
		if(ptr->recvbytes == 0){
			ptr->recvbytes = cbTransferred;
			ptr->sendbytes = 0;
			// 받은 데이터 출력
			ptr->buf[ptr->recvbytes] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), ptr->buf);
		}
		else{
			ptr->sendbytes += cbTransferred;
		}
		if(ptr->recvbytes > ptr->sendbytes){
			// 데이터 보내기
			
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
			ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

			DWORD sendbytes;
			SOCKETINFO* nowTurn = turn[ptr->channel];
			turn[ptr->channel] = ptr;
			retval = WSASend(ptr->sock, &ptr->wsabuf, 1,&sendbytes, 0, &ptr->overlapped, NULL);
			if(retval == SOCKET_ERROR){
				if(WSAGetLastError() != WSA_IO_PENDING){
					err_display("WSASend()");
				}
				continue;
			}
			
		}
		else{
			ptr->recvbytes = 0;

			// 데이터 받기
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->buf;
			ptr->wsabuf.len = BUFSIZE;

			DWORD recvbytes;
			DWORD flags = 0;
			retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, 
				&recvbytes, &flags, &ptr->overlapped, NULL);
			if(retval == SOCKET_ERROR){
				if(WSAGetLastError() != WSA_IO_PENDING){
					err_display("WSARecv()");
				}
				continue;
			}
		}
		*/
	}
	
	return 0;
}

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}