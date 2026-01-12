// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "iconmanager.hpp"
#include <QIcon>

using namespace Qt::Literals::StringLiterals;
IconManager::IconManager(QObject *parent)
    : QObject{parent}
    , m_iconName{{Icon::TOOL_SELECTION, u"tool-selection"_s},
                 {Icon::TOOL_FREEFORM, u"tool-freeform"_s},
                 {Icon::TOOL_RECTANGLE, u"tool-rectangle"_s},
                 {Icon::TOOL_ELLIPSE, u"tool-ellipse"_s},
                 {Icon::TOOL_ARROW, u"tool-arrow"_s},
                 {Icon::TOOL_LINE, u"tool-line"_s},
                 {Icon::TOOL_ERASER, u"tool-eraser"_s},
                 {Icon::TOOL_TEXT, u"tool-text"_s},
                 {Icon::TOOL_MOVE, u"tool-move"_s},
                 {Icon::ACTION_SAVE, u"action-save"_s},
                 {Icon::ACTION_OPEN_FILE, u"action-open_file"_s},
                 {Icon::ACTION_ZOOM_IN, u"action-zoom_in"_s},
                 {Icon::ACTION_ZOOM_OUT, u"action-zoom_out"_s},
                 {Icon::ACTION_UNDO, u"action-undo"_s},
                 {Icon::ACTION_REDO, u"action-redo"_s},
                 {Icon::ACTION_DARK_MODE, u"action-dark_mode"_s},
                 {Icon::ACTION_LIGHT_MODE, u"action-light_mode"_s},
                 {Icon::ACTION_GROUP, u"action-group"_s},
                 {Icon::ACTION_UNGROUP, u"action-ungroup"_s},
                 {Icon::ACTION_DELETE, u"action-delete"_s}}
    , m_curTheme(u"fa-dark"_s) // default
{
}

QString IconManager::iconPath(Icon icon) const
{
    const QString path{u":/icons/"_s + m_curTheme + u'/' + m_iconName.at(icon) + u".svg"_s};
    return path;
}

QIcon IconManager::icon(Icon icon) const
{
    return QIcon(iconPath(icon));
}
