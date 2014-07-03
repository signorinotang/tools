#include "UIPictureSetup.h"
#include "vnUIRoot.h"
#include "vnGfxApplication.h"

VN_IMPL_UI_ELEMENT(UIPictureSetup, "PictureSetup")
UIPictureSetup::UIPictureSetup()
: m_originalPoint(0,0)
, m_mouseDown(false) {

}

UIPictureSetup::UIPictureSetup(const UIPictureSetup& other)
:UIPicture(other) {

}

UIPictureSetup* UIPictureSetup::clone() const{
	return new UIPictureSetup();
}

void UIPictureSetup::init(const vn::TreeDataObject* object) {
	UIPicture::init(object);

}

void UIPictureSetup::onMouseLeftDown(const vn::vector2f& position) {
	vn::UIRoot::instance().captureMouse(this);
	m_mouseDown = true;
	m_lastMousePos = position;
}

void UIPictureSetup::onMouseLeftUp(const vn::vector2f& position) {
	m_mouseDown = false;
	vn::UIRoot::instance().releaseMouse(this);
}

void UIPictureSetup::_onUpdate(vn::f32 deltaTime) {
	UIPicture::_onUpdate(deltaTime);
	if (!m_mouseDown) {
		return;
	}
	vn::aabox2f rect = m_boundingBox; 
	vn::vector2f pos = vn::GfxApplication::instance().getMousePosition();
	vn::vector2f delta = pos - m_lastMousePos;
	if (!delta.x && !delta.y) {
		return;
	}
	m_lastMousePos = pos;
	rect += delta;
	setLocation(rect);

	m_originalPoint += delta;

}

void UIPictureSetup::_onRender(vn::UIRenderer* renderer) {
	UIPicture::_onRender(renderer);

}

const vn::vector2f& UIPictureSetup::getOriginalPoint() const {
	return m_originalPoint;
}


bool UIPictureSetup::isMouseDown() {
	return m_mouseDown;
}


void UIPictureSetup::setOriginalPoint(const vn::vector2f& point){
	m_originalPoint = point;
	return;
}