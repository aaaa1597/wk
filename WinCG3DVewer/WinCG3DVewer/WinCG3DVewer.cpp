// WinCG3DVewer.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <map>
#include <string>
#include <stdio.h>
#include "framework.h"
#include "include/GL2/gl2.h"
#include "../../AndCG3DViewer/app/src/main/cpp/CG3DCom.h"
#include "../../AndCG3DViewer/app/src/main/cpp/OpenGL/GLES2.h"
#include "../../AndCG3DViewer/app/src/main/cpp/format/MQO.h"
#include "../../AndCG3DViewer/app/src/main/cpp/format/import_fbx.h"
#include "WinCG3DVewer.h"
#include "ComDef.h"

#define ErrorMessage(x)	MessageBoxA(NULL, x, "ERROR", MB_ICONERROR)

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

#pragma warning(disable : 4996)
#pragma warning(disable : 6031)

static int WGLCreate(HWND window);
static void WGLDestroy();
static int Initialize();
static int SetDrawArea(int width, int height);
static void DrawScene();
static void SetTouchAngle(int x, int y);

/* レンダリングコンテキスト */
static HGLRC wglContext = 0;

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ここにコードを挿入してください。

	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINCG3DVEWER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーション初期化の実行:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINCG3DVEWER));

	MSG msg;

	// メイン メッセージ ループ:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		DrawScene();
		//InvalidateRect(ghWnd, NULL, FALSE);
	}

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINCG3DVEWER));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCEW(IDC_WINCG3DVEWER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
    if (!WGLCreate(hWnd)) {
		return FALSE;
	}
	if (!Initialize()) {
		return FALSE;
	}

	RECT rc;
	GetWindowRect(hWnd, &rc);
	if (!SetDrawArea(rc.right-rc.left, rc.bottom-rc.top)) {
		return FALSE;
	}

	return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウを描画する
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 選択されたメニューの解析:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
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
		// TODO: HDC を使用する描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {
			int x = HIWORD(lParam);
			int y = LOWORD(lParam);
			SetTouchAngle(x, y);
		}
		break;
	case WM_SIZE:
		{
			RECT rc;
			GetWindowRect(hWnd, &rc);
			SetDrawArea(rc.right-rc.left, rc.bottom-rc.top);
		}
		break;
	case WM_DESTROY:
		WGLDestroy();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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
}


/*
 *   WGLCreate: レンダリングコンテキストの作成
 */
int WGLCreate(HWND window)
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // support window 
		PFD_SUPPORT_OPENGL |   // support OpenGL 
		PFD_DOUBLEBUFFER,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		32,                    // 32-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		32,                    // 32-bit z-buffer 
		0,                     // no stencil buffer 
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
	};
	int  iPixelFormat;
	HDC hdc;
	HGLRC rc;


	hdc = GetDC(window);

	iPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, iPixelFormat, &pfd);

	rc = wglCreateContext(hdc);

	wglMakeCurrent(hdc, rc);

	/*
	 *   OpenGL ES APIの準備
	 */
	if (!SetupGLSLES100()) {
		ErrorMessage("Not support OpenGL ES 2.0\n");
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(rc);
		return 0;
	}

	if (dmpIsSupported("WGL_EXT_create_context_es2_profile")) {
		if (wglCreateContextAttribsARB) {
			/* ES2 PROFILE */
			const int attribList[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB,  2,
				WGL_CONTEXT_MINOR_VERSION_ARB,  0,
				WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_ES2_PROFILE_BIT_EXT,
				0,
			};

			wglContext = wglCreateContextAttribsARB(hdc, NULL, attribList);
		}

		if (!wglContext) {
			ErrorMessage("Not support OpenGL ES 2.0\n");
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(rc);
			return 0;
		}
		wglMakeCurrent(hdc, wglContext);
		wglDeleteContext(rc);
	}
	else {
		int major, minor;

		/* OpenGL バージョンチェック */
		char* version = (char*)glGetString(GL_VERSION);

		sscanf(version, "%d.%d", &major, &minor);
		if (major >= 5 || (major == 4 && major >= 1)) {
			wglContext = rc;
		}
		else {
			ErrorMessage("Not support OpenGL ES 2.0\n");
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(rc);
			return 0;
		}
	}

	if (wglGetExtensionsStringARB) {
		CG3D::TRACE("WGL_EXTENSIONS              : %s\n", wglGetExtensionsStringARB(hdc));
	}

	/* 垂直同期 */
	if (wglSwapIntervalEXT) {
		wglSwapIntervalEXT(1);
	}

	return 1;
}

