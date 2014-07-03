#include "vnGfxApplication.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <clocale>
#include "vnColor.h"
#include "vnFileManager.h"
#include "vnLog.h"
#include <map>
#include "EditorApp.h"

#ifdef VN_DEBUG
int main()
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpzCmdLine, int nShowCmd)
#endif
{	

	_wsetlocale(LC_ALL, L".65001");

	vn::Log::instance().addPrinter(vnnew vn::LogPrinterSTDOUT(), false);

	vn::GfxApplication &app = vn::GfxApplication::instance();

	app.setDelegate(&EditorApp::instance());

	app.createWindow(1024, 768, "EditorApp");

	app.run(); 

	return 0;
}