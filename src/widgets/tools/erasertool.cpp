// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "erasertool.hpp"

#include "common/constants.hpp"
#include <QPainter>
#include <qnamespace.h>

#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/removeitemcommand.hpp"
#include "common/renderitems.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "event/event.hpp"
#include "item/item.hpp"
#include "properties/widgets/propertymanager.hpp"

EraserTool::EraserTool()
{
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties = {Property::Type::EraserSize};
}

void EraserTool::mousePressed(ApplicationContext *context)
{
    auto event{context->uiContext()->appEvent()};

    if (event->button() == Qt::LeftButton) {
        m_isErasing = true;
    }
}

// FIXME: messy code
void EraserTool::mouseMoved(ApplicationContext *context)
{
    auto spatialContext{context->spatialContext()};
    auto renderingContext{context->renderingContext()};
    auto uiContext{context->uiContext()};
    auto transformer{spatialContext->coordinateTransformer()};

    // Erase previous box
    renderingContext->canvas()->paintOverlay([&](QPainter &painter) -> void {
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(m_lastRect + Common::cleanupMargin, Qt::transparent);
    });

    const int eraserSide{uiContext->propertyManager()->value(Property::Type::EraserSize).value<int>()};
    const QSize eraserSize{eraserSide, eraserSide};

    // TODO: Adjustable eraser size
    const double eraserCenterOffset{eraserSide / 2.0 - 1};
    const QPointF eraserCenterOffsetPoint{eraserCenterOffset, eraserCenterOffset};

    QRectF curRect{uiContext->appEvent()->pos() - eraserCenterOffsetPoint, eraserSize};
    QRectF worldEraserRect{transformer.viewToWorld(curRect)};

    if (m_isErasing) {
        QList<std::shared_ptr<Item>> toBeErased{spatialContext->quadtree().queryItems(worldEraserRect)};

        for (const std::shared_ptr<Item> &item : toBeErased) {
            if (m_toBeErased.count(item) > 0)
                continue;

            item->setProperty(Property::Type::Opacity, Property{Common::eraseItemOpacity, Property::Type::Opacity});

            m_toBeErased.insert(item);
            renderingContext->cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
            renderingContext->markForRender();
        }

        renderingContext->canvas()->paintOverlay([&](QPainter &painter) -> void {
            painter.fillRect(curRect, Common::eraserBackgroundColor);
        });
    }

    renderingContext->markForUpdate();

    // Draw eraser box
    renderingContext->canvas()->paintOverlay([&](QPainter &painter) -> void {
        QPen pen{Common::eraserBorderColor, Common::eraserBorderWidth};
        painter.setPen(pen);
        painter.drawRect(curRect);
    });

    renderingContext->markForUpdate();

    m_lastRect = curRect;
}

void EraserTool::mouseReleased(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->appEvent()->button() == Qt::LeftButton) {
        auto spatialContext{context->spatialContext()};
        auto renderingContext{context->renderingContext()};
        auto selectionContext{context->selectionContext()};
        auto commandHistory{spatialContext->commandHistory()};

        QList<std::shared_ptr<Item>> erasedItems;
        for (const std::shared_ptr<Item> &item : m_toBeErased) {
            if (selectionContext->selectedItems().count(item) > 0) {
                selectionContext->selectedItems().erase(item);
            }

            // reset opacity
            item->setProperty(Property::Type::Opacity, Property{Common::maxItemOpacity, Property::Type::Opacity});
            erasedItems.push_back(item);
        }

        if (!erasedItems.empty()) {
            commandHistory->insert(std::make_shared<RemoveItemCommand>(erasedItems));
        }

        renderingContext->markForRender();
        renderingContext->markForUpdate();

        m_toBeErased.clear();
        m_isErasing = false;
    }
}

void EraserTool::leave([[maybe_unused]] ApplicationContext *context)
{
    cleanup();
}

void EraserTool::cleanup()
{
    ApplicationContext *context{ApplicationContext::instance()};

    context->uiContext()->appEvent()->setButton(Qt::LeftButton);
    mouseReleased(context);

    context->renderingContext()->canvas()->setOverlayBg(Qt::transparent);

    context->renderingContext()->markForUpdate();
}

Tool::Type EraserTool::type() const
{
    return Tool::Type::Eraser;
}

QString EraserTool::tooltip() const
{
    return QObject::tr("Eraser Tool");
}

IconManager::Icon EraserTool::icon() const
{
    return IconManager::Icon::TOOL_ERASER;
}
