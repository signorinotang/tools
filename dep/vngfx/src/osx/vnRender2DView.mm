//
//  vnRender2DView.m
//  vngfx
//
//  Created by Wayne on 13-12-13.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#import "vnRender2DView.h"
#include "vnGfxApplicationImpl.h"
#include "vnScopedLock.h"

static CVReturn display_link_callback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime,CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext) {
    [(__bridge Render2DView *)displayLinkContext updateAndDraw];
    return kCVReturnSuccess;
}

@implementation Render2DView

- (id)initWithFrame:(NSRect)frame
{
    NSOpenGLPixelFormatAttribute attrs[] = {
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 24,
		// Must specify the 3.2 Core Profile to use OpenGL 3.2
		NSOpenGLPFAOpenGLProfile,
		NSOpenGLProfileVersion3_2Core,
		0
	};
    NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    self = [self initWithFrame:frame pixelFormat:pf];
    if (!self) {
        return nil;
    }
    
    m_markedRange = NSMakeRange(NSNotFound, 0);
    m_inputPosition = NSMakePoint(0, frame.size.height);
    m_inputEnabled = false;
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification object:[self window]];
    
    [[self openGLContext] makeCurrentContext];
    
    CVDisplayLinkCreateWithActiveCGDisplays(&m_displayLink);
    CVDisplayLinkSetOutputCallback(m_displayLink, &display_link_callback, (__bridge void *)self);
    
    _vn_ns::internal::GfxApplicationImpl::_instance->_view_init((__bridge void *)self);
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
	[super drawRect:dirtyRect];
	
    // Drawing code here.
    [self updateAndDraw];
}

- (void)reshape
{
    [super reshape];
    NSRect viewRectPoints = [self bounds];
    //NSOpenGLContext * __weak obj = [self openGLContext];
    //CGLContextObj cobj = (CGLContextObj)[obj CGLContextObj];
    //CGLLockContext(cobj);
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_view_resize(viewRectPoints.size.width, viewRectPoints.size.height);
    //CGLUnlockContext(cobj);
}

- (void)renewGState
{
	[[self window] disableScreenUpdatesUntilFlush];
	[super renewGState];
}

- (BOOL) acceptsFirstResponder
{
    return YES;
}

- (BOOL)becomeFirstResponder {
    return YES;
}

- (BOOL)resignFirstResponder {
    return YES;
}

- (void)windowWillClose:(NSNotification*)notification
{
    _vn_ns::internal::GfxApplicationImpl::_instance->_view_close();
    CVDisplayLinkRelease(m_displayLink);
    m_displayLink = 0;
}

- (void)updateAndDraw
{
    NSOpenGLContext * __weak obj = [self openGLContext];
    //CGLContextObj cobj = (CGLContextObj)[obj CGLContextObj];
    [obj makeCurrentContext];
    //CGLLockContext(cobj);
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_view_update();
    _vn_ns::internal::GfxApplicationImpl::_instance->_view_draw();
    //CGLFlushDrawable(cobj);
	//CGLUnlockContext(cobj);
}

- (void)present
{
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

- (void)start
{
    CVDisplayLinkStart(m_displayLink);
}

- (void)stop
{
    CVDisplayLinkStop(m_displayLink);
}

- (void)thread_init
{
    [[self openGLContext] makeCurrentContext];
}

- (void)thread_lock
{
    //CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    m_lock.lock();
}

- (void)thread_unlock
{
    //CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    m_lock.unlock();
}

- (void)setTextInputEnabled:(bool)enabled
{
    if (!(m_inputEnabled = enabled) && [self hasMarkedText]) {
        [self unmarkText];
    }
}

- (bool)isTextInputEnabled
{
    return m_inputEnabled;
}


- (void)setTextInputPositionX:(float)x andY:(float)y
{
    m_inputPosition = NSMakePoint(x, y);
}

- (void)undo:(id)sender
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->performUndo();
}

- (void)redo:(id)sender
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->performRedo();
}

#pragma mark -

- (void)mouseDown:(NSEvent *)theEvent
{
    NSPoint pt = [theEvent locationInWindow];
    //pt = [self convertPoint:pt fromView:nil];
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_mouse_button_down(0, pt.x, pt.y);
}

- (void)mouseUp:(NSEvent *)theEvent
{
    NSPoint pt = [theEvent locationInWindow];
    //pt = [self convertPoint:pt fromView:nil];
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_mouse_button_up(0, pt.x, pt.y);
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
    [super rightMouseDown:theEvent];
    
    NSPoint pt = [theEvent locationInWindow];
    //pt = [self convertPoint:pt fromView:nil];
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_mouse_button_down(1, pt.x, pt.y);
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
    NSPoint pt = [theEvent locationInWindow];
    //pt = [self convertPoint:pt fromView:nil];
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_mouse_button_up(1, pt.x, pt.y);
}

- (void)scrollWheel:(NSEvent *)theEvent
{
    NSPoint pt = [theEvent locationInWindow];
    //pt = [self convertPoint:pt fromView:nil];
    float dx = theEvent.deltaX;
    float dy = theEvent.deltaY;
    //if (dx || dy) {
        _vn_ns::ScopedLock<LockType> guard(m_lock);
        _vn_ns::internal::GfxApplicationImpl::_instance->_mouse_wheel(pt.x, pt.y, dx, dy);
    //}
}

