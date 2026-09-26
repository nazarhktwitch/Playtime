#pragma once

#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class PlaytimeLayer : public geode::Popup {
protected:
    int pageCount = 2; // scuffed
    int curPage = 0; // scuffed

    CCLabelBMFont* playtimeTitle;
    TextArea* textArea;

    std::string formattedTime(float time, bool isEdit);

    bool init();

    void onInfo(CCObject* sender); // Handles info button logic
    void onDel(CCObject* sender); // Handles playtime wipe button logic :3
    void setPage(int p0); // Prev apge logic

public:
    static PlaytimeLayer* create();

    void nextPage(CCObject* sender); // Next page logic
    void prevPage(CCObject* sender); // Prev apge logic
};
