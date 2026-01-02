// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "iconmanager.hpp"

#include <QIcon>
using namespace Qt::Literals::StringLiterals;
IconManager::IconManager(QObject *parent)
    : QObject{parent}
    , m_curTheme(u"fa-dark"_s) // default
{
    m_iconName = {{TOOL_SELECTION, u"tool-selection"_s},
                  {TOOL_FREEFORM, u"tool-freeform"_s},
                  {TOOL_RECTANGLE, u"tool-rectangle"_s},
                  {TOOL_ELLIPSE, u"tool-ellipse"_s},
                  {TOOL_ARROW, u"tool-arrow"_s},
                  {TOOL_LINE, u"tool-line"_s},
                  {TOOL_ERASER, u"tool-eraser"_s},
                  {TOOL_TEXT, u"tool-text"_s},
                  {TOOL_MOVE, u"tool-move"_s},
                  {ACTION_SAVE, u"action-save"_s},
                  {ACTION_OPEN_FILE, u"action-open_file"_s},
                  {ACTION_ZOOM_IN, u"action-zoom_in"_s},
                  {ACTION_ZOOM_OUT, u"action-zoom_out"_s},
                  {ACTION_UNDO, u"action-undo"_s},
                  {ACTION_REDO, u"action-redo"_s},
                  {ACTION_DARK_MODE, u"action-dark_mode"_s},
                  {ACTION_LIGHT_MODE, u"action-light_mode"_s},
                  {ACTION_GROUP, u"action-group"_s},
                  {ACTION_UNGROUP, u"action-ungroup"_s},
                  {ACTION_DELETE, u"action-delete"_s},
                  {UI_ANGLE_DOWN, u"ui-angle_up"_s},
                  {UI_ANGLE_UP, u"ui-angle_down"_s}};
}

QIcon IconManager::icon(Icon icon) const
{
    const QString path{u":/icons/"_s + m_curTheme + u'/' + m_iconName.at(icon) + u".svg"_s};
    return QIcon(path);
}
