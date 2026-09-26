#include "layer/PlaytimeLayer.h"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(CircleButtonSprite::createWithSpriteFrameName("GJ_timeIcon_001.png", 0.85f, CircleBaseColor::Green, CircleBaseSize::MediumAlt), this, menu_selector(MyMenuLayer::onPlaytime));

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("playtime-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onPlaytime(CCObject*) {
		PlaytimeLayer::create()->show();
	}
};
