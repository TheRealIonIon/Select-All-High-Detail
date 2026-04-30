#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyEditorPauseLayer, EditorPauseLayer) {
    void onSelectHDM(CCObject*) {
        auto editor = LevelEditorLayer::get();
        if (!editor) return;
        
        auto ui = editor->m_editorUI;
        auto objs = CCArray::create();
        for (auto obj : CCArrayExt<GameObject*>(editor->m_objects)) {
            if (obj->m_isHighDetail) objs->addObject(obj);
        }
        ui->deselectAll();
        ui->selectObjects(objs, true);
    }

    bool init(LevelEditorLayer* l) {
        if (!EditorPauseLayer::init(l)) return false;
        
        if (auto m = this->getChildByID("small-actions-menu")) {
            auto spr = ButtonSprite::create("Select All\nHigh Detail", "bigFont.fnt", "GJ_button_04.png");
            spr->setScale(0.22f);
            
            auto b = CCMenuItemSpriteExtra::create(
                spr, nullptr, this, menu_selector(MyEditorPauseLayer::onSelectHDM)
            );
            b->setID("select-all-hdm-btn");
            
            m->insertBefore(b, nullptr);
            m->updateLayout();
        }
        return true;
    }
};
