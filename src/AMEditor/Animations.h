#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__
#include "vnTypes.h"
#include "vnVector2.h"
#include "vnPicture.h"
#include <vector>
#include <map>
#include "UIPictureSetup.h"
#include "vnFileManager.h"


typedef struct FrameEvent {
	vn::str8 name;
	vn::vector2f point;
}FrameEventDef;

//for save
typedef struct FrameDiscribe {
	vn::str8 path;
	vn::str8 name;
	vn::vector2f originalPoint;
	std::vector<FrameEvent> Events;
}FrameDef;




//for draw
typedef struct PictureDiscribe {
	vn::Picture* pic;
	vn::vector2f originalPoint;
}PictureDef;

 

class Animation {
public:
	Animation();
	~Animation();

	void addImage(const vn::str8& path, const vn::str8 &name, const vn::vector2f& originalPoint);
	void deleteFrame(const vn::u8& frameIndex);

	void setOriginalPoint(const vn::u8& frameIndex, vn::vector2f point);
	void setFps(const vn::f32 &fps);
	const vn::f32& getFps() const;

	void update(const vn::f32& deltaTime);

	void showFrame(const vn::s8& frameIndex);

	const vn::u8& getFrameIndex() const;
	bool isNull();
	void playAnimation();
	void stopAnimation();

	//tool function
	vn::aabox2f getCurrentAabox(const vn::vector2f& originalPoint);
	const vn::vector2f& originalPoint(const vn::s8& frameIndex) const;

	typedef std::vector<vn::str8> nameVector;
	nameVector m_names;

	//update
	void setPictureSetup(UIPictureSetup* UIPicture);
	void setPictureAxis(UIPictureSetup* UIPicture);

	void resetCurrentIndex();

	void saveFileToPath(vn::FileStream& file);
	void loadAnimFormFile(vn::FileStream& file);
	//add by ts 2014/3/13
	bool exportToTDL(std::ostringstream &oss);
	void importFromTDL(const vn::TreeDataNode *node);

	//add by ts 2014/1/20
	void deleteDummy(const vn::s32& frameIndex, const vn::s32& dummyIndex);
	void setDummy(const vn::s32& frameIndex, const FrameEvent& dummy);
	void setDummyName(const vn::s32& frameIndex, const vn::s32& dummyIndex, const vn::str8& name);
	void setDummyPoint(const vn::s32& frameIndex, const vn::s32& dummyIndex, const vn::vector2f& point);
	vn::vector2f& DummyPoint(const vn::s32& frameIndex, const vn::s32& dummyIndex);
	const std::vector<FrameDiscribe>& Frames() const;
private:
	vn::u8 m_frameNum;
	bool m_bShowAimation;
	vn::u8 m_frameIndex;
	

	vn::f32 m_fps;
	vn::f32 m_frameGap;
	vn::f32 m_time;
	vn::s8  m_currentIndex;

	vn::Picture* m_currentPicture;
	vn::aabox2f  m_currentAabox;
	vn::aabox2f  m_animBoundingBox;
	vn::aabox2f  m_frameBoundingBox;

	typedef std::vector<FrameDiscribe> framesVector;
	framesVector m_frames;

	typedef std::vector<PictureDiscribe> picVector;
	picVector m_pictures;

	//update
	UIPictureSetup* m_pictureSetup;
	UIPictureSetup* m_pictureAxis;

};




class Animations {
public:
	Animations();
	~Animations();
		
	bool createAnimation();

	void setAnimFps(const vn::u8& animIndex, const vn::f32& fps);
	const vn::f32& getAnimFps(const vn::s8& animIndex) const;

	void addImageToAnim(const vn::u8& animIndex, const vn::str8& path, const vn::str8 &name, const vn::vector2f& originalPoint = vn::vector2f(0, 0));
	void setOrignalPoint(const vn::s8& animIndex, const vn::s8& frameIndex, const vn::vector2f& originalPoint);
	
	void setAnimPoint(const vn::u32& animIndex, const vn::vector2f point);
	const vn::vector2f& getAnimPoint(const vn::s32 animIndex) const;

	void setAnimName(const vn::s32& animIndex, const vn::str8 &name); 

	void deleteImageFromAnim(const vn::s8& animIndex, const vn::s8& frameIndex);
	void deleteAnimtion(const vn::u8& animIndex);
	
	void update(const vn::f32& deltaTime, const vn::s8& animIndex);

	void showFrmae(const vn::s8& animIndex, const vn::s8& frameIndex);
	const vn::vector2f& originalPoint(const vn::s8& animIndex, const vn::s8& frameIndex) const;

	const vn::u8& getFrameIndex(const vn::s8& animIndex) const;
	
	void playAnimation(const vn::s8& animIndex);
	void stopAnimation(const vn::s8& animIndex);

	bool isNull(const vn::s8& animIndex);

	std::vector<Animation*>& getAnimations();
	//update
	void setPictureSetup(UIPictureSetup* UIPicture);
	void setPictureAxis(UIPictureSetup* UIPicture);

	void resetCurrentIndex(const vn::s32& animIndex);
	// load and save
	bool saveFileToPath(vn::FileStream& file);
	void loadAnimFromFile(vn::FileStream& file);
	//add by ts 2014/3/13
	bool exportToTDL(std::ostringstream &oss);
	void importFromTDL(const vn::TreeDataNode *node);

	void clear();
	const std::vector<vn::str8>& getNames() const;

	//add by ts 2014/1/20
	void addDummy(const vn::s32& animIndex, const vn::s32& frameIndex);
	void deleteDummy(const vn::s32& animIndex, const vn::s32& frameIndex, const vn::s32& dummyIndex);
	void setDummyName(const vn::s32& animIndex, const vn::s32& frameIndex, const vn::s32& dummyIndex, vn::str8& name);
	void setDummyPoint(const vn::s32& animIndex, const vn::s32& frameIndex, const vn::s32& dummyIndex, vn::vector2f& point);
	vn::vector2f& DummyPoint(const vn::s32& animIndex, const vn::s32& frameIndex, const vn::s32& dummyIndex);
	const std::vector<FrameDiscribe>& FramesEvent(const vn::s32& animIndex);

	//add by ts 2014/3/3
	void setAnimLayer(vn::s32 animIndex, vn::s32 layer);
	vn::s32 getAnimLayer(vn::s32 animIndex) const;
private:
	typedef	std::vector<Animation*> animationVector;
	animationVector m_animations;

	typedef std::vector<vn::str8> animNameVector;
	animNameVector m_names;

	typedef std::vector<vn::s32> animLayer;
	animLayer m_layers;

	typedef std::vector<vn::vector2f> animPointVector;
	animPointVector m_points;

	UIPictureSetup* m_pictureSetup;
	UIPictureSetup* m_pictureAxis;
	vn::u8 version;
};

#endif /*__ANIMATIONS_H__*/