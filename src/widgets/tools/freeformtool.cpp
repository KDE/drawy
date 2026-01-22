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
#include <iterator>
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

        // PERF: We are re-drawing the current stroke on every point insertion. This means
        // if the number of points in the stroke are a lot, rendering would be very slow it would start to lag.
        // To workaround this, we are making sure that the total number of points each stroke has will never exceed
        // a certain number. The more you zoom in, the less number of points your stroke can have. This is because
        // when zoomed in, you need to draw more pixels, which means more computation, which is slower.
        // This means if you have opacity set to something lower than 100%, you will see intersection points
        // after each split occurs. The only real fix that I can think of is introducing hardware acceleration and
        // offloading the rendering part to the GPU but Qt does not have good support for that so we might be stuck :<
        // See: https://invent.kde.org/graphics/drawy/-/merge_requests/171
        const qsizetype maxPointsPerStroke{
            std::min(Common::maxFreeformPointCount, std::max(static_cast<int>(Common::maxFreeformPointCount / zoom), Common::minFreeformPointCount))};
        if (curItem->points().size() >= maxPointsPerStroke) {
            std::shared_ptr<FreeformItem> prevItem{curItem};
            m_itemList.push_back(prevItem);

            // draw temporarily on canvas
            renderingContext->canvas()->paintCanvas([&](QPainter &painter) -> void {
                painter.scale(zoom, zoom);
                prevItem->draw(painter, spatialContext->offsetPos());
            });

            renderingContext->markForUpdate();

            curItem = std::dynamic_pointer_cast<FreeformItem>(m_itemFactory->create());
            curItem->setProperty(Property::Type::StrokeWidth, uiContext->propertyManager()->value(Property::Type::StrokeWidth));
            curItem->setProperty(Property::Type::StrokeColor, uiContext->propertyManager()->value(Property::Type::StrokeColor));
            curItem->setProperty(Property::Type::Opacity, uiContext->propertyManager()->value(Property::Type::Opacity));

            // add last point to ensure it looks continuous
            curItem->addPoint(prevItem->points().back(), prevItem->pressures().back());
        }

        curItem->addPoint(transformer.viewToWorld(curPoint), uiContext->appEvent()->pressure());

        // clear the overlay and draw again
        renderingContext->canvas()->setOverlayBg(renderingContext->canvas()->overlayBg());
        renderingContext->canvas()->paintOverlay([&](QPainter &painter) -> void {
            painter.scale(zoom, zoom);
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
<<<<<<< HEAD
    context->uiContext()->appEvent()->setButton(Qt::LeftButton);
=======
    context->uiContext()->appEvent().setButton(Qt::LeftButton);
    m_itemList.clear();
>>>>>>> 540e8e6 (perf: workaround performance issues when drawing large strokes)
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
