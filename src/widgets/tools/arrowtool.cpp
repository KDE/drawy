// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrowtool.hpp"

#include "item/factory/arrowfactory.hpp"
using namespace Qt::Literals::StringLiterals;

ArrowTool::ArrowTool()
{
    m_itemFactory = std::make_unique<ArrowFactory>();
}

QString ArrowTool::tooltip() const
{
    return QObject::tr("Arrow Tool");
}

QString ArrowTool::icon() const
{
    return u"draw-arrow"_s;
}

Tool::Type ArrowTool::type() const
{
    return Tool::Type::Arrow;
}
