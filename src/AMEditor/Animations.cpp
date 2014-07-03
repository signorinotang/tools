#include "Animations.h"
#include "vnRender2DInterface.h"
#include "vnUIRoot.h"
#include "vnPictureManager.h"
#include "vnLog.h"
#include <sstream>

Animation::Animation()
: m_currentIndex(-1)
, m_frameIndex(0)
, m_time(0)
, m_fps(12.f)
, m_bShowAimation(false)
, m_frameNum(0)
, m_frameGap(0)
, m_pictureSetup(0)
{
	m_frameGap = 1 / m_fps;
}

Animation::~Animation() {
	m_names.clear();
	m_frames.clear();
	for (picVector::iterator it = m_pictures.begin(); it != m_pictures.end(); ++it)
	{
		VN_SAFE_DELETE((*it).pic);
	}
	m_pictures.clear();
}


void Animation::addImage(const vn::str8& path, const vn::str8 &name, const vn::vector2f& originalPoint) {

	vn::str8 fullPath = path + '|' + name;

	vn::Picture* pic = vn::PictureManager::instance().createPicture(fullPath.c_str());

	if (!pic)
	{
		printf("Animation addImage create picture filed");
		return;
	}
	
	FrameDiscribe frame;
	frame.originalPoint = originalPoint;
	frame.path = path;
	frame.name = name;
	m_frames.push_back(frame);


	PictureDiscribe picDis;
	picDis.originalPoint = originalPoint;
	picDis.pic = pic;
	m_pictures.push_back(picDis);

	m_names.push_back(name);
}

void Animation::deleteFrame(const vn::u8& frameIndex)
{
	m_names.erase(m_names.begin() + frameIndex);
	m_frames.erase(m_frames.begin() + frameIndex);
	VN_SAFE_DELETE((m_pictures.begin() + frameIndex)->pic)
	m_pictures.erase(m_pictures.begin() + frameIndex);
	m_currentIndex = -1;
}


void Animation::setOriginalPoint(const vn::u8& frameIndex, vn::vector2f point) {
	m_frames[frameIndex].originalPoint = point;
	m_pictures[frameIndex].originalPoint = point;
	m_animBoundingBox = m_pictureAxis->boundingBox();
	m_currentAabox = m_animBoundingBox;
	m_currentAabox.min_corner += m_pictures[frameIndex].originalPoint;
	m_currentAabox.max_corner = m_currentAabox.min_corner + m_currentPicture->size;
	m_pictureSetup->setLocation(m_currentAabox);
}

void Animation::setFps(const vn::f32 &fps)
{
	m_fps = fps;
	m_frameGap = 1 / m_fps;
}

const vn::f32& Animation::getFps() const {

	return m_fps;
}

//update
void Animation::update(const vn::f32& deltaTime) {

	if (m_bShowAimation)
	{
		m_time += deltaTime;
		if (m_time > m_frameGap)
		{ 
			vn::u32 index = (vn::u32)floor(m_time / m_frameGap);
			m_frameIndex += index;
			m_time = 0;
			if (m_frameIndex >= m_frameNum)
			{
				m_frameIndex = 0;
			}
		}
	}
}

const vn::u8& Animation::getFrameIndex() const
{
	return m_frameIndex;
}



void Animation::showFrame(const vn::s8& frameIndex) {
	
	if (m_currentIndex != frameIndex)
	{
		m_currentIndex = frameIndex;
		if (m_currentIndex == -1) {
			return;
		}
		m_currentPicture = m_pictures[frameIndex].pic;
		if (!m_currentPicture) {
			return;
		}
		m_pictureSetup->setColor(0xFFFFFFFF);
		m_animBoundingBox = m_pictureAxis->boundingBox();
		m_currentAabox = m_animBoundingBox;
		m_currentAabox.min_corner += m_pictures[frameIndex].originalPoint;
		m_currentAabox.max_corner = m_currentAabox.min_corner + m_currentPicture->size;
		m_pictureSetup->setLocation(m_currentAabox);
		m_currentPicture->grab();
		static_cast<vn::UIRenderPicture1 *>(m_pictureSetup->getPicture().ptr())->setPicture(m_currentPicture);
	}
}

