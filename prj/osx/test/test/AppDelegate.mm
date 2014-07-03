//
//  AppDelegate.m
//  test
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#import "AppDelegate.h"
#include "vnLog.h"
#include "vnFileManager.h"
#include "../../../../src/test/TestApp.h"

@implementation AppDelegate

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
    return YES;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // setup log.
    vn::Log::instance().addPrinter(vnnew vn::LogPrinterSTDOUT(), false);
    
    // init file systems.
    vn::FileManager::instance().setFileSystem(0, vnnew vn::FileSystemOS("/"), false);
    vn::str8 res_path = [[[NSBundle mainBundle] resourcePath] UTF8String];
    vn::FileManager::instance().setFileSystem(1, vnnew vn::FileSystemOS(res_path), false);
    
    // set app's delegate
    vn::GfxApplication::instance().setDelegate(&TestApp::instance());
    
    vn::GfxApplication::instance().createWindow(1024, 768, "test");
    
    vn::GfxApplication::instance().run();
}

@end
