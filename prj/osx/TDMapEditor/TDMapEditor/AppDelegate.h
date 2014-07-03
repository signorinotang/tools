//
//  AppDelegate.h
//  TDMapEditor
//
//  Created by Wayne on 13-12-26.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;

- (IBAction)performUndo:(id)sender;
- (IBAction)performRedo:(id)sender;
- (IBAction)performCut:(id)sender;
- (IBAction)performCopy:(id)sender;
- (IBAction)performPaste:(id)sender;

@end