void Animation::resetCurrentIndex() {
	m_currentIndex = -1;
}


vn::aabox2f Animation::getCurrentAabox(const vn::vector2f& originalPoint) {
	//vn::vector2f point = vn::UIRoot::instance().getViewSize();
	//point *= 0.5f;
	//point -= originalPoint;
	//vn::aabox2f box = vn::aabox2f(point, point + m_currentPicture->size);
	return vn::aabox2f(0,0,0,0);
}

void Animation::playAnimation()
{
	m_frameNum = m_pictures.size();
	m_bShowAimation = true;
}

void Animation::stopAnimation()
{
	m_bShowAimation = false;
	m_time = 0;
	m_frameIndex = 0;
}


bool Animation::isNull()
{
	return m_names.empty();
}

const vn::vector2f& Animation::originalPoint(const vn::s8& frameIndex) const {
	return m_pictures[frameIndex].originalPoint;
}

void Animation::setPictureSetup(UIPictureSetup* UIPicture) {
	m_pictureSetup = UIPicture;	
}

void Animation::setPictureAxis(UIPictureSetup* UIPicture) {
	m_pictureAxis = UIPicture;
}

void Animation::saveFileToPath(vn::FileStream& file) {
	vn::u32 size = m_frames.size();

	file << vn::DataStream::z_u32(size);
	file << m_fps;
	for (vn::u32 i = 0; i < size; ++i) {
		file << m_frames[i].name;
		file << m_frames[i].path;
		file << m_frames[i].originalPoint.x;
		file << m_frames[i].originalPoint.y;

		vn::u32 dummySize = m_frames[i].Events.size();
		file << vn::DataStream::z_u32(dummySize);
		for (vn::u32 j = 0; j < dummySize; ++j) {
			file <<	m_frames[i].Events[j].name;
			file << m_frames[i].Events[j].point.x;
			file << m_frames[i].Events[j].point.y;
		}
	}
}

void Animation::loadAnimFormFile(vn::FileStream& file) {
	vn::DataStream::z_u32 size;
	vn::str8 fullPath;
	file >> size;
	file >> m_fps;
	setFps(m_fps);
	for (vn::u32 i = 0; i < size.value; ++i) {
		FrameDiscribe frame;
		file >> frame.name;
		file >> frame.path;
		file >> frame.originalPoint.x;
		file >> frame.originalPoint.y;

		vn::DataStream::z_u32 dummySize;
		file >> dummySize;
		for (int j = 0; j < dummySize.value; ++j) {
			FrameEvent event;
			file >> event.name;
			file >> event.point.x;
			file >> event.point.y;
			frame.Events.push_back(event);
		}
		m_frames.push_back(frame);	

		fullPath = frame.path + '|' + frame.name;
		vn::Picture* pic = vn::PictureManager::instance().createPicture(fullPath.c_str());
		
		PictureDiscribe picDis;
		picDis.originalPoint = frame.originalPoint;
		picDis.pic = pic;
		m_pictures.push_back(picDis);

		m_names.push_back(frame.name);
	}
	return;
}

bool Animation::exportToTDL(std::ostringstream &oss) {
	vn::u32 size = m_frames.size();
	oss << "\t\tfps = " << m_fps << "\n";
	oss << "\t}\n";
	for (int i = 0; i < size; ++i) {
		oss << "\t\t\t@<frame {\n";
		oss << "\t\t\t\tname = \"" + m_frames[i].name + "\"\n"; 
		oss << "\t\t\t\timage = \"" + m_frames[i].path + "\"\n";
		oss << "\t\t\t\tpointX = " << m_frames[i].originalPoint.x << "\n";
		oss << "\t\t\t\tpointY = " << m_frames[i].originalPoint.y << "\n";
		oss << "\t\t\t\tdummies = [\n";
		vn::u32 dummySize = m_frames[i].Events.size();
		for (int j = 0; j < dummySize; ++j) {
			oss << "\t\t\t\t\t\t\t\"" + m_frames[i].Events[j].name + "\" " << m_frames[i].Events[j].point.x << " " << m_frames[i].Events[j].point.y << "\n";
		}
		oss << "\t\t\t\t          ]\n";
		oss << "\t\t\t}\n";
		oss << "\t\t\t@>frame\n";
	}
	oss << "\t@>anim\n\n";
	return true;
}

