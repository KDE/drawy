// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrowtool.hpp"
#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "event/event.hpp"
#include "item/line.hpp"
#include "properties/widgets/propertymanager.hpp"
#include <KLocalizedString>

#include "item/factory/arrowfactory.hpp"
using namespace Qt::Literals::StringLiterals;

ArrowTool::ArrowTool(ApplicationContext *context)
    : LineDrawingTool(context)
{
    m_itemFactory = std::make_unique<ArrowFactory>();
    m_properties += {Property::Type::StartArrowType, Property::Type::EndArrowType};
}

void ArrowTool::mousePressed(ApplicationContext *context)
{
    LineDrawingTool::mousePressed(context);

    auto uiContext{context->uiContext()};
    if (uiContext->appEvent()->button() == Qt::LeftButton) {
        if (curItem) {
            curItem->setProperty(Property::Type::StartArrowType, uiContext->propertyManager()->value(Property::Type::StartArrowType));
            curItem->setProperty(Property::Type::EndArrowType, uiContext->propertyManager()->value(Property::Type::EndArrowType));
        }

        qDebug() << "Property: " << uiContext->propertyManager()->value(Property::Type::StartArrowType);
    }
}

QString ArrowTool::icon() const
{
    return u"draw-arrow"_s;
}

Tool::Type ArrowTool::type() const
{
    return Tool::Type::Arrow;
}
