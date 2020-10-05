/*
 * widgets.h
 *
 *  Created on: Jan 8, 2013
 *      Author: sushil
 */

#ifndef WIDGETS_H_
#define WIDGETS_H_

#include <OgreVector2.h>
#include <OgreColourValue.h>
#include <Overlay/OgreOverlayManager.h>
#include <Overlay/OgreOverlayContainer.h>
#include <Overlay/OgreOverlayElement.h>
#include <Overlay/OgreTextAreaOverlayElement.h>



namespace OgreGFX {

	static std::string DEFAULT_PANEL_MATERIAL = "ECSLENT/navy/material/shipInfo/overlay";
	static Ogre::Vector2 DEFAULT_LABEL_SIZE = Ogre::Vector2(100, 20);

	enum Placement {
		Below = 0,
		Right = 1
	};

	class GraphicsInteractionManager;

	class Widget {

	public:
		Ogre::Vector2 pos;
		Ogre::Vector2 size;
		Widget *parent;
		GraphicsInteractionManager *gfx;

		Widget(GraphicsInteractionManager *gim, Widget *aParent = 0, Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X, Ogre::Vector2 defaultSize = Ogre::Vector2::NEGATIVE_UNIT_X );
		virtual ~Widget();

		Ogre::Vector2 screenPos();
		bool cursonInMe(const Ogre::Vector2 &pos);

		virtual void posChanged(){};
		virtual void show(){};
		virtual void hide(){};
		virtual Ogre::OverlayElement* getOverlayElementToAdd(){return 0;};

	};


	class UIOutline : public Widget {

	public:

		Ogre::OverlayManager *overlayManagerPtr;
		Ogre::Overlay* overlay;
		virtual void show();
		virtual void hide();

		UIOutline(GraphicsInteractionManager *gim);
		virtual ~UIOutline(){}

	};

	class FEPanel : public Widget {
	private:

	public:
		FEPanel(GraphicsInteractionManager *gim, std::string name = "FEPanel", Widget* aParent = 0,
				Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X, Ogre::Vector2 defaultSize = Ogre::Vector2::NEGATIVE_UNIT_X,
				std::string material = DEFAULT_PANEL_MATERIAL);//: Widget(gim) {};
		virtual ~FEPanel();

		Ogre::Overlay* overlayPtr;
		Ogre::OverlayManager* overlayManagerPtr;
		Ogre::OverlayContainer *panel;
		Ogre::TextAreaOverlayElement *textArea;
		std::string material;
		std::string name;
		Ogre::Vector2 belowPos;
		Ogre::Vector2 rightPos;
		Ogre::Vector2 gap;
		std::list<Widget*> items;
		std::list<Ogre::OverlayElement*> separators;
		int separatorHeight;


		virtual void show();
		virtual void hide();
		virtual void posChanged();

		void addItem(Widget *item, Placement placement = Below);

		//		void getItems();
//		void getOverlayElementToAdd();
		void addSep(int separation);
		void adjustSeparators();
		Ogre::OverlayElement* makeSep();
		virtual Ogre::OverlayElement* getOverlayElementToAdd();

	};


	class FELabel : public Widget {
	public:
		FELabel(GraphicsInteractionManager *gim, std::string caption = "FELabel",
				Widget *aParent = 0, Ogre::ColourValue color = Ogre::ColourValue::Red,
				Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X, Ogre::Vector2 defaultSize = DEFAULT_LABEL_SIZE);
		virtual ~FELabel();

		Ogre::OverlayManager* overlayManagerPtr;
		std::string caption;
		Ogre::ColourValue color;
		Ogre::Vector2 offset;
		Ogre::TextAreaOverlayElement* textArea;

		void setCaption(std::string val);

		virtual void show();
		virtual void hide();
		virtual void posChanged();
		virtual Ogre::OverlayElement* getOverlayElementToAdd();

	};

	class FELabelPair : public FEPanel {
	public:
		FELabelPair(GraphicsInteractionManager *gim, std::string caption1 = "Key", std::string caption2 = "Value",
				Widget *aParent = 0, Ogre::ColourValue color = Ogre::ColourValue::Green,
				Ogre::Vector2 defaultPos = Ogre::Vector2::NEGATIVE_UNIT_X,
				Ogre::Vector2 defaultColumnWidths = Ogre::Vector2(100, 200), int columnHeight = DEFAULT_LABEL_SIZE.y,
				std::string materialName = DEFAULT_PANEL_MATERIAL);
		/*dimensions/positions are in pixels */
		virtual ~FELabelPair(){};

		FELabel *left, *right;

		virtual void show();
		virtual void hide();

		void setLeft(std::string val);
		void setRight(std::string val);

	};


}


#endif /* WIDGETS_H_ */
