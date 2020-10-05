/*
 * widgets.cpp
 *
 *  Created on: Jan 8, 2013
 *      Author: sushil
 */

#include <GraphicsInteractionManager.h>
#include <widgets.h>

#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>

OgreGFX::Widget::Widget(GraphicsInteractionManager *gim, Widget *aParent, Ogre::Vector2 defaultPos, Ogre::Vector2 defaultSize) {

	if (defaultPos == Ogre::Vector2::NEGATIVE_UNIT_X)
		this->pos = Ogre::Vector2(0, 0);
	if (defaultSize == Ogre::Vector2::NEGATIVE_UNIT_X)
		this->size = Ogre::Vector2(100, 30);
	if (!aParent)
		this->parent = 0;

	this->gfx = gim;
	this->parent = aParent;
	this->pos = defaultPos;
	this->size = defaultSize;

}

OgreGFX::Widget::~Widget(){

};

Ogre::Vector2 OgreGFX::Widget::screenPos(){
	if (this->parent) {
		return this->parent->screenPos() + this->pos;
	} else {
		return this->pos;
	}
}

bool OgreGFX::Widget::cursonInMe(const Ogre::Vector2 &pos){
	Ogre::Vector2 myPos = screenPos();
	return (pos.x >= myPos.x && pos.x <= myPos.x + this->size.x && pos.y >= myPos.y && pos.y <= myPos.y + size.y);
}


//--------------------------------------------------------------------------------------------------------------------------------

OgreGFX::UIOutline::UIOutline(GraphicsInteractionManager *gim) : OgreGFX::Widget(gim) {

	this->overlayManagerPtr = Ogre::OverlayManager::getSingletonPtr();
	std::string nextId = gfx->widgetMgr->getNextId("OutlinePanel");
	this->overlay = this->overlayManagerPtr->create("UIOutline");


	Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*> (this->overlayManagerPtr->createOverlayElement("Panel", nextId));


	this->pos = Ogre::Vector2(0.0, 0.0);
	this->size = Ogre::Vector2(gfx->mWindow->getWidth(), gfx->mWindow->getHeight());
	this->size = Ogre::Vector2(1.0, 1.0);
	panel->setPosition(pos.x, pos.y);
	panel->setDimensions(size.x, size.y);
	panel->setMaterialName("ECSLENT/UI");

	this->overlay->add2D(static_cast<Ogre::OverlayContainer *> (panel));
	this->overlay->add2D(panel);
	std::cout << "Added OvelayContainer Panel" << std::endl;

	overlay->show();

}

void OgreGFX::UIOutline::show() {
	this->overlay->show();
}

void OgreGFX::UIOutline::hide() {
	this->overlay->hide();
}

//--------------------------------------------------------------------------------------------------------------------------------
//FEPanel(gim, caption1, aparent, apos, Ogre::Vector2(columnWidths.x + columnWidths.y, columnHeight), materialName)

OgreGFX::FEPanel::FEPanel(GraphicsInteractionManager *gim, std::string name, Widget* iparent,
		Ogre::Vector2 ipos, Ogre::Vector2 isize, std::string imaterial):
		Widget(gim, iparent, ipos, isize) {

		this->material = imaterial;
		this->name = name;
		this->overlayManagerPtr = Ogre::OverlayManager::getSingletonPtr();
		this->panel = static_cast<Ogre::OverlayContainer*>(this->overlayManagerPtr->createOverlayElement("Panel", gfx->widgetMgr->getNextId(name)));
		this->panel->setMetricsMode(Ogre::GMM_PIXELS);
		this->panel->setPosition(pos.x, pos.y);
		//size.y = 0; // or extra space when adding labels to this
		this->panel->setDimensions(size.x, size.y);
		this->panel->setMaterialName(material);
		this->panel->show();

		if (!this->parent){
			std::cout << "Creating panel overlay" << std::endl;
			this->overlayPtr = this->overlayManagerPtr->create(gfx->widgetMgr->getNextId(name));
			this->overlayPtr->add2D(this->panel);
			this->overlayPtr->show();
		}

		this->belowPos = Ogre::Vector2(0.0, 0.0);
		this->rightPos = Ogre::Vector2(0.0, 0.0);
		this->gap      = Ogre::Vector2(0.0, 0.0);
		this->separatorHeight = 1;

};

OgreGFX::FEPanel::~FEPanel(){

}


void OgreGFX::FEPanel::show(){
	if (this->parent){
		this->overlayPtr->show();
	}
	this->panel->show();
	for(std::list<Widget*>::iterator it = this->items.begin(); it != this->items.end(); ++it){
		(*it)->show();
	}
}


void OgreGFX::FEPanel::hide(){
	for(std::list<Widget*>::iterator it = this->items.begin(); it != this->items.end(); ++it){ // do I need this?
		(*it)->hide();
	}
	this->panel->hide();
	if (this->parent){
		this->overlayPtr->hide();
	}
}

Ogre::OverlayElement * OgreGFX::FEPanel::makeSep(){

	Ogre::OverlayElement *sep = this->overlayManagerPtr->createOverlayElement("Panel", gfx->widgetMgr->getNextId("lineSep"));
	sep->setMetricsMode(Ogre::GMM_PIXELS);
	sep->setPosition(0, 0);
	sep->setDimensions(this->pos.x, this->separatorHeight);
	sep->setMaterialName("ECSLENT/line");
	sep->show();
	return sep;
}

