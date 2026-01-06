// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freeformtool.hpp"

#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/insertitemcommand.hpp"
#include "common/renderitems.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "event/event.hpp"
#include "item/factory/freeformfactory.hpp"
#include "item/freeform.hpp"
#include "item/item.hpp"
#include "properties/widgets/propertymanager.hpp"

FreeformTool::FreeformTool()
{
    m_itemFactory = std::make_unique<FreeformFactory>();

    const int size{5};
    const int borderWidth{1};
    QBitmap cursorShape{size, size};
    QPen cursorPen{};
    cursorShape.fill(Qt::transparent);
    cursorPen.setWidth(borderWidth);
    cursorPen.setColor(Qt::black);
    cursorPen.setJoinStyle(Qt::MiterJoin);

    QPainter cursorPainter{&cursorShape};
    cursorPainter.setPen(cursorPen);

    cursorPainter.drawEllipse(borderWidth / 2, borderWidth / 2, size - borderWidth, size - borderWidth);
    m_cursor = QCursor{cursorShape, size / 2, size / 2};

    m_properties = {Property::Type::StrokeWidth, Property::Type::StrokeColor};
}

QString FreeformTool::tooltip() const
{
    return QObject::tr("Pen Tool");
}

void FreeformTool::mousePressed(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->event()->button() == Qt::LeftButton) {
        SpatialContext *spatialContext{context->spatialContext()};
        RenderingContext *renderingContext{context->renderingContext()};
        CoordinateTransformer *transformer{spatialContext->coordinateTransformer()};

        curItem = std::dynamic_pointer_cast<FreeformItem>(m_itemFactory->create());

        curItem->setProperty(Property::Type::StrokeWidth, uiContext->propertyManager()->value(Property::Type::StrokeWidth));
        curItem->setProperty(Property::Type::StrokeColor, uiContext->propertyManager()->value(Property::Type::StrokeColor));

        m_lastPoint = uiContext->event()->pos();

        curItem->addPoint(transformer->viewToWorld(m_lastPoint), uiContext->event()->pressure());

        auto *painter{renderingContext->overlayPainter()};
        painter->save();

        const qreal zoom{renderingContext->zoomFactor()};
        painter->scale(zoom, zoom);

        m_isDrawing = true;
    }
}

void FreeformTool::mouseMoved(ApplicationContext *context)
{
    if (m_isDrawing) {
        SpatialContext *spatialContext{context->spatialContext()};
        RenderingContext *renderingContext{context->renderingContext()};
        UIContext *uiContext{context->uiContext()};
        CoordinateTransformer *transformer{spatialContext->coordinateTransformer()};

        const QPointF curPoint{uiContext->event()->pos()};

        // distance between the two points in the "view" coordinate system
        double dist{std::sqrt(std::pow(m_lastPoint.x() - curPoint.x(), 2) + std::pow(m_lastPoint.y() - curPoint.y(), 2))};

        if (dist < FreeformItem::minPointDistance())
            return;

        QPainter *painter{renderingContext->overlayPainter()};

        curItem->addPoint(transformer->viewToWorld(curPoint), uiContext->event()->pressure());
        curItem->quickDraw(*painter, spatialContext->offsetPos());

        m_lastPoint = curPoint;
        renderingContext->markForUpdate();
    }
}

void FreeformTool::mouseReleased(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->event()->button() == Qt::LeftButton && m_isDrawing) {
        SpatialContext *spatialContext{context->spatialContext()};
        RenderingContext *renderingContext{context->renderingContext()};
        CommandHistory *commandHistory{spatialContext->commandHistory()};

        QPainter *overlayPainter{renderingContext->overlayPainter()};
        renderingContext->canvas()->overlay()->fill(Qt::transparent);
        overlayPainter->restore();

        QVector<std::shared_ptr<Item>> itemsAfterSplitting{curItem->split()};
        commandHistory->insert(std::make_shared<InsertItemCommand>(itemsAfterSplitting));

        curItem.reset();

        m_isDrawing = false;
        renderingContext->markForRender();
        renderingContext->markForUpdate();
    }
}

void FreeformTool::cleanup()
{
    ApplicationContext *context{ApplicationContext::instance()};
    context->uiContext()->event()->setButton(Qt::LeftButton);
    mouseReleased(context);
}

Tool::Type FreeformTool::type() const
{
    return Tool::Type::Freeform;
}

IconManager::Icon FreeformTool::icon() const
{
    return IconManager::Icon::TOOL_FREEFORM;
}