void Animation::importFromTDL(const vn::TreeDataNode *node) {
	vn::str8 fullPath;
	for (const vn::TreeDataNode *frame = node->firstChild(); frame; frame = frame->nextNeighbour()) {
		FrameDiscribe framedes;
		framedes.name = frame->queryAttributeString("name")->value();
		framedes.path = frame->queryAttributeString("image")->value();
		framedes.originalPoint.x = frame->queryAttributeInteger("pointX");
		framedes.originalPoint.y = frame->queryAttributeInteger("pointY");
		const vn::TreeDataArray *array = frame->queryAttributeArray("dummies");
		size_t size = array->size();
		for (int i = 0; i < size; ++i) {
			FrameEvent event;
			event.name = ((vn::TreeDataString*)(array->at(i++)))->value();
			event.point.x = ((vn::TreeDataInteger*)(array->at(i++)))->value();
			event.point.y = ((vn::TreeDataInteger*)(array->at(i)))->value();
			framedes.Events.push_back(event);
		}
		m_frames.push_back(framedes);

		fullPath = framedes.path + '|' + framedes.name;
		vn::Picture* pic = vn::PictureManager::instance().createPicture(fullPath.c_str());

		PictureDiscribe picDis;
		picDis.originalPoint = framedes.originalPoint;
		picDis.pic = pic;
		m_pictures.push_back(picDis);

		m_names.push_back(framedes.name);
	}
}

void Animation::setDummy(const vn::s32& frameIndex, const FrameEvent& dummy) {
	m_frames[frameIndex].Events.push_back(dummy);
}

void Animation::setDummyName(const vn::s32& frameIndex, const vn::s32& dummyIndex, const vn::str8& name) {
	m_frames[frameIndex].Events[dummyIndex].name = name;
}

void Animation::setDummyPoint(const vn::s32& frameIndex, const vn::s32& dummyIndex, const vn::vector2f& point) {
	m_frames[frameIndex].Events[dummyIndex].point = point;
}

vn::vector2f& Animation::DummyPoint(const vn::s32& frameIndex, const vn::s32& dummyIndex) {
	return m_frames[frameIndex].Events[dummyIndex].point;
}

const std::vector<FrameDiscribe>& Animation::Frames() const {
	return m_frames;
}

void Animation::deleteDummy(const vn::s32& frameIndex, const vn::s32& dummyIndex){
	m_frames[frameIndex].Events.erase((m_frames[frameIndex].Events).begin() + dummyIndex);
}


//******************************animations******************************
Animations::Animations()
:version(0)
{

}

Animations::~Animations()
{
	for (animationVector::iterator it = m_animations.begin(); it != m_animations.end(); ++it)
	{
		VN_SAFE_DELETE(*it)
	}
}

bool Animations::createAnimation()
{
	Animation* anim = new Animation();
	if (anim)
	{
		anim->setPictureSetup(m_pictureSetup);
		anim->setPictureAxis(m_pictureAxis);
		m_animations.push_back(anim);
		m_names.push_back("animations");
		m_points.push_back(vn::vector2f(0, 0));
		m_layers.push_back(0);
		return true;
	}
	return false;
}

void Animations::deleteAnimtion(const vn::u8& animIndex)
{
	delete m_animations[animIndex];
	m_animations.erase(m_animations.begin() + animIndex);
	m_names.erase(m_names.begin() + animIndex);
	m_points.erase(m_points.begin() + animIndex);
	m_layers.erase(m_layers.begin() + animIndex);
}