/*
 *   WGLDestroy: レンダリングコンテキストの削除
 */
void WGLDestroy()
{
	//glDetachShader(program, vs);
	//glDetachShader(program, fs);

	//glDeleteShader(vs);
	//glDeleteShader(fs);

	//glDeleteProgram(program);
	//glDeleteTextures(1, &tex);

	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(wglContext);
}

int Initialize() {
	std::vector<std::string> assetFiles;
	/* assetsのファイルリスト取得 */
	const WCHAR* basepath = L"../../AndCG3DViewer/app/src/main/assets";
	WIN32_FIND_DATA win32fd;
	WCHAR basepathtmp[512] = { 0 };
	memcpy(basepathtmp, basepath, wcslen(basepath) * 2);
	HANDLE hFind = FindFirstFile(wcscat(basepathtmp, L"/*.*"), &win32fd);
	do {
		if (wcscmp(win32fd.cFileName, L".") == 0) continue;
		if (wcscmp(win32fd.cFileName, L"..") == 0) continue;
		if (wcscmp(win32fd.cFileName, L"images") == 0) continue;
		if (wcscmp(win32fd.cFileName, L"webkit") == 0) continue;

		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			WIN32_FIND_DATA win32fd2;
			WCHAR serchpath2[512] = { 0 };
			memcpy(serchpath2, basepath, wcslen(basepath) * 2);
			wcscat(serchpath2, L"/");
			wcscat(serchpath2, win32fd.cFileName);
			HANDLE hFind2 = FindFirstFile(wcscat(serchpath2, L"/*.*"), &win32fd2);
			do {
				if (wcscmp(win32fd2.cFileName, L".") == 0) continue;
				if (wcscmp(win32fd2.cFileName, L"..") == 0) continue;

				if (win32fd2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					/* フォルダだったらその配下を追加 */
					WIN32_FIND_DATA win32fd3;
					WCHAR serchpath3[512] = { 0 };
					memcpy(serchpath3, basepath, wcslen(basepath) * 2);
					wcscat(serchpath3, L"/");
					wcscat(serchpath3, win32fd.cFileName);
					wcscat(serchpath3, L"/");
					wcscat(serchpath3, win32fd2.cFileName);
					HANDLE hFind3 = FindFirstFile(wcscat(serchpath3, L"/*.*"), &win32fd3);
					do {
						if (wcscmp(win32fd3.cFileName, L".") == 0) continue;
						if (wcscmp(win32fd3.cFileName, L"..") == 0) continue;
						/* ファイルが見つかったらVector配列に保存する */
						WCHAR tmpbuf3[512] = { 0 };
						memcpy(tmpbuf3, win32fd.cFileName, wcslen(win32fd.cFileName) * 2);
						wcscat(tmpbuf3, L"/");
						wcscat(tmpbuf3, win32fd2.cFileName);
						wcscat(tmpbuf3, L"/");
						wcscat(tmpbuf3, win32fd3.cFileName);
						char c[550] = { 0 };
						wcstombs(c, tmpbuf3, wcslen(tmpbuf3));
						assetFiles.push_back(c);
					} while (FindNextFile(hFind3, &win32fd3));

				}
				else {
					/* ファイルが見つかったらVector配列に保存する */
					WCHAR tmpbuf2[512] = { 0 };
					memcpy(tmpbuf2, win32fd.cFileName, wcslen(win32fd.cFileName) * 2);
					wcscat(tmpbuf2, L"/");
					wcscat(tmpbuf2, win32fd2.cFileName);
					char c[550] = { 0 };
					wcstombs(c, tmpbuf2, wcslen(tmpbuf2));
					assetFiles.push_back(c);
				}
			} while (FindNextFile(hFind2, &win32fd2));
		}
		else {
			/* ファイルが見つかったらVector配列に保存する */
			WCHAR tmpbuf[512] = { 0 };
			char c[550] = { 0 };
			wcstombs(c, win32fd.cFileName, sizeof(win32fd.cFileName));
			assetFiles.push_back(c);
		}
	} while (FindNextFile(hFind, &win32fd));

	std::cout << "assetsのデータ一括取得 s" << std::endl;

	/* assetsのデータ一括取得 */
	std::string basepathstr = "../../AndCG3DViewer/app/src/main/assets/";
	std::map<std::string, std::vector<char>> AssetDatas;
	std::for_each(assetFiles.begin(), assetFiles.end(), [basepathstr, &AssetDatas](std::string f) {
		/* 読込 */
		std::ifstream ifs(basepathstr + f, std::ios::in | std::ios::binary);
		ifs.seekg(0, std::ios::end);
		size_t size = (size_t)ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		std::vector<char> buf(size);
		ifs.read(buf.data(), size);
		AssetDatas.insert({ f, buf });
	});

	std::cout << "assetsのデータ一括取得 e" << std::endl;
	GlRenderData& RenderData = GlRenderData::GetIns();

	bool ret = GLES2::OpenGLInit();
	if (!ret) return false;
	bool ret4 = GLES2::ShaderInit(AssetDatas.at("Shaders/mqocol.vsh"), AssetDatas.at("Shaders/mqocol.fsh"), AssetDatas.at("Shaders/mqotex.vsh"), AssetDatas.at("Shaders/mqotex.fsh"), RenderData.mGlShaderObjs, RenderData.mTexId);
	if (!ret4) return false;
	bool ret5 = GLES2::BufferInit(RenderData.mGlBufferObj);
	if (!ret5) return false;

	MqoInfo MqoInfo;
	bool ret0 = false;
	std::tie(ret0, MqoInfo) = MQO::init(AssetDatas.at("vignette_ppp.mqo"));
	if (!ret0) return false;
	bool ret3 = MQO::remakeDrawInfo(MqoInfo, RenderData.mDrawInfos);
	if (!ret3) return false;
	bool ret6 = MQO::TextureInit(AssetDatas, MqoInfo.mMqoMaterials, RenderData.mDrawInfos);
	if (!ret6) return false;

	bool ret7 = false;
	/* TODO std::tie(ret7, FbxInfo) = */
	ret7 = fbx::import_fbx::load(AssetDatas.at("dragon56-fbx/Dragon 2.5_fbx.fbx"));

	if (!ret7) return false;

	return 1;
}

int SetDrawArea(int width, int height) {
	glViewport(0, 0, width, height);     // 表示領域を設定する
	__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "aaaaa  width=%d height=%d\n", width, height);

	std::array<float, 16> projMatrix = MatrixVector::GetPerspectivef(30.0, (float)width / height, 1.0, 5000.0);
	std::array<float, 16> viewMatrix = MatrixVector::GetLookAtf(0.0f, 250.0f, 1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	MatrixVector::MultMatrixf(GlRenderData::GetIns().mVpMatrix, projMatrix, viewMatrix);
	return 1;
}

void DrawScene() {
	GLES2::draw();
	SwapBuffers(wglGetCurrentDC());
	return;
}

void SetTouchAngle(int x, int y) {
	GlRenderData& RenderData = GlRenderData::GetIns();
	RenderData.mTouchAngleX = (float)x;
	RenderData.mTouchAngleY = (float)y;
	return;
}