void OgreGFX::FEPanel::addSep(int separation){
	Ogre::OverlayElement *sep = makeSep();
	this->separators.push_back(sep);
	sep->setPosition(0, separation);
	sep->setDimensions(this->size.x, this->separatorHeight);
	this->panel->addChild(sep);
}

void OgreGFX::FEPanel::adjustSeparators(){
	for(std::list<Ogre::OverlayElement*>::iterator it = this->separators.begin(); it != this->separators.end(); ++it){
		(*it)->setDimensions(this->size.x, this->separatorHeight);
	}
}

void OgreGFX::FEPanel::posChanged(){
	this->panel->setPosition(this->pos.x, this->pos.y);
	for(std::list<Widget*>::iterator it = this->items.begin(); it != this->items.end(); ++it){ // do I need this?
		(*it)->posChanged();
	}

}

void OgreGFX::FEPanel::addItem(Widget *item, Placement placement){
	this->items.push_back(item);
	if (this->size.x < item->size.x){
		this->size.x = item->size.x;
		adjustSeparators();
	}

	if (placement == Below){
		std::cout << "Adding below: " << std::endl;
		item->pos = this->belowPos;
		item->posChanged();
		this->rightPos = this->belowPos + Ogre::Vector2(item->size.x + this->gap.x, 0.0);
		this->belowPos.y += item->size.y + this->gap.y;
		std::cout << "Below Pos: " << belowPos.x << ", " << belowPos.y << std::endl;
	} else if (placement == Right){
		item->pos = this->rightPos;
		item->posChanged();
		this->rightPos.x += item->size.x + this->gap.x;
	}

	if (this->rightPos.x > this->size.x){
		this->size.x = this->rightPos.x;
	}
	if (this->belowPos.y > this->size.y){
		this->size.y = this->belowPos.y;
	}


	this->panel->addChild(item->getOverlayElementToAdd());
	this->panel->setDimensions(this->size.x, this->size.y);
	addSep(this->size.y);

}

Ogre::OverlayElement * OgreGFX::FEPanel::getOverlayElementToAdd(){
	return static_cast<Ogre::OverlayElement *>(this->panel);
}

//--------------------------------------------------------------------------------------------------------------------------------

OgreGFX::FELabel::FELabel(GraphicsInteractionManager *gim,
		std::string caption, Widget *aparent, Ogre::ColourValue acolor,
		Ogre::Vector2 apos, Ogre::Vector2 asize):
		Widget(gim, aparent, apos, asize) {

	this->overlayManagerPtr = Ogre::OverlayManager::getSingletonPtr();
	this->caption = caption;
	this->color = color;
	this->offset = Ogre::Vector2(5.0, 2.0);
	this->textArea = static_cast<Ogre::TextAreaOverlayElement*>(this->overlayManagerPtr->createOverlayElement("TextArea", gfx->widgetMgr->getNextId(caption)));
	this->textArea->setMetricsMode(Ogre::GMM_PIXELS);
	this->textArea->setCaption(caption);
	//std::cout << "Label Size: " << size.x << ", " << size.y << std::endl;
	this->textArea->setPosition(pos.x + offset.x, pos.y + offset.y);
	this->textArea->setDimensions(size.x, size.y);
	this->textArea->setFontName("SdkTrays/Value");
	this->textArea->setCharHeight(size.y);
	this->textArea->setColourBottom(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));
	this->textArea->setColourTop(color);
	this->textArea->show();
}

OgreGFX::FELabel::~FELabel(){

}

void OgreGFX::FELabel::setCaption(std::string val){
	this->caption = val;
	this->textArea->setCaption(val);
}


void OgreGFX::FELabel::show(){
	std::cout << "Showing label" << std::endl;
	this->textArea->show();
}

void OgreGFX::FELabel::hide(){
	this->textArea->hide();
}

Ogre::OverlayElement* OgreGFX::FELabel::getOverlayElementToAdd(){
	return static_cast<Ogre::OverlayElement*>(this->textArea);
}

void OgreGFX::FELabel::posChanged(){
	this->textArea->setPosition(pos.x, pos.y);
}

//--------------------------------------------------------------------------------------------------------------------------------

OgreGFX::FELabelPair::FELabelPair(GraphicsInteractionManager *gim, std::string caption1, std::string caption2, Widget *aparent,
		Ogre::ColourValue color, Ogre::Vector2 apos, Ogre::Vector2 columnWidths, int columnHeight, std::string materialName):
		OgreGFX::FEPanel(gim, caption1, aparent, apos, Ogre::Vector2(columnWidths.x + columnWidths.y, columnHeight), materialName) {

	this->left = new OgreGFX::FELabel(gim, caption1, this, color, apos, Ogre::Vector2(columnWidths.x, columnHeight));
	this->right = new OgreGFX::FELabel(gim, caption2, this, color, Ogre::Vector2(apos.x+columnWidths.x, apos.y), Ogre::Vector2(columnWidths.y, columnHeight));
	this->addItem(this->left, Below);
	this->addItem(this->right, Right);

}

void OgreGFX::FELabelPair::setLeft(std::string val){
	this->left->setCaption(val);
}


void OgreGFX::FELabelPair::setRight(std::string val){

	this->right->setCaption(val);
}

void OgreGFX::FELabelPair::show(){
	std::cout << "Showing labelpair" << std::endl;
	this->panel->show();
	this->left->show();
	this->right->show();
}

void OgreGFX::FELabelPair::hide(){
	this->panel->hide();
	this->left->hide();
	this->right->hide();

}