void Animations::addImageToAnim(const vn::u8& animIndex, const vn::str8& path, const vn::str8 &name, const vn::vector2f& originalPoint) {
	m_animations[animIndex]->addImage(path, name, originalPoint);
}

void Animations::deleteImageFromAnim(const vn::s8& animIndex, const vn::s8& frameIndex) {
	m_animations[animIndex]->deleteFrame(frameIndex);
}

void Animations::setAnimFps(const vn::u8& animIndex, const vn::f32& fps)
{
	if (animIndex == -1)	{
		return;
	}
	m_animations[animIndex]->setFps(fps);
}


void Animations::update(const vn::f32& deltaTime, const vn::s8& animIndex) {
	if (animIndex == -1)
	{
		return;
	}
	m_animations[animIndex]->update(deltaTime);
}

void Animations::showFrmae(const vn::s8& animIndex, const vn::s8& frameIndex) {
	if (animIndex == -1 || frameIndex == -1)
	{
		return;
	}
	m_animations[animIndex]->showFrame(frameIndex);
}

const vn::u8& Animations::getFrameIndex(const vn::s8& animIndex) const
{	
	return m_animations[animIndex]->getFrameIndex();
}


void Animations::playAnimation(const vn::s8& animIndex)
{
	if (animIndex == -1) {
		return;
	}
	m_animations[animIndex]->playAnimation();
}

void Animations::stopAnimation(const vn::s8& animIndex)
{
	if (animIndex == -1) {
		return;
	}
	m_animations[animIndex]->stopAnimation();
}


std::vector<Animation*>& Animations::getAnimations()
{
	return m_animations;
}


bool Animations::isNull(const vn::s8& animIndex)
{
	return m_animations[animIndex]->isNull();
}

const vn::f32& Animations::getAnimFps(const vn::s8& animIndex) const
{
	return m_animations[animIndex]->getFps();
}


const vn::vector2f& Animations::originalPoint(const vn::s8& animIndex, const vn::s8& frameIndex) const{
	static vn::vector2f pos;
	if (animIndex == -1 || frameIndex == -1)
	{
		return pos;
	}
	return m_animations[animIndex]->originalPoint(frameIndex);
}


void Animations::setOrignalPoint(const vn::s8& animIndex, const vn::s8& frameIndex, const vn::vector2f& originalPoint)
{
	m_animations[animIndex]->setOriginalPoint(frameIndex, originalPoint);
}

void Animations::setPictureSetup(UIPictureSetup* UIPicture) {
	m_pictureSetup = UIPicture;
}

void Animations::setPictureAxis(UIPictureSetup* UIPicture) {
	m_pictureAxis = UIPicture;
}

void Animations::setAnimPoint(const vn::u32& animIndex, const vn::vector2f point) {
	m_points[animIndex] = point;
}

const vn::vector2f& Animations::getAnimPoint(const vn::s32 animIndex) const {
	return m_points[animIndex];
}


void Animations::setAnimName(const vn::s32& animIndex, const vn::str8 &name) {
	m_names[animIndex] = name;
}


void Animations::resetCurrentIndex(const vn::s32& animIndex) {
	if (animIndex == -1) {
		return;
	}
	m_animations[animIndex]->resetCurrentIndex();
}


bool Animations::saveFileToPath(vn::FileStream& file) {

	vn::u32 size = m_animations.size();
	if (size == 0)
	{
		return false;
	}
	file << version;
	file << vn::DataStream::z_u32(size);
	
	for (vn::u32 i = 0; i < size; ++i) {

		file << m_names[i];
		file << m_points[i].x;
		file << m_points[i].y;
		file << m_layers[i];
		m_animations[i]->saveFileToPath(file);
	}
	return true;
}

void Animations::loadAnimFromFile(vn::FileStream& file) {
	vn::DataStream::z_u32 size;
	file >> version;
	file >> size;

	vn::str8 name;
	vn::vector2f point;
	vn::s32 layer;

	for (vn::u32 i = 0; i < size.value; ++i) {
		file >> name;
		file >> point.x;
		file >> point.y;
		file >> layer;
		m_names.push_back(name);
		m_points.push_back(point);
		m_layers.push_back(layer);
		Animation* anim = new Animation();
		anim->loadAnimFormFile(file);
		anim->setPictureSetup(m_pictureSetup);
		anim->setPictureAxis(m_pictureAxis);
		m_animations.push_back(anim);
	}
}

