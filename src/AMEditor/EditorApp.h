#ifndef __editor__EditorApp__
#define __editor__EditorApp__

#include "../common/GfxAppDelegate.h"
#include "../common/UIFileBrowser.h"
#include "vnUIList.h"
#include "vnUIFrame.h"
#include "vnUITextField.h"
#include "UIListItemCheckBox.h"
#include "vnUIWindow.h"
#include "UIPictureSetup.h"


class Animations;
class EditorApp : public GfxAppDelegate, public vn::Singleton<EditorApp> {
	
public:
	EditorApp();
	~EditorApp();

protected:
	virtual void _init(const vn::vector2f &size);
	virtual void _fini();
	virtual void _update(vn::f32 deltaTime);
	virtual void _render();
	virtual void _resize(const vn::vector2f &size);
	void _loadConfigFile();
	//update
	void updateAnimOriginalPoint(vn::f32 deltaTime);

	//filebrowser event
	void _onFileOpen(const vn::FilePath &path, const vn::str8 &name);
	void _onSave(const vn::FilePath &path, const vn::str8 &name);
	void _onLoad(const vn::FilePath &path, const vn::str8 &name);
	void _onImport(const vn::FilePath &path, const vn::str8 &name);
	void _onExport(const vn::FilePath &path, const vn::str8 &name);
	//button event
	void _onString(vn::UIElement *, const vn::str32 &);
	void _onStringParameters(vn::UIElement *, const vn::str32 &);
	void _onStringDummy(vn::UIElement*, const vn::str32&);

	void _onClicked(vn::UIElement *);
	void _onWindowButtonClicked(vn::UIElement*);
	//animationslist event
	void _onAnimSelectedChanged(vn::UIElement*, vn::UIListItem*);
	void _onAnimItemClicked(vn::UIElement*, vn::UIListItem*);
	//picturesList event
	void _onPicSelectedChanged(vn::UIElement*, vn::UIListItem*);
	void _onPicItemClicked(vn::UIElement*, vn::UIListItem*);
	//parametersList event
	void _onParSelectedChanged(vn::UIElement*, vn::UIListItem*);
	void _onParItemClicked(vn::UIElement*, vn::UIListItem*);
	//valueList event
	void _onValSelectedChanged(vn::UIElement*, vn::UIListItem*);
	void _onValItemClicked(vn::UIElement*, vn::UIListItem*);
	//checkboxlist event
	void _onCheckBoxSelectedChanged(vn::UIElement*, vn::UIListItem*);
	void _onCheckBoxItemClicked(vn::UIElement*, vn::UIListItem*);
	//dummyList event
	void _onDummySelectedChanged(vn::UIElement*, vn::UIListItem*); 
	void _onDummyClicked(vn::UIElement*, vn::UIListItem*);

	//picture
	UIPictureSetup* m_pictureSetup;
	UIPictureSetup* m_axis;
	UIPictureSetup* m_dummy;

	//uiwindow
	vn::UIWindow* m_window;
	//
	UIFileBrowser *m_fileBrowser;
	UIFileBrowser *m_saveFileBrowser;
	UIFileBrowser *m_loadFileBrowser;
	UIFileBrowser *m_importBorwser;
	UIFileBrowser *m_exportBorwser;
	//textFiled
	vn::UITextField* m_textFiled;
	vn::UITextField* m_valueTextFiled;
	vn::UITextField* m_dummyTextFiled;

	vn::UIFrame* m_bkg;
	vn::UIFrame* m_valueBkg;
	vn::UIFrame* m_dummyBkg;
	//checkboxlist
	vn::UIList* m_checkBoxList;
	//list
	vn::UIList* m_animationsList;
	vn::UIList* m_picturesList;
	vn::UIList* m_dummyList;
	//pair
	vn::UIList* m_parametersList;
	vn::UIList* m_valueList;
	//animations
	Animations *m_AnimGroup;
	//path
	vn::FilePath m_imagePath;
	//picture preview
	vn::UIPicture* m_preview;

	//preview boundingBox
	vn::aabox2f m_previewBox;
	

	//index
	vn::s8 m_currentAnimIndex;
	vn::s8 m_currentFrameIndex;
	vn::s8 m_currentItemIndex;
	vn::s8 m_currentDummyIndex;
	//bool
	bool m_bPlaying;
	bool m_click;
	bool m_doubleClick;
	vn::f32 m_timer;
};



#endif /*__editor__EitorApp__*/

