// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QWidget>
class ToolBar;
class PropertyBar;
class ActionBar;
class Event;
class PropertyManager;
class Tool;
class ApplicationContext;
class KeybindManager;
class ActionManager;
class IconManager;

class UIContext : public QObject
{
    Q_OBJECT
public:
    explicit UIContext(ApplicationContext *context);
    ~UIContext() override;

    void initializeUIContext();

    [[nodiscard]] ToolBar *toolBar() const;
    [[nodiscard]] PropertyBar *propertyBar() const;
    [[nodiscard]] ActionBar *actionBar() const;
    [[nodiscard]] Event *appEvent() const;
    [[nodiscard]] KeybindManager *keybindManager() const;
    [[nodiscard]] ActionManager *actionManager() const;
    [[nodiscard]] PropertyManager *propertyManager() const;
    [[nodiscard]] IconManager *iconManager() const;

    void reset();

    void toolChanged(Tool &);

private:
    ToolBar *m_toolBar = nullptr;
    PropertyBar *m_propertyBar = nullptr;
    ActionBar *m_actionBar = nullptr;
    PropertyManager *m_propertyManager = nullptr;
    KeybindManager *m_keybindManager = nullptr;
    ActionManager *m_actionManager = nullptr;
    Event *m_event = nullptr;
    IconManager *m_iconManager = nullptr;

    Tool *m_lastTool = nullptr; // Used to call the cleanup function of the last tool

    ApplicationContext *const m_applicationContext;
};