bool Animations::exportToTDL(std::ostringstream &oss) {
	vn::u32 size = m_animations.size();
	if (!size) {
		return false;
	}
	oss << "@<root\n";
	for (int i = 0; i < size; ++i) {
		oss << "\t@<anim {\n";
		oss << "\t\tname = \"" + m_names[i] <<"\"\n";
		oss << "\t\tpointX = " << m_points[i].x << "\n";
		oss << "\t\tpointY = " << m_points[i].y << "\n";
		oss << "\t\tlayer = " << m_layers[i] << "\n";
		m_animations[i]->exportToTDL(oss);
	}
	oss << "@>root";
	return true;
}

void Animations::importFromTDL(const vn::TreeDataNode *node) {
	vn::str8 name;
	vn::vector2f point;
	vn::s32 layer;
	vn::f32 fps;

	for (const vn::TreeDataNode *anim = node->firstChild(); anim; anim = anim->nextNeighbour()) {
		const vn::TreeDataString * tds = anim->queryAttributeString("name");
		if (!tds) {
			VN_LOG_ERROR("can`t not find the anmi name");
		}
		name = tds->value();
		point.x = anim->queryAttributeInteger("pointX");
		point.y = anim->queryAttributeInteger("pointY");
		layer = anim->queryAttributeInteger("layer");
		fps = anim->queryAttributeFloat("fps");
		m_names.push_back(name);
		m_points.push_back(point);
		m_layers.push_back(layer);
		Animation* animation = new Animation();
		animation->setFps(fps);
		animation->importFromTDL(anim);
		animation->setPictureSetup(m_pictureSetup);
		animation->setPictureAxis(m_pictureAxis);
		m_animations.push_back(animation);
	}

}

void Animations::clear() {
	for (animationVector::iterator it = m_animations.begin(); it != m_animations.end(); ++it)
	{
		VN_SAFE_DELETE(*it)
	}
	m_animations.clear();
	m_names.clear();
	m_points.clear();
}

const std::vector<vn::str8>& Animations::getNames() const {
	return m_names;
}

//add by ts 2014/1/20
void Animations::addDummy(const vn::s32& animIndex, const vn::s32& frameIndex) {
	FrameEventDef dummy;
	dummy.name = "cast";
	dummy.point = vn::vector2f(0, 0);
	m_animations[animIndex]->setDummy(frameIndex, dummy);
}


void Animations::setDummyName(const vn::s32& animIndex, const vn::s32& frameIndex, const vn::s32& dummyIndex, vn::str8& name) {
	m_animations[animIndex]->setDummyName(frameIndex, dummyIndex, name);
}


vn::vector2f& Animations::DummyPoint(const vn::s32& animIndex, const vn::s32& frameIndex, const vn::s32& dummyIndex){
	return m_animations[animIndex]->DummyPoint(frameIndex, dummyIndex);
}


void Animations::setDummyPoint(const vn::s32& animIndex, const vn::s32& frameIndex, const vn::s32& dummyIndex, vn::vector2f& point) {
	m_animations[animIndex]->setDummyPoint(frameIndex, dummyIndex, point);
}


const std::vector<FrameDiscribe>& Animations::FramesEvent(const vn::s32& animIndex) {
	return m_animations[animIndex]->Frames();
}

void Animations::deleteDummy(const vn::s32& animIndex, const vn::s32& frameIndex, const vn::s32& dummyIndex) {
	m_animations[animIndex]->deleteDummy(frameIndex, dummyIndex);
}

void Animations::setAnimLayer(vn::s32 animIndex, vn::s32 layer) {
	m_layers[animIndex] = layer;
}

vn::s32 Animations::getAnimLayer(vn::s32 animIndex) const {
	return m_layers[animIndex];
}