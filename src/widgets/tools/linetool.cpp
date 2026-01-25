// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "linetool.hpp"

#include "item/factory/linefactory.hpp"
using namespace Qt::Literals::StringLiterals;

LineTool::LineTool()
{
    m_itemFactory = std::make_unique<LineFactory>();
}

QString LineTool::tooltip() const
{
    return QObject::tr("Line Tool");
}

QString LineTool::icon() const
{
    return u"tool_line"_s;
}