- (void)keyDown:(NSEvent *)theEvent
{
    if (!([NSEvent modifierFlags] & NSCommandKeyMask)) {
        _vn_ns::ScopedLock<LockType> guard(m_lock);
        _vn_ns::internal::GfxApplicationImpl::_instance->_key_down(*(unsigned int *)[theEvent.characters cStringUsingEncoding:NSUTF32StringEncoding], theEvent.isARepeat != NO);
    }
    if (m_inputEnabled) {
        [self.inputContext handleEvent:theEvent];
    }
}

- (void)keyUp:(NSEvent *)theEvent
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_key_up(*(unsigned int *)[theEvent.characters cStringUsingEncoding:NSUTF32StringEncoding]);
}

#pragma mark -

- (void)insertNewline:(id)sender
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_text_newLine();
}

- (void)deleteForward:(id)sender
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_text_deleteForward();
}

- (void)deleteBackward:(id)sender
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_text_deleteBackward();
}

- (void)moveUp:(id)sender
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
	_vn_ns::internal::GfxApplicationImpl::_instance->_text_move_up();
}

- (void)moveDown:(id)sender
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
	_vn_ns::internal::GfxApplicationImpl::_instance->_text_move_down();
}

- (void)moveLeft:(id)sender
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
	_vn_ns::internal::GfxApplicationImpl::_instance->_text_move_left();
}

- (void)moveRight:(id)sender
{
    _vn_ns::ScopedLock<LockType> guard(m_lock);
	_vn_ns::internal::GfxApplicationImpl::_instance->_text_move_right();
}

- (void)cancelOperation:(id)sender
{
	
}

#pragma mark -

/* The receiver inserts aString replacing the content specified by replacementRange. aString can be either an NSString or NSAttributedString instance.
 */
- (void)insertText:(id)aString replacementRange:(NSRange)replacementRange
{
    if ([self hasMarkedText]) {
        [self unmarkText];
    }
    //std::cout << "insert: \"" << [aString UTF8String] << "\"" << std::endl;
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_text_insert((const vn::c32 *)[aString cStringUsingEncoding:NSUTF32StringEncoding]);
}

/* The receiver invokes the action specified by aSelector.
 */
- (void)doCommandBySelector:(SEL)aSelector
{
    [super doCommandBySelector:aSelector]; // NSResponder's implementation will do nicely
}

/* The receiver inserts aString replacing the content specified by replacementRange. aString can be either an NSString or NSAttributedString instance. selectedRange specifies the selection inside the string being inserted; hence, the location is relative to the beginning of aString. When aString is an NSString, the receiver is expected to render the marked text with distinguishing appearance (i.e. NSTextView renders with -markedTextAttributes).
 */
- (void)setMarkedText:(id)aString selectedRange:(NSRange)selectedRange replacementRange:(NSRange)replacementRange
{
    
    NSUInteger len = [aString length];
    if (len) {
        m_markedRange = NSMakeRange(0, len);
        _vn_ns::ScopedLock<LockType> guard(m_lock);
        _vn_ns::internal::GfxApplicationImpl::_instance->_text_mark((const vn::c32 *)[[aString string] cStringUsingEncoding:NSUTF32StringEncoding], (vn::u32)selectedRange.location);
    } else {
        [self unmarkText];
    }
    
}

/* The receiver unmarks the marked text. If no marked text, the invocation of this method has no effect.
 */
- (void)unmarkText
{
    m_markedRange = NSMakeRange(NSNotFound, 0);
    [[self inputContext] discardMarkedText];
    _vn_ns::ScopedLock<LockType> guard(m_lock);
    _vn_ns::internal::GfxApplicationImpl::_instance->_text_unmark();
}

/* Returns the selection range. The valid location is from 0 to the document length.
 */
- (NSRange)selectedRange
{
    return NSMakeRange(NSNotFound, 0);
}

/* Returns the marked range. Returns {NSNotFound, 0} if no marked range.
 */
- (NSRange)markedRange
{
    return m_markedRange;
}

/* Returns whether or not the receiver has marked text.
 */
- (BOOL)hasMarkedText
{
    return m_markedRange.location != NSNotFound;
}

/* Returns attributed string specified by aRange. It may return nil. If non-nil return value and actualRange is non-NULL, it contains the actual range for the return value. The range can be adjusted from various reasons (i.e. adjust to grapheme cluster boundary, performance optimization, etc).
 */
- (NSAttributedString *)attributedSubstringForProposedRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange
{
    return nil;
}

/* Returns an array of attribute names recognized by the receiver.
 */
- (NSArray*)validAttributesForMarkedText
{
    return nil;
}

/* Returns the first logical rectangular area for aRange. The return value is in the screen coordinate. The size value can be negative if the text flows to the left. If non-NULL, actuallRange contains the character range corresponding to the returned area.
 */
- (NSRect)firstRectForCharacterRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange
{
    NSRect rect = {};
    rect.origin = [_window convertBaseToScreen:[self bounds].origin];
    rect.origin.x += m_inputPosition.x;
    rect.origin.y += m_inputPosition.y;
    return rect;
}

/* Returns the index for character that is nearest to aPoint. aPoint is in the screen coordinate system.
 */
- (NSUInteger)characterIndexForPoint:(NSPoint)aPoint
{
    return NSNotFound;
}

#pragma mark -

@end