//
//  vnGfxView.h
//  vngfx
//
//  Created by Wayne on 13-12-13.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol GfxView <NSObject>

@required

-(void)setTextInputEnabled:(bool)enabled;
-(bool)isTextInputEnabled;
-(void)setTextInputPositionX:(float)x andY:(float)y;
-(void)start;
-(void)stop;
-(void)thread_init;
-(void)thread_lock;
-(void)thread_unlock;

@end
