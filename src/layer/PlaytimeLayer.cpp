#include "PlaytimeLayer.h"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool PlaytimeLayer::init() {
    if (!Popup::init(375.f, 169.f, "GJ_square01.png")) {
        return false;
    }

    this->setTitle("Playtime");
    this->setID("PlaytimeLayer");

    auto bg = CCScale9Sprite::create("square02b_001.png");
    bg->setContentSize({280.f, 54.f});
    bg->setColor(ccColor3B({0, 0, 0}));
    bg->setOpacity(75);
    m_mainLayer->addChildAtPosition(bg, Anchor::Top, {0, -93});

    playtimeTitle = CCLabelBMFont::create("", "bigFont.fnt");
    playtimeTitle->setScale(0.8f);

    textArea = TextArea::create(
        "",
        "chatFont.fnt",
        1,
        200.f,
        {0.5f, 0.5f},
        15.f,
        false
    );

    m_mainLayer->addChildAtPosition(playtimeTitle, Anchor::Top, {0, -48});
    m_mainLayer->addChildAtPosition(textArea, Anchor::Top, {0, -93});

    setPage(0);

    auto infoBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        menu_selector(PlaytimeLayer::onInfo)
    );

    m_buttonMenu->addChildAtPosition(
        infoBtn,
        Anchor::TopRight,
        {-20, -20}
    );

    auto delDataBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("edit_delBtnSmall_001.png"),
        this,
        menu_selector(PlaytimeLayer::onDel)
    );

    m_buttonMenu->addChildAtPosition(
        delDataBtn,
        Anchor::BottomRight,
        {-20, 20}
    );

    auto leftArrowBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(PlaytimeLayer::prevPage)
    );

    m_buttonMenu->addChildAtPosition(
        leftArrowBtn,
        Anchor::Left,
        {24, -7}
    );

    auto rightArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    rightArrow->setFlipX(true);

    auto rightArrowBtn = CCMenuItemSpriteExtra::create(
        rightArrow,
        this,
        menu_selector(PlaytimeLayer::nextPage)
    );

    m_buttonMenu->addChildAtPosition(
        rightArrowBtn,
        Anchor::Right,
        {-24, -7}
    );

    auto okSpr = ButtonSprite::create(
        "OK",
        "goldFont.fnt",
        "GJ_button_01.png",
        0.8f
    );

    auto okBtn = CCMenuItemSpriteExtra::create(
        okSpr,
        this,
        menu_selector(PlaytimeLayer::onClose)
    );

    m_buttonMenu->addChildAtPosition(
        okBtn,
        Anchor::Bottom,
        {0, 25}
    );

    return true;
}

std::string PlaytimeLayer::formattedTime(float time, bool isEdit) {
    std::string ret = "";

    int hour = static_cast<int>(time / 3600);
    time = std::fmod(time, 3600);

    int minutes = static_cast<int>(time / 60);
    time = std::fmod(time, 60);

    int sec = static_cast<int>(time);
    int ms = static_cast<int>((time - sec) * 100);

    std::string playOrEdit = "";

    if (isEdit) {
        playOrEdit = "editing";
    }
    else {
        playOrEdit = "playing";
    }

    if (time <= 0) {
        ret = fmt::format(
            "Start <cg>{}</c> to count your <cy>playtime</c>!",
            playOrEdit
        );
    }
    else {
        if (hour > 0)
            ret = fmt::format(
                "<cr>{:02}</c>:<cy>{:02}</c>:<cg>{:02}</c>.<cl>{:02}</c>",
                hour,
                minutes,
                sec,
                ms
            );
        else if (minutes > 0)
            ret = fmt::format(
                "<cy>{:02}</c>:<cg>{:02}</c>.<cl>{:02}</c>",
                minutes,
                sec,
                ms
            );
        else
            ret = fmt::format(
                "<cg>{}</c>s",
                sec
            );
    }

    return ret;
}

void PlaytimeLayer::onInfo(CCObject* sender) {
    FLAlertLayer::create(
        "Playtime",
        "Displays your <cy>playtime</c> for the <cg>game</c>.\n"
        "Your <cy>playtime</c> is split up between "
        "<cl>in-game time</c> and <co>editor time</c>.",
        "OK"
    )->show();
}

void PlaytimeLayer::onDel(CCObject* sender) {
    std::string ret = fmt::format(
        "Are you <cr>sure</c> you want to delete your <cy>playtime</cy>?\n"
        "Your <cy>playtime</cy> will get <co>lost</co>.\n"
        "<cl>Total In-Game Time: {}</c>s\n"
        "<co>Total Editor Time: {}</c>s",
        Mod::get()->getSavedValue<double>("in-game-playtime"),
        Mod::get()->getSavedValue<double>("editor-playtime")
    );

    createQuickPopup(
        "Playtime",
        ret,
        "Cancel",
        "Yes",
        [](auto, bool btn2) {
            if (btn2) {
                Mod::get()->setSavedValue("editor-playtime", 0.f);
                Mod::get()->setSavedValue("in-game-playtime", 0.f);

                FLAlertLayer::create(
                    "Playtime",
                    "Playtime successfully <cr>cleared</c>.",
                    "OK"
                )->show();
            }
        }
    );
}

void PlaytimeLayer::setPage(int p0) {
    curPage = p0;

    if (p0 == 0) {
        playtimeTitle->setString("Total In-Game Time");

        textArea->setString(
            formattedTime(
                Mod::get()->getSavedValue<double>("in-game-playtime"),
                false
            )
        );
    }
    else if (p0 == 1) {
        playtimeTitle->setString("Total Editor Time");

        textArea->setString(
            formattedTime(
                Mod::get()->getSavedValue<double>("editor-playtime"),
                true
            )
        );
    }
}

void PlaytimeLayer::prevPage(CCObject* sender) {
    curPage--;

    if (curPage < 0)
        curPage = pageCount - 1;

    setPage(curPage);
}

void PlaytimeLayer::nextPage(CCObject* sender) {
    curPage++;

    if (curPage >= pageCount)
        curPage = 0;

    setPage(curPage);
}

PlaytimeLayer* PlaytimeLayer::create() {
    auto ret = new PlaytimeLayer();

    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}
