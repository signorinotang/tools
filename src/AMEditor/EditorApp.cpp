#include "EditorApp.h"
#include "vnFontManager.h"
#include "vnTreeDataScript.h"
#include "vnUIButtonDefault.h"
#include "vnUIListItem.h"
#include "vnLog.h"
#include "vnUnicode.h"
#include "Animations.h"
#include "vnPictureManager.h"
#include "vnUIPicture.h"
#include <iostream>
#include "vnFileManager.h"




EditorApp::EditorApp()
: m_fileBrowser(0)
, m_AnimGroup(0)
, m_currentAnimIndex(-1)
, m_currentFrameIndex(-1)
, m_bPlaying(false)
, m_click(false)
, m_doubleClick(false)
, m_timer(0.05f)
, m_currentItemIndex(-1)
, m_preview(0)
, m_currentDummyIndex(-1)
{

}

EditorApp::~EditorApp()
{
	if (m_AnimGroup)
	{
		VN_SAFE_DELETE(m_AnimGroup)
	}
}


void EditorApp::_init(const vn::vector2f &size) {
	_loadConfigFile();
	vn::FontManager::instance().createFontFT("", "1:/common/msyh.ttf", 24);

	vn::UIFactory::instance().createElementsFromTDL(m_uiRootElement, "1:/AMEditor/ui.tdl");

	vn::UIFactory::ElementMap namedElements;

	m_uiRootElement->buildElementMap(namedElements);

	namedElements["text"]->bindAction("string", vn::make_memfn(this, &EditorApp::_onString));
	namedElements["button"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	//add by ts

	//uipicture
	m_preview = (vn::UIPicture*)namedElements["preview"];
	
	
	//button click
	namedElements["new"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["delete"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["+"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["-"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["play"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["stop"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["save"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["load"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["E+"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["E-"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["import"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	namedElements["export"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onClicked));
	//window button click
	namedElements["selectAll"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));
	namedElements["invertSelect"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));
	namedElements["confirm"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));
	namedElements["close"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));
	namedElements["1-"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));
	namedElements["2-"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));
	namedElements["3-"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));
	namedElements["4-"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));
	namedElements["5-"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));
	namedElements["6-"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onWindowButtonClicked));



	//window
	m_window = (vn::UIWindow*)namedElements["window"];
	m_window->setVisible(false);
	//picture setup
	m_pictureSetup = (UIPictureSetup*)namedElements["picturesetup"];
	m_axis = (UIPictureSetup*)namedElements["axis"];
	m_dummy = (UIPictureSetup*)namedElements["dummy"];
	//test list
	m_checkBoxList = (vn::UIList*)namedElements["checkBoxList"];
	m_checkBoxList->bindAction_SelectedChanged(vn::make_memfn(this, &EditorApp::_onCheckBoxSelectedChanged));
	m_checkBoxList->bindAction_ItemClicked(vn::make_memfn(this, &EditorApp::_onCheckBoxItemClicked));

	//list click
	m_animationsList = (vn::UIList*)namedElements["animationsList"];
	m_animationsList->bindAction_SelectedChanged(vn::make_memfn(this, &EditorApp::_onAnimSelectedChanged));
	m_animationsList->bindAction_ItemClicked(vn::make_memfn(this, &EditorApp::_onAnimItemClicked));

	m_picturesList = (vn::UIList*)namedElements["picturesList"];
	m_picturesList->bindAction_SelectedChanged(vn::make_memfn(this, &EditorApp::_onPicSelectedChanged));
	m_picturesList->bindAction_ItemClicked(vn::make_memfn(this, &EditorApp::_onPicItemClicked));
	
	m_dummyList = (vn::UIList*)namedElements["dummyList"];
	m_dummyList->bindAction_SelectedChanged(vn::make_memfn(this, &EditorApp::_onDummySelectedChanged));
	m_dummyList->bindAction_ItemClicked(vn::make_memfn(this, &EditorApp::_onDummyClicked));
	
	
	m_parametersList = (vn::UIList*)namedElements["parametersList"];
	m_parametersList->bindAction_SelectedChanged(vn::make_memfn(this, &EditorApp::_onParSelectedChanged));
	m_parametersList->bindAction_ItemClicked(vn::make_memfn(this, &EditorApp::_onParItemClicked));

	m_valueList = (vn::UIList*)namedElements["valueList"];
	m_valueList->bindAction_SelectedChanged(vn::make_memfn(this, &EditorApp::_onValSelectedChanged));
	m_valueList->bindAction_ItemClicked(vn::make_memfn(this, &EditorApp::_onValItemClicked));

	m_parametersList->addItem()->setText("");
	m_valueList->addItem()->setText("");
	m_parametersList->addItem()->setText("");
	m_valueList->addItem()->setText("");
	m_parametersList->addItem()->setText("");
	m_valueList->addItem()->setText("");
	m_parametersList->addItem()->setText("");
	m_valueList->addItem()->setText("");
	m_parametersList->addItem()->setText("");
	m_valueList->addItem()->setText("");
	m_parametersList->addItem()->setText("");
	m_valueList->addItem()->setText("");
	m_parametersList->addItem()->setText("");
	m_valueList->addItem()->setText("");
	m_parametersList->addItem()->setText("");
	m_valueList->addItem()->setText("");

	//string
	m_textFiled = (vn::UITextField*)namedElements["animationsName"];
	m_valueTextFiled = (vn::UITextField*)namedElements["valueTextFiled"];
	m_dummyTextFiled = (vn::UITextField*)namedElements["dummyName"];
	
	m_textFiled->bindAction_TextCompleted(vn::make_memfn(this, &EditorApp::_onString));
	m_valueTextFiled->bindAction_TextCompleted(vn::make_memfn(this, &EditorApp::_onStringParameters));
	m_dummyTextFiled->bindAction_TextCompleted(vn::make_memfn(this, &EditorApp::_onStringDummy));

	m_bkg = (vn::UIFrame*)namedElements["bkg"];
	m_valueBkg = (vn::UIFrame*)namedElements["valuebkg"];
	m_dummyBkg = (vn::UIFrame*)namedElements["dummybkg"];

	m_bkg->setVisible(false);
	m_valueBkg->setVisible(false);
	m_dummyBkg->setVisible(false);
	//create animations
	m_AnimGroup = new Animations();
	vn::UIRenderPicture1* renderPic = vnnew vn::UIRenderPicture1();
	m_pictureSetup->setPicture(renderPic);

	m_AnimGroup->setPictureSetup(m_pictureSetup);
	m_AnimGroup->setPictureAxis(m_axis);


	vn::TreeDataScript tds;
	vn::TreeDataNode *tdnode = tds.parse("1:/common/file_browser.tdl");
	if (tdnode) {
		m_fileBrowser = vnnew UIFileBrowser();
		m_fileBrowser->init(tdnode);
		m_fileBrowser->startup();
		m_fileBrowser->bindAction_FileOpen(vn::make_memfn(this, &EditorApp::_onFileOpen));
		tdnode->drop();
	}
	m_fileBrowser->setTitle("Add Image To Animation");
	m_fileBrowser->setImagesMode(false);
	m_fileBrowser->setExtensionFilter(".imgs");

	//m_fileBrowser->setReadOnly(false);
	vn::UILogic *logic = vn::UIFactory::instance().createLogicFromTDL("1:/AMEditor/file_browser_save.tdl");
	if (logic) {
		m_saveFileBrowser = dynamic_cast<UIFileBrowser *>(logic);
		if (!m_saveFileBrowser) {

			logic->drop();
		}
		else {
			m_saveFileBrowser->startup();
			m_saveFileBrowser->bindAction_FileOpen(vn::make_memfn(this, &EditorApp::_onSave));
		}
	}
	m_saveFileBrowser->setTitle("Save Animations");
	m_saveFileBrowser->setImagesMode(false);
	m_saveFileBrowser->setExtensionFilter(".ans");
	m_saveFileBrowser->setReadOnly(false);

	logic = vn::UIFactory::instance().createLogicFromTDL("1:/AMEditor/file_browser_load.tdl");
	if (logic) {
		m_loadFileBrowser = dynamic_cast<UIFileBrowser *>(logic);
		if (!m_loadFileBrowser) {
			logic->drop();
		}
		else {
			m_loadFileBrowser->startup();
			m_loadFileBrowser->bindAction_FileOpen(vn::make_memfn(this, &EditorApp::_onLoad));
		}
	}
	m_loadFileBrowser->setTitle("Load Animations");
	m_loadFileBrowser->setImagesMode(false);
	m_loadFileBrowser->setExtensionFilter(".ans");
	m_loadFileBrowser->setReadOnly(false);

	logic = vn::UIFactory::instance().createLogicFromTDL("1:/AMEditor/file_browser_import.tdl");
	if (logic) {
		m_importBorwser = dynamic_cast<UIFileBrowser *>(logic);
		if (!m_importBorwser) {
			logic->drop();
		}
		else {
			m_importBorwser->startup();
			m_importBorwser->bindAction_FileOpen(vn::make_memfn(this, &EditorApp::_onImport));
		}
	}
	m_importBorwser->setTitle("Import From TDL");
	m_importBorwser->setImagesMode(false);
	m_importBorwser->setExtensionFilter(".tdl");
	m_importBorwser->setReadOnly(false);

	logic = vn::UIFactory::instance().createLogicFromTDL("1:/AMEditor/file_browser_export.tdl");
	if (logic) {
		m_exportBorwser = dynamic_cast<UIFileBrowser *>(logic);
		if (!m_exportBorwser) {
			logic->drop();
		}
		else {
			m_exportBorwser->startup();
			m_exportBorwser->bindAction_FileOpen(vn::make_memfn(this, &EditorApp::_onExport));
		}
	}
	m_exportBorwser->setTitle("Export To TDL");
	m_exportBorwser->setImagesMode(false);
	m_exportBorwser->setExtensionFilter(".tdl");
	m_exportBorwser->setReadOnly(false);
}

void EditorApp::_fini() {
	VN_SAFE_DROP(m_fileBrowser);
	VN_SAFE_DROP(m_saveFileBrowser);
	VN_SAFE_DROP(m_loadFileBrowser);
	VN_SAFE_DROP(m_importBorwser);
	VN_SAFE_DROP(m_exportBorwser);
}

void EditorApp::_update(vn::f32 deltaTime) {
	if (m_bPlaying)
	{
		if (!m_AnimGroup->isNull(m_currentAnimIndex))
		{
			vn::u8 index = m_AnimGroup->getFrameIndex(m_currentAnimIndex);
			m_picturesList->setSelectedItem(m_picturesList->item(index));	
		}
	}

	if (m_click) {
		m_timer -= deltaTime;
		if (m_timer < 0)
		{
			m_timer = 0.5f;
			m_click = false;
		}
	}

	m_AnimGroup->update(deltaTime, m_currentAnimIndex);

	if (m_pictureSetup->isMouseDown() && m_currentFrameIndex != -1 && !m_bPlaying){
		std::cout << "x:" << m_pictureSetup->getOriginalPoint().x << "y:" << m_pictureSetup->getOriginalPoint().y << std::endl;
		vn::vector2f point = m_pictureSetup->getOriginalPoint();
		char buf[64];
		sprintf(buf, "%g", point.x);
		m_valueList->item(3)->setText(buf);
		sprintf(buf, "%g", point.y);
		m_valueList->item(4)->setText(buf);
		m_AnimGroup->setOrignalPoint(m_currentAnimIndex, m_currentFrameIndex, point);

	}
	if (m_axis->isMouseDown() && m_currentAnimIndex != -1) {
		vn::vector2f point = m_axis->getOriginalPoint();
		char buf[64];
		sprintf(buf, "%g", point.x);
		m_valueList->item(1)->setText(buf);
		sprintf(buf, "%g", point.y);
		m_valueList->item(2)->setText(buf);
		m_AnimGroup->setAnimPoint(m_currentAnimIndex, point);
		std::cout << "x:" << m_axis->getOriginalPoint().x << "y:" << m_axis->getOriginalPoint().y << std::endl;
	}
	if (m_dummy->isMouseDown() && m_currentDummyIndex != -1) {
		vn::vector2f point = m_dummy->getOriginalPoint();
		char buf[64];
		sprintf(buf, "%g", point.x);
		m_valueList->item(5)->setText(buf);
		sprintf(buf, "%g", point.y);
		m_valueList->item(6)->setText(buf);
		m_AnimGroup->setDummyPoint(m_currentAnimIndex, m_currentFrameIndex, m_currentDummyIndex, point);
	}


}
void updateAnimOriginalPoint(vn::f32 deltaTime) {

}

void EditorApp::_render() {
	m_AnimGroup->showFrmae(m_currentAnimIndex, m_currentFrameIndex);
}
void EditorApp::_resize(const vn::vector2f &size) {



}

void EditorApp::_loadConfigFile() {
	vn::TreeDataScript tds;
	vn::TreeDataNode *root = tds.parse(vn::FilePath(0, "config.tdl"));
	if (!root) {
		return;
	}
	for (vn::TreeDataNode *p = root->firstChild(); p; p = p->nextNeighbour()) {
		if (p->name() == "FileSystem") {
			vn::u32 fsId = p->queryAttributeInteger("id", -1);
			vn::str8 path;
			if (fsId != -1 && p->queryAttributeString("path", path)) {
				vn::FileManager::instance().setFileSystem(fsId, vnnew vn::FileSystemOS(path), false);
			}
		}
	}
	root->drop();
}

void EditorApp::_onString(vn::UIElement *sender, const vn::str32 &test) {
	m_animationsList->getSelectedItem()->setText(test);

	vn::str8 t;
	vn::Unicode::convert(test.c_str(), test.length(), t);
	m_AnimGroup->setAnimName(m_currentAnimIndex, t);

	m_bkg->setVisible(false);
	m_textFiled->setText("");
}
void EditorApp::_onStringParameters(vn::UIElement *sender, const vn::str32 &test) {
	
	m_valueList->item(m_currentItemIndex)->setText(test);
	
	vn::str8 t;
	vn::Unicode::convert(test.c_str(), test.length(), t);

	bool ok = false;
	vn::f32 value = 20.f;
	try {
		size_t idx;
		vn::f32 f = std::stof(t, &idx);
		if (idx == t.length()) {
			value = f;
			ok = true;
		}
	}
	catch (std::invalid_argument &) {

	}
	VN_LOG_DEBUG("value:" << value << " ok: " << ok );

	if (ok)
	{
		if (m_currentItemIndex == 0 && m_currentAnimIndex != -1)
		{
			m_AnimGroup->setAnimFps(m_currentAnimIndex, value);
		}
		else if (m_currentItemIndex == 1 && m_currentAnimIndex != -1) {
			vn::vector2f point = m_AnimGroup->getAnimPoint(m_currentAnimIndex);
			point.x = value;
			m_axis->setOriginalPoint(vn::vector2f(value, m_axis->getOriginalPoint().y));
			m_AnimGroup->setAnimPoint(m_currentAnimIndex, point);

			point = m_AnimGroup->getAnimPoint(m_currentAnimIndex);
			vn::vector2f min = vn::UIRoot::instance().getViewSize() / 2;
			min += point;
			vn::vector2f max = min + vn::vector2f(300, 300);
			m_axis->setLocation(vn::aabox2f(min, max));
			

		}
		else if (m_currentItemIndex == 2 && m_currentAnimIndex != -1) {
			vn::vector2f point = m_AnimGroup->getAnimPoint(m_currentAnimIndex);
			point.y = value;
			m_axis->setOriginalPoint(vn::vector2f(m_axis->getOriginalPoint().x, value));
			m_AnimGroup->setAnimPoint(m_currentAnimIndex, point);

			point = m_AnimGroup->getAnimPoint(m_currentAnimIndex);
			vn::vector2f min = vn::UIRoot::instance().getViewSize() / 2;
			min += point;
			vn::vector2f max = min + vn::vector2f(300, 300);
			m_axis->setLocation(vn::aabox2f(min, max));
		}
		else if (m_currentItemIndex == 3 && m_currentFrameIndex != -1)
		{
			vn::vector2f point = m_AnimGroup->originalPoint(m_currentAnimIndex, m_currentFrameIndex);
			point.x = value;
		
			m_pictureSetup->setOriginalPoint(vn::vector2f(value, m_pictureSetup->getOriginalPoint().y));
			m_AnimGroup->setOrignalPoint(m_currentAnimIndex, m_currentFrameIndex, point);
		}
		else if (m_currentItemIndex == 4 && m_currentFrameIndex != -1)
		{
			vn::vector2f point = m_AnimGroup->originalPoint(m_currentAnimIndex, m_currentFrameIndex);
			point.y = value;
			m_pictureSetup->setOriginalPoint(vn::vector2f(m_pictureSetup->getOriginalPoint().x,value));
			m_AnimGroup->setOrignalPoint(m_currentAnimIndex, m_currentFrameIndex, point);
		}
		else if (m_currentItemIndex == 5 && m_currentDummyIndex != -1)
		{
			vn::vector2f point = m_AnimGroup->DummyPoint(m_currentAnimIndex, m_currentFrameIndex, m_currentDummyIndex);
			point.x = value;
			m_dummy->setOriginalPoint(vn::vector2f(value, m_pictureSetup->getOriginalPoint().y));
			m_AnimGroup->setDummyPoint(m_currentAnimIndex, m_currentFrameIndex, m_currentDummyIndex, point);
			vn::aabox2f box = m_pictureSetup->boundingBox();
			vn::aabox2f dummyBox;
			dummyBox.min_corner = box.min_corner;
			dummyBox.min_corner += (point - vn::vector2f(25, 25));
			dummyBox.max_corner = dummyBox.min_corner + vn::vector2f(50, 50);
			m_dummy->setLocation(dummyBox);
		}
		else if (m_currentItemIndex == 6 && m_currentDummyIndex != -1) 
		{
			vn::vector2f point = m_AnimGroup->DummyPoint(m_currentAnimIndex, m_currentFrameIndex, m_currentDummyIndex);
			point.y = value;
			m_dummy->setOriginalPoint(vn::vector2f(m_pictureSetup->getOriginalPoint().x, value));
			m_AnimGroup->setDummyPoint(m_currentAnimIndex, m_currentFrameIndex, m_currentDummyIndex, point);
			vn::aabox2f box = m_pictureSetup->boundingBox();
			vn::aabox2f dummyBox;
			dummyBox.min_corner = box.min_corner;
			dummyBox.min_corner += (point - vn::vector2f(25, 25));
			dummyBox.max_corner = dummyBox.min_corner + vn::vector2f(50, 50);
			m_dummy->setLocation(dummyBox);

		} else if (m_currentItemIndex == 7 && m_currentAnimIndex != -1) {
			m_AnimGroup->setAnimLayer(m_currentAnimIndex, value);
		}
		m_valueBkg->setVisible(false);
	}

}
void EditorApp::_onStringDummy(vn::UIElement* sender, const vn::str32& test){
	m_dummyList->getSelectedItem()->setText(test);
	vn::str8 t;
	vn::Unicode::convert(test.c_str(), test.length(), t);
	m_AnimGroup->setDummyName(m_currentAnimIndex, m_currentFrameIndex, m_currentDummyIndex, t);
	m_dummyBkg->setVisible(false);
	m_dummyTextFiled->setText("");
}

void EditorApp::_onClicked(vn::UIElement *sender) {

	//m_fileBrowser->show(m_uiRootElement);
	vn::str8 name = sender->getName();
	if (name == "new" && !m_bPlaying)
	{
		m_animationsList->addItem()->setText("animtion");
		m_AnimGroup->createAnimation();
	}
	else if (name == "delete" && !m_bPlaying)
	{
		if (m_currentAnimIndex != -1)
		{
			if (m_currentFrameIndex != -1) {
				static_cast<vn::UIRenderPicture1 *>(m_pictureSetup->getPicture().ptr())->setPicture(vn::PicturePtr::kNull);
			}
			m_AnimGroup->deleteAnimtion(m_currentAnimIndex);
			m_animationsList->eraseItem(m_currentAnimIndex);
			m_picturesList->clearItems();
			m_currentAnimIndex = -1;
		}
	}
	else if (name == "+" && !m_bPlaying)
	{
		if (m_animationsList->getSelectedItem())
		{
			m_fileBrowser->show(m_uiRootElement);
		}
	}
	else if (name == "-" && !m_bPlaying)
	{
		if (m_currentFrameIndex == -1)
		{
			return;
		}
		static_cast<vn::UIRenderPicture1 *>(m_pictureSetup->getPicture().ptr())->setPicture(vn::PicturePtr::kNull);
		m_AnimGroup->deleteImageFromAnim(m_currentAnimIndex, m_currentFrameIndex);
		m_picturesList->eraseItem(m_currentFrameIndex);
		
	}
	else if (name == "E+" && !m_bPlaying && m_currentFrameIndex != -1 && m_currentAnimIndex != -1) {
		m_dummyList->addItem()->setText("cast");
		m_AnimGroup->addDummy(m_currentAnimIndex, m_currentFrameIndex);
	}
	else if (name == "E-" && !m_bPlaying && m_currentDummyIndex != -1) {

		m_AnimGroup->deleteDummy(m_currentAnimIndex, m_currentFrameIndex, m_currentDummyIndex);
		m_dummyList->eraseItem(m_currentDummyIndex);
	}
	else if (name == "play" && !m_bPlaying && m_currentAnimIndex != -1)
	{
		m_AnimGroup->playAnimation(m_currentAnimIndex);
		m_bPlaying = true;
	}
	else if (name == "stop" && m_bPlaying)
	{
		m_AnimGroup->stopAnimation(m_currentAnimIndex);
		m_bPlaying = false;
	} 
	else if (name == "save" && !m_bPlaying) {
		m_saveFileBrowser->show(m_uiRootElement);
	}
	else if (name == "load" && !m_bPlaying) {
		m_loadFileBrowser->show(m_uiRootElement);
	} 
	else if (name == "import" && !m_bPlaying) {
		m_importBorwser->show(m_uiRootElement);
	}
	else if (name == "export" && !m_bPlaying) {
		m_exportBorwser->show(m_uiRootElement);
	}



}
void EditorApp::_onWindowButtonClicked(vn::UIElement* sender) {
	vn::str8 name = sender->getName();
	if (name == "selectAll") {
		vn::u16 count = m_checkBoxList->itemCount();
		for (vn::u16 i = 0; i < count; ++i)
		{
			UIListItemCheckBox* tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			if (tmp){
				tmp->setCheckBoxPushed();
			}
		}
	}
	else if (name == "1-") {
		vn::u16 count = m_checkBoxList->itemCount();
		for (vn::u16 i = 0; i < count; ++i)
		{
			UIListItemCheckBox* tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			vn::str8 s;
			tmp->getText(s);
			if (!strncmp(s.c_str(), "1-", 2)) {
				tmp->setCheckBoxPushed();
			}
		}
		m_currentAnimIndex = m_animationsList->itemIndex(m_animationsList->getSelectedItem());
	
		vn::str8 imagePath;
		vn::str8 name;
		//vn::str8 fullPath;
		m_imagePath.getFullPath(imagePath);
		UIListItemCheckBox* tmp;

		for (vn::u16 i = 0; i < count; ++i) {
			tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			if (tmp && tmp->isCheckBoxPushed())	{
				tmp->getText(name);
				//fullPath = imagePath + '|' + name;
				m_picturesList->addItem()->setText(name);
				m_AnimGroup->addImageToAnim(m_currentAnimIndex, imagePath, name);
			}
		}
		m_window->setVisible(false);
		m_checkBoxList->clearItems();
	} else if (name == "2-") {
		vn::u16 count = m_checkBoxList->itemCount();
		for (vn::u16 i = 0; i < count; ++i)
		{
			UIListItemCheckBox* tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			vn::str8 s;
			tmp->getText(s);
			if (!strncmp(s.c_str(), "2-", 2)) {
				tmp->setCheckBoxPushed();
			}
		}
		m_currentAnimIndex = m_animationsList->itemIndex(m_animationsList->getSelectedItem());
		
		vn::str8 imagePath;
		vn::str8 name;
		//vn::str8 fullPath;
		m_imagePath.getFullPath(imagePath);
		UIListItemCheckBox* tmp;

		for (vn::u16 i = 0; i < count; ++i) {
			tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			if (tmp && tmp->isCheckBoxPushed())	{
				tmp->getText(name);
				//fullPath = imagePath + '|' + name;
				m_picturesList->addItem()->setText(name);
				m_AnimGroup->addImageToAnim(m_currentAnimIndex, imagePath, name);
			}
		}
		m_window->setVisible(false);
		m_checkBoxList->clearItems();
	} else if (name == "3-") {
		vn::u16 count = m_checkBoxList->itemCount();
		for (vn::u16 i = 0; i < count; ++i)
		{
			UIListItemCheckBox* tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			vn::str8 s;
			tmp->getText(s);
			if (!strncmp(s.c_str(), "3-", 2)) {
				tmp->setCheckBoxPushed();
			}
		}
		m_currentAnimIndex = m_animationsList->itemIndex(m_animationsList->getSelectedItem());
		
		vn::str8 imagePath;
		vn::str8 name;
		//vn::str8 fullPath;
		m_imagePath.getFullPath(imagePath);
		UIListItemCheckBox* tmp;

		for (vn::u16 i = 0; i < count; ++i) {
			tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			if (tmp && tmp->isCheckBoxPushed())	{
				tmp->getText(name);
				//fullPath = imagePath + '|' + name;
				m_picturesList->addItem()->setText(name);
				m_AnimGroup->addImageToAnim(m_currentAnimIndex, imagePath, name);
			}
		}
		m_window->setVisible(false);
		m_checkBoxList->clearItems();

	} else if (name == "4-") {
		vn::u16 count = m_checkBoxList->itemCount();
		for (vn::u16 i = 0; i < count; ++i)
		{
			UIListItemCheckBox* tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			vn::str8 s;
			tmp->getText(s);
			if (!strncmp(s.c_str(), "4-", 2)) {
				tmp->setCheckBoxPushed();
			}
		}
		m_currentAnimIndex = m_animationsList->itemIndex(m_animationsList->getSelectedItem());
		
		vn::str8 imagePath;
		vn::str8 name;
		//vn::str8 fullPath;
		m_imagePath.getFullPath(imagePath);
		UIListItemCheckBox* tmp;

		for (vn::u16 i = 0; i < count; ++i) {
			tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			if (tmp && tmp->isCheckBoxPushed())	{
				tmp->getText(name);
				//fullPath = imagePath + '|' + name;
				m_picturesList->addItem()->setText(name);
				m_AnimGroup->addImageToAnim(m_currentAnimIndex, imagePath, name);
			}
		}
		m_window->setVisible(false);
		m_checkBoxList->clearItems();
	} else if (name == "5-") {
		vn::u16 count = m_checkBoxList->itemCount();
		for (vn::u16 i = 0; i < count; ++i)
		{
			UIListItemCheckBox* tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			vn::str8 s;
			tmp->getText(s);
			if (!strncmp(s.c_str(), "5-", 2)) {
				tmp->setCheckBoxPushed();
			}
		}
		m_currentAnimIndex = m_animationsList->itemIndex(m_animationsList->getSelectedItem());
		
		vn::str8 imagePath;
		vn::str8 name;
		//vn::str8 fullPath;
		m_imagePath.getFullPath(imagePath);
		UIListItemCheckBox* tmp;

		for (vn::u16 i = 0; i < count; ++i) {
			tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			if (tmp && tmp->isCheckBoxPushed())	{
				tmp->getText(name);
				//fullPath = imagePath + '|' + name;
				m_picturesList->addItem()->setText(name);
				m_AnimGroup->addImageToAnim(m_currentAnimIndex, imagePath, name);
			}
		}
		m_window->setVisible(false);
		m_checkBoxList->clearItems();
	} else if (name == "6-") {
		vn::u16 count = m_checkBoxList->itemCount();
		for (vn::u16 i = 0; i < count; ++i)
		{
			UIListItemCheckBox* tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			vn::str8 s;
			tmp->getText(s);
			if (!strncmp(s.c_str(), "6-", 2)) {
				tmp->setCheckBoxPushed();
			}
		}
		m_currentAnimIndex = m_animationsList->itemIndex(m_animationsList->getSelectedItem());
	
		vn::str8 imagePath;
		vn::str8 name;
		//vn::str8 fullPath;
		m_imagePath.getFullPath(imagePath);
		UIListItemCheckBox* tmp;

		for (vn::u16 i = 0; i < count; ++i) {
			tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			if (tmp && tmp->isCheckBoxPushed())	{
				tmp->getText(name);
				//fullPath = imagePath + '|' + name;
				m_picturesList->addItem()->setText(name);
				m_AnimGroup->addImageToAnim(m_currentAnimIndex, imagePath, name);
			}
		}
		m_window->setVisible(false);
		m_checkBoxList->clearItems();
	}
	else if (name == "invertSelect") {
		vn::u16 count = m_checkBoxList->itemCount();
		UIListItemCheckBox* tmp;
		for (vn::u16 i = 0; i < count; ++i)
		{
			tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			if (tmp){
				tmp->invertSelection();
			}
		}

	}
	else if (name == "confirm") {
		m_currentAnimIndex = m_animationsList->itemIndex(m_animationsList->getSelectedItem());
		vn::u16 count = m_checkBoxList->itemCount();
		vn::str8 imagePath;
		vn::str8 name;
		//vn::str8 fullPath;
		m_imagePath.getFullPath(imagePath);
		UIListItemCheckBox* tmp;

		for (vn::u16 i = 0; i < count; ++i) {
			tmp = dynamic_cast<UIListItemCheckBox*>(m_checkBoxList->item(i));
			if (tmp && tmp->isCheckBoxPushed())	{
				tmp->getText(name);
				//fullPath = imagePath + '|' + name;
				m_picturesList->addItem()->setText(name);
				m_AnimGroup->addImageToAnim(m_currentAnimIndex, imagePath, name);
			}
		}
		m_window->setVisible(false);
		m_checkBoxList->clearItems();
	}
	else if (name == "close")
	{
		m_window->setVisible(false);
		m_checkBoxList->clearItems();	
	}
}

void EditorApp::_onAnimSelectedChanged(vn::UIElement* sender, vn::UIListItem* item) {

	if (item)
	{
		m_picturesList->clearItems();
		m_bkg->setVisible(false);

		if (m_currentAnimIndex != -1){
			m_AnimGroup->resetCurrentIndex(m_currentAnimIndex);
		}
		m_currentAnimIndex = m_animationsList->itemIndex(item);
		if (m_bPlaying) {
			m_AnimGroup->playAnimation(m_currentAnimIndex);
		}
		//***************************************************************
		m_parametersList->setVisible(true);
		m_valueList->setVisible(true);
		vn::f32 fps = m_AnimGroup->getAnimFps(m_currentAnimIndex);	
		vn::vector2f point = m_AnimGroup->getAnimPoint(m_currentAnimIndex);
		vn::s32 layer = m_AnimGroup->getAnimLayer(m_currentAnimIndex);
		char buff[128];
		sprintf(buff, "%g", fps);
		m_parametersList->item(0)->setText("Fps");
		m_valueList->item(0)->setText(buff);
		sprintf(buff, "%g", point.x);
		m_parametersList->item(1)->setText("animtionX:");
		m_valueList->item(1)->setText(buff);
		sprintf(buff, "%g", point.y);
		m_parametersList->item(2)->setText("animtionY:");
		m_valueList->item(2)->setText(buff);
		sprintf(buff, "%d", layer);
		m_parametersList->item(7)->setText("animLayer");
		m_valueList->item(7)->setText(buff);

		vn::vector2f min = vn::UIRoot::instance().getViewSize() / 2;
		min += point;
		vn::vector2f max = min + vn::vector2f(300, 300);
		m_axis->setLocation(vn::aabox2f(min, max));
		m_axis->setOriginalPoint(point);
     	//**************************************************************
		Animation* tmpAni = m_AnimGroup->getAnimations()[m_currentAnimIndex];
		vn::u8 size = tmpAni->m_names.size();

		for (int i = 0; i < size; ++i)
		{
			m_picturesList->addItem()->setText(tmpAni->m_names[i]);
		}
		
	}
    else{
		m_AnimGroup->resetCurrentIndex(m_currentAnimIndex);
		m_currentAnimIndex = -1;
		m_picturesList->clearItems();
		m_dummyList->clearItems();
		m_parametersList->setVisible(false);
		m_valueList->setVisible(false);

		vn::vector2f min = vn::UIRoot::instance().getViewSize() / 2;
		vn::vector2f max = min + vn::vector2f(300, 300);
		m_axis->setLocation(vn::aabox2f(min, max));
		min += vn::vector2f(4, 4);
		max = min + vn::vector2f(200, 200);
		m_pictureSetup->setLocation(vn::aabox2f(min, max));
		m_pictureSetup->setColor(0xFFAAAAAA);
	}
}
void EditorApp::_onAnimItemClicked(vn::UIElement* sender, vn::UIListItem* item) {
	if (m_bPlaying)
	{
		if (m_animationsList->itemIndex(item) > m_AnimGroup->getAnimations().size() - 1){
			return;
		}
	}

	if (item) {
		if (m_click == true)
		{
			vn::aabox2f box = item->root()->boundingBox();
			m_bkg->setVisible(true);
			m_bkg->setLocation(box);
			vn::str8 str;
			item->getText(str);
			m_textFiled->setText(str);
			m_textFiled->selectAll();
			vn::UIRoot::instance().requireFocus(m_textFiled);
		}
	}
	else {
		m_bkg->setVisible(false);
	}
	m_click = true;
	m_animationsList->setSelectedItem(item);	


}

void EditorApp::_onDummySelectedChanged(vn::UIElement* sender, vn::UIListItem* item) {
	if (item) {
		m_currentDummyIndex = m_dummyList->itemIndex(item);
		vn::vector2f point = m_AnimGroup->DummyPoint(m_currentAnimIndex, m_currentFrameIndex, m_currentDummyIndex);
		m_parametersList->item(5)->setText("Dummy X:");
		m_parametersList->item(6)->setText("Dummy Y:");
		char buf[64];
		sprintf(buf, "%g", point.x);
		m_valueList->item(5)->setText(buf);
		sprintf(buf, "%g", point.y);
		m_valueList->item(6)->setText(buf);
		m_dummy->setOriginalPoint(point);
		vn::aabox2f box = m_pictureSetup->boundingBox();
		vn::aabox2f dummyBox;
		dummyBox.min_corner = box.min_corner;
		dummyBox.min_corner += (point - vn::vector2f(25, 25));
		dummyBox.max_corner = dummyBox.min_corner + vn::vector2f(50, 50);
		m_dummy->setLocation(dummyBox);
	}
	else {
		m_currentDummyIndex = -1;
		vn::aabox2f box = m_pictureSetup->boundingBox();
		vn::aabox2f dummyBox;
		dummyBox.min_corner = box.min_corner - vn::vector2f(25, 25);
		dummyBox.max_corner = dummyBox.min_corner + vn::vector2f(50, 50);
		m_dummy->setLocation(dummyBox);

		m_parametersList->item(5)->setText("");
		m_parametersList->item(6)->setText("");
		m_valueList->item(5)->setText("");
		m_valueList->item(6)->setText("");
	}


}
void EditorApp::_onDummyClicked(vn::UIElement* sender, vn::UIListItem* item) {
	if (m_bPlaying)
	{
		return;
	}

	if (item) {
		if (m_click == true)
		{
			vn::aabox2f box = item->root()->boundingBox();
			m_dummyBkg->setVisible(true);
			m_dummyBkg->setLocation(box);
			vn::str8 str;
			item->getText(str);
			m_dummyTextFiled->setText(str);
			m_dummyTextFiled->selectAll();
			vn::UIRoot::instance().requireFocus(m_dummyTextFiled);
		}
	}
	else {
		m_dummyBkg->setVisible(false);
	}
	m_click = true;

	m_dummyList->setSelectedItem(item);
}

void EditorApp::_onPicSelectedChanged(vn::UIElement* sender, vn::UIListItem* item) {
	
	if (item)
	{
		m_currentFrameIndex = m_picturesList->itemIndex(item);
		m_dummyList->clearItems();
		vn::vector2f point = m_AnimGroup->originalPoint(m_currentAnimIndex, m_currentFrameIndex);
		m_parametersList->item(3)->setText(" X:");
		m_parametersList->item(4)->setText(" Y:");
		char buf[64];
		sprintf(buf, "%g", point.x);
		m_valueList->item(3)->setText(buf);
		sprintf(buf, "%g", point.y);
		m_valueList->item(4)->setText(buf);

		//vn::aabox2f boundingBox = m_axis->boundingBox();
		//m_pictureSetup->setLocation(boundingBox);
		m_pictureSetup->setOriginalPoint(point);
		std::vector<FrameEvent> event = m_AnimGroup->FramesEvent(m_currentAnimIndex)[m_currentFrameIndex].Events;
		for (int i = 0; i < event.size(); ++i) {
			m_dummyList->addItem()->setText(event[i].name);
		}
	}
	else {
		if (m_currentAnimIndex != -1) {
			m_AnimGroup->resetCurrentIndex(m_currentAnimIndex);
		}
		m_dummyList->clearItems();
		m_currentFrameIndex = -1;
		m_currentDummyIndex = -1;
		m_dummyBkg->setVisible(false);
		m_parametersList->item(3)->setText("");
		m_parametersList->item(4)->setText("");
		m_valueList->item(3)->setText("");
		m_valueList->item(4)->setText("");
		vn::aabox2f boundingBox = m_axis->boundingBox();
		vn::vector2f min = boundingBox.min_corner;
		min += vn::vector2f(4, 4);
		vn::vector2f max = min + vn::vector2f(200, 200);
		static_cast<vn::UIRenderPicture1 *>(m_pictureSetup->getPicture().ptr())->setPicture(vn::PicturePtr::kNull);
		m_pictureSetup->setLocation(vn::aabox2f(min, max));
		m_pictureSetup->setColor(0xFFAAAAAA);
	}
}
void EditorApp::_onPicItemClicked(vn::UIElement* sender, vn::UIListItem* item) {

	if (m_bPlaying)
	{
		return;
	}
	m_picturesList->setSelectedItem(item);
}

void EditorApp::_onParSelectedChanged(vn::UIElement* sender, vn::UIListItem* item){

}
void EditorApp::_onParItemClicked(vn::UIElement* sender, vn::UIListItem* item) {
	m_parametersList->setSelectedItem(item);
	if (item) {
		m_currentItemIndex = m_parametersList->itemIndex(item);
		m_valueList->setSelectedItem(m_valueList->item(m_currentItemIndex));
	}else
	{
		m_valueList->setSelectedItem(0);
	}
	
}

void EditorApp::_onValSelectedChanged(vn::UIElement* sender, vn::UIListItem* item) {
	m_valueBkg->setVisible(false);
}
void EditorApp::_onValItemClicked(vn::UIElement* sender, vn::UIListItem* item) {

	m_valueList->setSelectedItem(item);
	
	if (item) {
		m_currentItemIndex = m_valueList->itemIndex(item);
		m_parametersList->setSelectedItem(m_parametersList->item(m_currentItemIndex));
		if (m_click == true)
		{
			vn::aabox2f box = item->root()->boundingBox();
			m_valueBkg->setVisible(true);
			m_valueBkg->setLocation(box);
			vn::str8 str;
			item->getText(str);
			m_valueTextFiled->setText(str);
			m_valueTextFiled->selectAll();
			vn::UIRoot::instance().requireFocus(m_valueTextFiled);
		}
	}
	else {
		m_valueBkg->setVisible(false);
	}
	m_click = true;


}


void EditorApp::_onCheckBoxSelectedChanged(vn::UIElement* sender, vn::UIListItem* item) {
	if (item)
	{
		UIListItemCheckBox* tmp = dynamic_cast<UIListItemCheckBox*>(item);
		if (tmp) {
			vn::str8 name;
			vn::str8 fullPath;
			tmp->getText(name);
			m_imagePath.getFullPath(fullPath);
			fullPath = fullPath + '|' + name;
			vn::PicturePtr pic = vn::PictureManager::instance().createPicture(fullPath.c_str());
			vn::UIRenderPicture1* renderPic = vnnew vn::UIRenderPicture1();
			renderPic->setPicture(pic);
			vn::UIRenderPicturePtr p = renderPic;
			vn::aabox2f boundingBox = m_preview->boundingBox();
			boundingBox.max_corner = boundingBox.min_corner + pic.ptr()->size;
			m_preview->setLocation(boundingBox);

			m_preview->setPicture(p);
			m_preview->setColor(vn::color_t::kWhite);		

			//static_cast<vn::UIRenderPicture1 *>(m_preview->getPicture().ptr())->setPicture();
		}

	}
	else
	{
		m_preview->setColor(vn::color_t::kBlack);
		vn::aabox2f box = m_window->boundingBox();
		vn::vector2f min = vn::vector2f(box.min_corner.x + 280, box.min_corner.y + 50);
		vn::vector2f max = min + vn::vector2f(250, 250);
		m_preview->setLocation(vn::aabox2f(min, max));
		static_cast<vn::UIRenderPicture1 *>(m_preview->getPicture().ptr())->setPicture(vn::PicturePtr::kNull);
	}
}
void EditorApp::_onCheckBoxItemClicked(vn::UIElement* sender, vn::UIListItem* item) {
	m_checkBoxList->setSelectedItem(item);
}

void EditorApp::_onFileOpen(const vn::FilePath &path, const vn::str8 &name) {
	VN_LOG_DEBUG("path: \"" << path.fsId << ":/" << path.fileName << "\"");
	VN_LOG_DEBUG("name: \"" << name << "\"");
	if (name.empty())
	{
		VN_LOG_DEBUG("error pith");
		return;
	}
	

	m_imagePath = path;
	m_imagePath.fileName += name;
	
	std::list<vn::str8> result;
	vn::PictureManager::instance().list(m_imagePath, result);
	for (std::list<vn::str8>::iterator it = result.begin(); it != result.end(); ++it) {
		m_checkBoxList->addItem()->setText(*it);
	}
	m_window->setVisible(true);
}


void EditorApp::_onSave(const vn::FilePath &path, const vn::str8 &name) {
	VN_LOG_DEBUG("path: \"" << path.fsId << ":/" << path.fileName << "\"");
	VN_LOG_DEBUG("name: \"" << name << "\"");

	//
	vn::FilePath tmpPath = path;
	tmpPath.fileName += name;

	vn::FileStream*	file = vn::FileManager::instance().create(tmpPath);
	if (!file)
	{
		VN_LOG_DEBUG("FileStream Create Success");
		return;
	}
	m_AnimGroup->saveFileToPath(*file);
	file->close();
}
void EditorApp::_onLoad(const vn::FilePath &path, const vn::str8 &name) {
	VN_LOG_DEBUG("path: \"" << path.fsId << ":/" << path.fileName << "\"");
	VN_LOG_DEBUG("name: \"" << name << "\"");

	vn::FilePath tmpPath = path;
	tmpPath.fileName += name;
	vn::FileStream* file = vn::FileManager::instance().open(tmpPath);
	if (!file) {
		VN_LOG_DEBUG("File open failed");
		return;
	}
	m_currentAnimIndex = -1;
	m_currentFrameIndex = -1;
	static_cast<vn::UIRenderPicture1 *>(m_pictureSetup->getPicture().ptr())->setPicture(vn::PicturePtr::kNull);
	m_AnimGroup->clear();
	m_animationsList->clearItems();
	m_picturesList->clearItems();

	m_AnimGroup->loadAnimFromFile(*file);
	vn::s32 size = m_AnimGroup->getNames().size();
	for (vn::s32 i = 0; i < size; ++i) {
		m_animationsList->addItem()->setText(m_AnimGroup->getNames()[i]);
	}
	m_valueList->item(0)->setText("");
	m_valueList->item(1)->setText("");
	m_valueList->item(2)->setText("");
	m_valueList->item(3)->setText("");
	m_valueList->item(4)->setText("");
	m_valueList->item(5)->setText("");
	m_valueList->item(6)->setText("");
	m_valueList->item(7)->setText("");
	file->close();
}

void EditorApp::_onImport(const vn::FilePath &path, const vn::str8 &name) {
	VN_LOG_DEBUG("path: \"" << path.fsId << ":/" << path.fileName << "\"");
	VN_LOG_DEBUG("name: \"" << name << "\"");
	vn::FilePath tmpPath = path;
	tmpPath.fileName += name;
	vn::TreeDataScript tds;
	vn::TreeDataNode *tdnode = tds.parse(tmpPath);

	if (!tdnode) {
		VN_LOG_DEBUG(tds.error());
		tdnode->drop();
	}
	static_cast<vn::UIRenderPicture1 *>(m_pictureSetup->getPicture().ptr())->setPicture(vn::PicturePtr::kNull);
	m_animationsList->clearItems();
	m_picturesList->clearItems();
	m_AnimGroup->clear();
	m_AnimGroup->importFromTDL(tdnode);
	vn::s32 size = m_AnimGroup->getNames().size();
	for (vn::s32 i = 0; i < size; ++i) {
		m_animationsList->addItem()->setText(m_AnimGroup->getNames()[i]);
	}
	m_valueList->item(0)->setText("");
	m_valueList->item(1)->setText("");
	m_valueList->item(2)->setText("");
	m_valueList->item(3)->setText("");
	m_valueList->item(4)->setText("");
	m_valueList->item(5)->setText("");
	m_valueList->item(6)->setText("");
	m_valueList->item(7)->setText("");
	tdnode->drop();
}

void EditorApp::_onExport(const vn::FilePath &path, const vn::str8 &name) {
	VN_LOG_DEBUG("path: \"" << path.fsId << ":/" << path.fileName << "\"");
	VN_LOG_DEBUG("name: \"" << name << "\"");
	std::ostringstream ossExport;
	m_AnimGroup->exportToTDL(ossExport);

	vn::FilePath tmpPath = path;
	tmpPath.fileName += name;

	vn::FileStream*	file = vn::FileManager::instance().create(tmpPath);
	if (file)
	{
		VN_LOG_DEBUG("FileStream Create Success");
	}
	file->write(ossExport.str().c_str(), ossExport.str().length());
	file->close();
}