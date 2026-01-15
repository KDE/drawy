// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "polygondrawingtool.hpp"

#include <memory>
#include <qnamespace.h>

#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/insertitemcommand.hpp"
#include "command/selectcommand.hpp"
#include "common/renderitems.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "event/event.hpp"
#include "item/factory/itemfactory.hpp"
#include "item/polygon.hpp"
#include "properties/widgets/propertymanager.hpp"

PolygonDrawingTool::PolygonDrawingTool()
{
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties = {Property::Type::StrokeWidth, Property::Type::StrokeColor, Property::Type::StrokeStyle, Property::Type::Opacity};
}

void PolygonDrawingTool::mousePressed(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->event().button() == Qt::LeftButton) {
        auto spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};

        curItem = std::dynamic_pointer_cast<PolygonItem>(m_itemFactory->create());

        curItem->setProperty(Property::Type::StrokeWidth, uiContext->propertyManager().value(Property::Type::StrokeWidth));
        curItem->setProperty(Property::Type::StrokeColor, uiContext->propertyManager().value(Property::Type::StrokeColor));
        curItem->setProperty(Property::Type::Opacity, uiContext->propertyManager().value(Property::Type::Opacity));
        if (curItem->hasProperty(Property::Type::StrokeStyle)) {
            curItem->setProperty(Property::Type::StrokeStyle, uiContext->propertyManager().value(Property::Type::StrokeStyle));
        }

        curItem->setStart(transformer.viewToWorld(uiContext->event().pos()));

        m_isDrawing = true;
    }
}

void PolygonDrawingTool::mouseMoved(ApplicationContext *context)
{
    if (m_isDrawing) {
        auto spatialContext{context->spatialContext()};
        auto transformer{spatialContext->coordinateTransformer()};
        auto renderingContext{context->renderingContext()};
        auto uiContext{context->uiContext()};

        const QPointF offsetPos{spatialContext->offsetPos()};
        const qreal zoom{renderingContext->zoomFactor()};

        renderingContext->canvas().paintOverlay([&](QPainter &painter) -> void {
            painter.scale(zoom, zoom);
            curItem->erase(painter, offsetPos);
        });

        curItem->setEnd(transformer.viewToWorld(uiContext->event().pos()));

        renderingContext->canvas().paintOverlay([&](QPainter &painter) -> void {
            painter.scale(zoom, zoom);
            curItem->draw(painter, offsetPos);
        });

        renderingContext->markForUpdate();
    }
}

void PolygonDrawingTool::mouseReleased(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->event().button() == Qt::LeftButton && m_isDrawing) {
        auto spatialContext{context->spatialContext()};
        auto renderingContext{context->renderingContext()};
        CommandHistory &commandHistory{spatialContext->commandHistory()};

        QList<std::shared_ptr<Item>> itemVector{curItem};
        commandHistory.insert(std::make_shared<InsertItemCommand>(itemVector));

        renderingContext->canvas().setOverlayBg(Qt::transparent);

        m_isDrawing = false;

        renderingContext->markForRender();
        renderingContext->markForUpdate();
    }
}

void PolygonDrawingTool::cleanup()
{
    ApplicationContext *context{ApplicationContext::instance()};
    context->uiContext()->event().setButton(Qt::LeftButton);
    mouseReleased(context);
}

Tool::Type PolygonDrawingTool::type() const
{
    return Tool::Type::PolygonDrawing;
}
