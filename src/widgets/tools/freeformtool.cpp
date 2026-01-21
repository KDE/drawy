// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freeformtool.hpp"

#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/insertitemcommand.hpp"
#include "common/constants.hpp"
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
#include <qnamespace.h>

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

    m_properties = {Property::Type::StrokeWidth, Property::Type::StrokeColor, Property::Type::Opacity};
}

QString FreeformTool::tooltip() const
{
    return QObject::tr("Pen Tool");
}

void FreeformTool::mousePressed(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->appEvent()->button() == Qt::LeftButton) {
        auto spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};

        curItem = std::dynamic_pointer_cast<FreeformItem>(m_itemFactory->create());

        curItem->setProperty(Property::Type::StrokeWidth, uiContext->propertyManager()->value(Property::Type::StrokeWidth));
        curItem->setProperty(Property::Type::StrokeColor, uiContext->propertyManager()->value(Property::Type::StrokeColor));
        curItem->setProperty(Property::Type::Opacity, uiContext->propertyManager()->value(Property::Type::Opacity));

        m_lastPoint = uiContext->appEvent()->pos();

        curItem->addPoint(transformer.viewToWorld(m_lastPoint), uiContext->appEvent()->pressure());

        m_isDrawing = true;
    }
}

void FreeformTool::mouseMoved(ApplicationContext *context)
{
    if (m_isDrawing) {
        auto spatialContext{context->spatialContext()};
        auto renderingContext{context->renderingContext()};
        auto uiContext{context->uiContext()};
        CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};

        QPointF curPoint{uiContext->appEvent()->pos()};

        // distance between the two points in the "view" coordinate system
        double dist{std::sqrt(std::pow(m_lastPoint.x() - curPoint.x(), 2) + std::pow(m_lastPoint.y() - curPoint.y(), 2))};

        if (dist < FreeformItem::minPointDistance())
            return;

        curItem->addPoint(transformer.viewToWorld(curPoint), uiContext->appEvent()->pressure());
        // Split the stroke if there are too many points which makes it
        // expensive to render
        if (curItem->points().size() >= Common::maxFreeformPointCount) {
            std::shared_ptr<FreeformItem> prevItem{curItem};

            QList<std::shared_ptr<Item>> itemList{prevItem};
            spatialContext->commandHistory()->insert(std::make_shared<InsertItemCommand>(itemList));

            renderingContext->markForRender();

            curItem = std::dynamic_pointer_cast<FreeformItem>(m_itemFactory->create());
            curItem->setProperty(Property::Type::StrokeWidth, uiContext->propertyManager()->value(Property::Type::StrokeWidth));
            curItem->setProperty(Property::Type::StrokeColor, uiContext->propertyManager()->value(Property::Type::StrokeColor));
            curItem->setProperty(Property::Type::Opacity, uiContext->propertyManager()->value(Property::Type::Opacity));

            // add last point to ensure it looks continuous
            curItem->addPoint(prevItem->points().back(), prevItem->pressures().back());
        }

        curItem->addPoint(transformer.viewToWorld(curPoint), uiContext->appEvent().pressure());
        const qreal zoom{renderingContext->zoomFactor()};

        renderingContext->canvas()->paintOverlay([&](QPainter &painter) -> void {
            painter.scale(zoom, zoom);
            curItem->erase(painter, spatialContext->offsetPos());
            curItem->draw(painter, spatialContext->offsetPos());
        });

        m_lastPoint = curPoint;
        renderingContext->markForUpdate();
    }
}

void FreeformTool::mouseReleased(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->appEvent()->button() == Qt::LeftButton && m_isDrawing) {
        auto spatialContext{context->spatialContext()};
        auto renderingContext{context->renderingContext()};
        auto commandHistory{spatialContext->commandHistory()};

        renderingContext->canvas()->setOverlayBg(Qt::transparent);

        QList<std::shared_ptr<Item>> itemList{curItem};
        commandHistory->insert(std::make_shared<InsertItemCommand>(itemList));

        curItem.reset();

        m_isDrawing = false;
        renderingContext->markForRender();
        renderingContext->markForUpdate();
    }
}

void FreeformTool::tablet([[maybe_unused]] ApplicationContext *context)
{
    if (curItem && curItem->isPressureSimulated()) {
        curItem->setSimulatePressure(false);
    }
}

void FreeformTool::cleanup()
{
    ApplicationContext *context{ApplicationContext::instance()};
    context->uiContext()->appEvent()->setButton(Qt::LeftButton);
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
