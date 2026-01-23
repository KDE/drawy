// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "iconmanager.hpp"
#include <QIcon>

using namespace Qt::Literals::StringLiterals;
IconManager::IconManager(QObject *parent)
    : QObject{parent}
    , m_iconName{
          {Icon::TOOL_SELECTION, u"tool_rect_selection"_s},
          {Icon::TOOL_FREEFORM, u"draw-freehand"_s},
          {Icon::TOOL_RECTANGLE, u"tool_rectangle"_s},
          {Icon::TOOL_ELLIPSE, u"tool_ellipse"_s},
          {Icon::TOOL_ARROW, u"draw-arrow"_s},
          {Icon::TOOL_LINE, u"tool_line"_s},
          {Icon::TOOL_ERASER, u"tool_eraser"_s},
          {Icon::TOOL_TEXT, u"tool_text"_s},
          {Icon::TOOL_MOVE, u"edit-move"_s},
          {Icon::TOOL_DIAMOND, {}},
          {Icon::ACTION_SAVE, u"document-save"_s},
          {Icon::ACTION_OPEN_FILE, u"document-open"_s},
          {Icon::ACTION_ZOOM_IN, u"zoom-in"_s},
          {Icon::ACTION_ZOOM_OUT, u"zoom-out"_s},
          {Icon::ACTION_UNDO, u"edit-undo"_s},
          {Icon::ACTION_REDO, u"edit-redo"_s},
          {Icon::ACTION_GROUP, u"object-group"_s},
          {Icon::ACTION_UNGROUP, u"object-ungroup"_s},
          {Icon::ACTION_DELETE, u"edit-delete"_s},
          {Icon::PROPERTY_LINE_DASHED, u"property-line_dashed"_s},
          {Icon::PROPERTY_LINE_SOLID, u"property-line_solid"_s},
          {Icon::PROPERTY_LINE_DOTTED, u"property-line_dotted"_s},
      }
{
}

QString IconManager::iconPath(Icon icon) const
{
    const QString path{m_iconName.at(icon)};
    return path;
}

QIcon IconManager::icon(Icon icon) const
{
    return QIcon::fromTheme(iconPath(icon));
}
