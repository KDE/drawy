// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "libdrawywidgets_private_export.h"
#include <QObject>
#include <unordered_map>

class LIBDRAWYWIDGETS_TESTS_EXPORT IconManager : public QObject
{
public:
    explicit IconManager(QObject *parent = nullptr);

    enum class Icon : int8_t {
        TOOL_SELECTION = 0,
        TOOL_FREEFORM,
        TOOL_RECTANGLE,
        TOOL_ELLIPSE,
        TOOL_ARROW,
        TOOL_LINE,
        TOOL_ERASER,
        TOOL_TEXT,
        TOOL_MOVE,
        ACTION_SAVE,
        ACTION_OPEN_FILE,
        ACTION_ZOOM_IN,
        ACTION_ZOOM_OUT,
        ACTION_UNDO,
        ACTION_REDO,
        ACTION_DARK_MODE,
        ACTION_LIGHT_MODE,
        ACTION_GROUP,
        ACTION_UNGROUP,
        ACTION_DELETE,
        UI_ANGLE_UP,
        UI_ANGLE_DOWN,
    };
    Q_ENUM(Icon)

    QIcon icon(IconManager::Icon icon) const;

    [[nodiscard]] QString iconPath(IconManager::Icon icon) const;

private:
    std::unordered_map<IconManager::Icon, QString> m_iconName;

    QString m_curTheme;
};
