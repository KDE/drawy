// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "polygondrawingtool.hpp"

#include <memory>

#include "../canvas/canvas.hpp"
#include "../command/commandhistory.hpp"
#include "../command/insertitemcommand.hpp"
#include "../command/selectcommand.hpp"
#include "../common/renderitems.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../context/uicontext.hpp"
#include "../event/event.hpp"
#include "../item/factory/itemfactory.hpp"
#include "../item/polygon.hpp"
#include "../properties/widgets/propertymanager.hpp"

PolygonDrawingTool::PolygonDrawingTool()
{
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties = {Property::StrokeWidth, Property::StrokeColor};
}

void PolygonDrawingTool::mousePressed(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->event()->button() == Qt::LeftButton) {
        SpatialContext *spatialContext{context->spatialContext()};
        CoordinateTransformer *transformer{spatialContext->coordinateTransformer()};
        RenderingContext *renderingContext{context->renderingContext()};

        curItem = std::dynamic_pointer_cast<PolygonItem>(m_itemFactory->create());

        curItem->setProperty(Property::StrokeWidth, uiContext->propertyManager()->value(Property::StrokeWidth));
        curItem->setProperty(Property::StrokeColor, uiContext->propertyManager()->value(Property::StrokeColor));

        curItem->setStart(transformer->viewToWorld(uiContext->event()->pos()));

        qreal zoom{renderingContext->zoomFactor()};

        QPainter *painter{renderingContext->overlayPainter()};
        painter->save();
        painter->scale(zoom, zoom);

        m_isDrawing = true;
    }
};

void PolygonDrawingTool::mouseMoved(ApplicationContext *context)
{
    if (m_isDrawing) {
        SpatialContext *spatialContext{context->spatialContext()};
        CoordinateTransformer *transformer{spatialContext->coordinateTransformer()};
        RenderingContext *renderingContext{context->renderingContext()};
        UIContext *uiContext{context->uiContext()};

        QPainter *overlayPainter{renderingContext->overlayPainter()};

        QPointF offsetPos{spatialContext->offsetPos()};
        curItem->erase(*overlayPainter, offsetPos);
        curItem->setEnd(transformer->viewToWorld(uiContext->event()->pos()));
        curItem->draw(*overlayPainter, offsetPos);

        renderingContext->markForUpdate();
    }
};

void PolygonDrawingTool::mouseReleased(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->event()->button() == Qt::LeftButton && m_isDrawing) {
        SpatialContext *spatialContext{context->spatialContext()};
        CoordinateTransformer *transformer{spatialContext->coordinateTransformer()};
        RenderingContext *renderingContext{context->renderingContext()};
        CommandHistory *commandHistory{spatialContext->commandHistory()};

        QVector<std::shared_ptr<Item>> itemVector{curItem};
        commandHistory->insert(std::make_shared<InsertItemCommand>(itemVector));

        QPainter *overlayPainter{renderingContext->overlayPainter()};
        renderingContext->canvas()->overlay()->fill(Qt::transparent);
        overlayPainter->restore();

        m_isDrawing = false;

        renderingContext->markForRender();
        renderingContext->markForUpdate();
    }
};

void PolygonDrawingTool::cleanup()
{
    ApplicationContext *context{ApplicationContext::instance()};
    context->uiContext()->event()->setButton(Qt::LeftButton);
    mouseReleased(context);
}

Tool::Type PolygonDrawingTool::type() const
{
    return Tool::PolygonDrawing;
}
