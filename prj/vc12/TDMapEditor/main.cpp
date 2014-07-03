//
//  main.h
//  TDMapEditor
//
//  Created by Wayne on 14-1-3.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "..\..\..\src\TDMapEditor\EditorApp.h"
#include "vnLog.h"
#include "vnFileManager.h"
#include "vnGfxApplication.h"

#ifdef _DEBUG
int wmain() {
	vn::Log::instance().addPrinter(vnnew vn::LogPrinterSTDOUT(), false);
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
#endif

	vn::FileManager::instance().setFileSystem(1, vnnew vn::FileSystemOS("res/"), false);
	vn::GfxApplication::instance().setDelegate(&EditorApp::instance());

	vn::GfxApplication::instance().createWindow(1024, 768, "TDMapEditor");
	return vn::GfxApplication::instance().run();
}