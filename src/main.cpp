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
            if (obj->m_highDetail) objs->addObject(obj);
        }
        ui->deselectAll();
        ui->selectObjects(objs, true);
        
        // resume editor to show selection
        this->onResume(nullptr);
    }

    bool init(LevelEditorLayer* l) {
        if (!EditorPauseLayer::init(l)) return false;
        
        if (auto m = this->getChildByID("small-actions-menu")) {
            auto b = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("cc_2x2_white_image"),
                nullptr, this, menu_selector(MyEditorPauseLayer::onSelectHDM)
            );
            b->setID("select-all-hdm-btn");
            m->addChild(b);
            m->updateLayout();
        }
        return true;
    }
};
