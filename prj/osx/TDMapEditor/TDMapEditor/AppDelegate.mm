//
//  AppDelegate.m
//  TDMapEditor
//
//  Created by Wayne on 13-12-26.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#import "AppDelegate.h"
#include "vnLog.h"
#include "vnFileManager.h"
#include "../../../../src/TDMapEditor/EditorApp.h"

@implementation AppDelegate

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
    return YES;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // setup log.
    vn::Log::instance().addPrinter(vnnew vn::LogPrinterSTDOUT(), false);
    
    // init file systems.
    vn::str8 main_path = [[[NSBundle mainBundle] bundlePath] UTF8String];
    size_t pos = main_path.find_last_of('/');
    main_path.erase(main_path.begin() + pos + 1, main_path.end());
    chdir(main_path.c_str());
    vn::str8 res_path = [[[NSBundle mainBundle] resourcePath] UTF8String];
    vn::FileManager::instance().setFileSystem(1, vnnew vn::FileSystemOS(res_path), false);
    
    // set app's delegate
    vn::GfxApplication::instance().setDelegate(&EditorApp::instance());
    
    vn::GfxApplication::instance().createWindow(1024, 768, "TDMapEditor");
    
    vn::GfxApplication::instance().run();
}

- (IBAction)performUndo:(id)sender
{
	vn::GfxApplication::instance().performUndo();
}

- (IBAction)performRedo:(id)sender
{
	vn::GfxApplication::instance().performRedo();
}

- (IBAction)performCut:(id)sender
{
	vn::GfxApplication::instance().performCut();
}

- (IBAction)performCopy:(id)sender
{
	vn::GfxApplication::instance().performCopy();
}

- (IBAction)performPaste:(id)sender
{
	vn::GfxApplication::instance().performPaste();
}

@end
