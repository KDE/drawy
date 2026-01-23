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
        auto renderingContext{context->renderingContext()};
        CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};

        curItem = std::dynamic_pointer_cast<FreeformItem>(m_itemFactory->create());

        curItem->setProperty(Property::Type::StrokeWidth, uiContext->propertyManager()->value(Property::Type::StrokeWidth));
        curItem->setProperty(Property::Type::StrokeColor, uiContext->propertyManager()->value(Property::Type::StrokeColor));
        curItem->setProperty(Property::Type::Opacity, uiContext->propertyManager()->value(Property::Type::Opacity));

        m_lastPoint = uiContext->appEvent()->pos();
        m_itemList.clear();

        curItem->addPoint(transformer.viewToWorld(m_lastPoint), uiContext->appEvent()->pressure());
        m_currentCache = QPixmap{renderingContext->canvas()->dimensions()};
        m_currentCache.fill(Qt::transparent);

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

        const qreal zoom{renderingContext->zoomFactor()};
        curItem->addPoint(transformer.viewToWorld(curPoint), uiContext->appEvent()->pressure());

        if (curItem->isBufferFull()) {
            QPainter cachePainter{&m_currentCache};
            cachePainter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
            cachePainter.scale(zoom, zoom);
            curItem->drawBuffer(cachePainter, spatialContext->offsetPos());
        } else {
            // clear the overlay and draw the cached pixmap and the latest buffer
            renderingContext->canvas()->setOverlayBg(renderingContext->canvas()->overlayBg());
            renderingContext->canvas()->paintOverlay([&](QPainter &painter) -> void {
                painter.drawPixmap(QPointF{0, 0}, m_currentCache);
                painter.scale(zoom, zoom);
                curItem->drawBuffer(painter, spatialContext->offsetPos());
            });
        }

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

        curItem->finalizeStroke();
        m_itemList.push_back(curItem);
        commandHistory->insert(std::make_shared<InsertItemCommand>(m_itemList));

        curItem.reset();
        m_currentCache = QPixmap{};

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
    m_itemList.clear();
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
