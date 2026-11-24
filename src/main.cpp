#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorCommandsPopup.hpp>

using namespace geode::prelude;

class $modify(CustomEditorLayer, LevelEditorLayer) {
    void selectHD() {
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;
        if (!m_editorUI || !m_objects) return;

        auto ui = m_editorUI;
        auto objs = CCArray::create();

        for (auto obj : CCArrayExt<GameObject*>(m_objects)) {
            if (obj->m_isHighDetail) objs->addObject(obj);
        }

        auto cnt = objs->count();
        std::string str = "object";
        if (cnt > 1) str.push_back('s');

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
                m_undoObjects->addObject(undo);
            }

            ui->deselectAll();
            ui->selectObjects(objs, true);
            ui->updateButtons();
            ui->updateDeleteButtons();

            auto text = fmt::format("Selected {} HD {}", cnt, str);
            Notification::create(text, NotificationIcon::Success)->show();
        } else Notification::create("No HD objects found", NotificationIcon::Error)->show();
    }

    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) return false;
        
        this->addEventListener(
            KeybindSettingPressedEventV3(Mod::get(), "keybind"),
            [this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
                if (down && !repeat) selectHD();
            }
        );
        
        return true;
    }
};

class $modify(EditorToolsPopup, EditorCommandsPopup) {
    void onCommand(CCObject* sender) {
        auto button = typeinfo_cast<CCNode*>(sender)->getChildByIndex(0);
        auto label = button->getChildByType<CCLabelBMFont>();

        if (std::string(label->getString()) == "Select All High Detail") {
            auto editor = LevelEditorLayer::get();
            static_cast<CustomEditorLayer*>(editor)->selectHD();            
        } else EditorCommandsPopup::onCommand(sender);
    }

    void onBuildWindowContent() {
        EditorCommandsPopup::onBuildWindowContent();
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;

        addCommandButton("Select All High Detail", static_cast<EditorCommand>(20));
        layoutCommandButtons();
    }
};