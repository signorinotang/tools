#ifndef _UIPICTURE_SETUP_
#define _UIPICTURE_SETUP_
#include "vnUIPicture.h"
#include "vnUIRenderPicture.h"
//#include "vnUIRenderer.h"


class UIPictureSetup : public vn::UIPicture {
	VN_DECL_UI_ELEMENT()
public:
	UIPictureSetup();
	UIPictureSetup(const UIPictureSetup& other);
	virtual UIPictureSetup* clone() const;
	
	//methods
	virtual void init(const vn::TreeDataObject* object);

	//mouseEvent
	virtual void onMouseLeftDown(const vn::vector2f &position);
	virtual void onMouseLeftUp(const vn::vector2f &position);

	//tool
	const vn::vector2f& getOriginalPoint() const;
	void setOriginalPoint(const vn::vector2f& point);
	bool isMouseDown();

protected:
	virtual void _onUpdate(vn::f32 deltaTime);
	virtual void _onRender(vn::UIRenderer* renderer);
	vn::vector2f m_originalPoint;
	bool m_mouseDown;
	vn::vector2f m_lastMousePos;
};

#endif