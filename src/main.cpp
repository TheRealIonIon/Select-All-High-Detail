#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyEditorPauseLayer, EditorPauseLayer) {
    
    void onSelectHDM(CCObject* sender) {
        auto editor = LevelEditorLayer::get();
        if (!editor || !editor->m_editorUI) return;

        auto ui = editor->m_editorUI;
        auto objs = CCArray::create();

        for (auto obj : CCArrayExt<GameObject*>(editor->m_objects)) {
            if (obj->m_isHighDetail) {
                objs->addObject(obj);
            }
        }

        if (objs->count() > 0) {
            ui->deselectAll();
            ui->selectObjects(objs, true);
            
            Notification::create(
                fmt::format("Selected {} HD objects", objs->count()),
                NotificationIcon::Info
            )->show();
        } else {
            Notification::create("No HD objects found", NotificationIcon::Error)->show();
        }
    }

    bool init(LevelEditorLayer* lel) {
        if (!EditorPauseLayer::init(lel)) return false;

        if (auto menu = this->getChildByID("actions-menu")) {
            auto spr = ButtonSprite::create(
                "Select All\nHigh Detail",
                38,
                true, 
                "bigFont.fnt",
                "GJ_button_04.png",
                24,
                0.75f
            );

            auto btn = CCMenuItemSpriteExtra::create(
                spr,
                nullptr,
                this,
                menu_selector(MyEditorPauseLayer::onSelectHDM)
            );

            btn->setID("select-all-hdm-btn");

            if (auto buildHelper = menu->getChildByID("build-helper-button")) {
                menu->insertBefore(btn, buildHelper);
            } else {
                menu->addChild(btn);
            }

            menu->updateLayout();
        }

        return true;
    }
};
