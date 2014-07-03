#include "UIListItemCheckBox.h"


VN_IMPL_UI_LOGIC(UIListItemCheckBox, "ListItemCheckBox")
UIListItemCheckBox::UIListItemCheckBox()
: m_checkBox(0) {

}

UIListItemCheckBox::UIListItemCheckBox(const UIListItemCheckBox& other)
: UIListItemText(other)
, m_checkBox(0)
{

}
UIListItemCheckBox * UIListItemCheckBox::clone() const {
	return vnnew UIListItemCheckBox(*this);
}



void UIListItemCheckBox::init(const vn::TreeDataObject* object) {
	UIListItemText::init(object);

}
 

bool UIListItemCheckBox::_startup(vn::UIFactory::ElementMap &namedElements) {
	UIListItemText::_startup(namedElements);

	std::map<vn::str8, vn::UIElement *>::iterator it;
	it = namedElements.find("checkbox");
	if (it == namedElements.end()) {
		return false;
	}
	m_checkBox = dynamic_cast<vn::UIButtonDefault*>(it->second);
	if (!m_checkBox) {
		return false;
	}
	m_checkBox->bindAction("clicked", vn::make_memfn(this, &UIListItemCheckBox::_onClicked));
	
	return true;
}


void UIListItemCheckBox::_onClicked(vn::UIElement *pSender) {

	m_checkBox->setPushed(!m_checkBox->isPushed());
}


bool UIListItemCheckBox::isCheckBoxPushed() {
	return m_checkBox->isPushed();
}

void UIListItemCheckBox::setCheckBoxPushed()
{
	m_checkBox->setPushed(true);
}

void UIListItemCheckBox::invertSelection() {
	m_checkBox->setPushed(!m_checkBox->isPushed());
}