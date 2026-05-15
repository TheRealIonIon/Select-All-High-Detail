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
            if (obj->m_isHighDetail) objs->addObject(obj);
        }

        auto cnt = objs->count();
        std::string str = "object";
        if (cnt > 1) str += "s";

        if (cnt > 0) {
            auto prev = CCArray::create();

            if (ui->m_selectedObjects && ui->m_selectedObjects->count() > 0) {
                prev->addObjectsFromArray(ui->m_selectedObjects);
            } else if (ui->m_selectedObject) prev->addObject(ui->m_selectedObject);

            if (auto undo = UndoObject::create(nullptr, UndoCommand::Select)) {
                if (!undo->m_objects) {
                    undo->m_objects = CCArray::create();
                    undo->m_objects->retain();
                }

                undo->m_objects->addObjectsFromArray(prev);
                editor->m_undoObjects->addObject(undo);
            }

            ui->deselectAll();
            ui->selectObjects(objs, true);
            ui->updateButtons();
            ui->updateDeleteButtons();

            Notification::create(
                fmt::format("Selected {} HD {}", cnt, str),
                NotificationIcon::Success
            )->show();
        } else {
            Notification::create("No HD objects found", NotificationIcon::Error)->show();
        }
    }

    bool init(LevelEditorLayer* lel) {
        if (!EditorPauseLayer::init(lel)) return false;

        if (auto menu = this->getChildByID("actions-menu")) {
            auto spr = ButtonSprite::create("");
            spr->setContentSize({56, 24});
            spr->removeAllChildren();

            auto label = CCLabelBMFont::create(
                "Select All\nHigh Detail",
                "bigFont.fnt"
            );

            label->setScale(0.25f);
            label->setAnchorPoint({0.5f, 0.5f});
            label->setPosition(spr->getContentSize() / 2 + ccp(0.5f, 1.0f));
            spr->addChild(label);

            auto bg = CCScale9Sprite::create("GJ_button_04.png");
            bg->setContentSize({66, 30});
            bg->setAnchorPoint({0.5f, 0.5f});
            bg->setScale(0.81f);
            bg->setPosition(spr->getContentSize() / 2);
            spr->addChild(bg, -1);

            auto btn = CCMenuItemSpriteExtra::create(
                spr,
                nullptr,
                this,
                menu_selector(MyEditorPauseLayer::onSelectHDM)
            );

            auto build = menu->getChildByID("build-helper-button");
            btn->setID("select-all-hdm-btn");
            menu->insertBefore(btn, build);
            menu->updateLayout();
        }

        return true;
    }
};
