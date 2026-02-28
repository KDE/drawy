// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
class ToolBar;
class PropertyBar;
class Event;
class PropertyManager;
class Tool;
class ApplicationContext;
class KeybindManager;
class ActionManager;
class TopWidgets;
class TopLeftWidgets;
class BottomLeftWidgets;
class PluginFormManager;

class UIContext : public QObject
{
    Q_OBJECT
public:
    explicit UIContext(ApplicationContext *context);
    ~UIContext() override;

    void initializeUIContext();

    [[nodiscard]] ToolBar *toolBar() const;
    [[nodiscard]] PropertyBar *propertyBar() const;
    [[nodiscard]] TopWidgets *topWidgets() const;
    [[nodiscard]] TopLeftWidgets *topLeftWidgets() const;
    [[nodiscard]] BottomLeftWidgets *bottomLeftWidgets() const;
    [[nodiscard]] Event *appEvent() const;
    [[nodiscard]] KeybindManager *keybindManager() const;
    [[nodiscard]] ActionManager *actionManager() const;
    [[nodiscard]] PropertyManager *propertyManager() const;

    void showContextMenu() const;
    void reset();

    void toolChanged(Tool &);

    void slotThemeChanged();

Q_SIGNALS:
    void themeChanged(bool isDark);

private:
    ToolBar *m_toolBar = nullptr;
    PropertyBar *m_propertyBar = nullptr;
    PropertyManager *m_propertyManager = nullptr;
    KeybindManager *const m_keybindManager;
    ActionManager *m_actionManager = nullptr;
    TopWidgets *m_topWidgets = nullptr;
    TopLeftWidgets *m_topLeftWidgets = nullptr;
    BottomLeftWidgets *m_bottomLeftWidgets = nullptr;
    Event *const m_event;
    Tool *m_lastTool = nullptr; // Used to call the cleanup function of the last tool
    PluginFormManager *const mPluginFormManager;

    ApplicationContext *const m_applicationContext;
};
