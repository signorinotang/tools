#include "vnUIListItem.h"
#include "vnUIFactory.h"
#include "vnUIButtonDefault.h"

class UIListItemCheckBox : public vn::UIListItemText {
	VN_DECL_UI_LOGIC()
public:
	UIListItemCheckBox();
	UIListItemCheckBox(const UIListItemCheckBox& other);
	virtual UIListItemCheckBox* clone() const;

	virtual void init(const vn::TreeDataObject* object);

	void setCheckBoxPushed();
	void invertSelection();
	bool isCheckBoxPushed();
	

private:
	virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
	void _onClicked(vn::UIElement *);
	vn::UIButtonDefault* m_checkBox;

};
