// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "erasertool.hpp"
#include <KLocalizedString>

#include "common/constants.hpp"
#include <QPainter>

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
using namespace Qt::Literals::StringLiterals;

EraserTool::EraserTool(ApplicationContext *context)
    : Tool(context)
{
    m_cursor = QCursor(Qt::BlankCursor);

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
    const QRectF worldEraserRect{transformer.viewToWorld(curRect)};

    if (m_isErasing) {
        const QList<std::shared_ptr<Item>> toBeErased{spatialContext->quadtree().queryItems(worldEraserRect)};

        for (const std::shared_ptr<Item> &item : toBeErased) {
            if (m_toBeErased.contains(item)) {
                continue;
            }

            const auto oldOpacity{item->property(Property::Type::Opacity).value<qreal>()};
            const auto newOpacity{oldOpacity * Common::erasedItemOpacityDecrease / 100};
            item->setProperty(Property::Type::Opacity, Property{newOpacity, Property::Type::Opacity});

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
        const QPen pen{Common::eraserBorderColor, Common::eraserBorderWidth};
        painter.setPen(pen);
        painter.drawRect(curRect);
    });

    renderingContext->markForUpdate();

    m_lastRect = curRect;
}

void EraserTool::mouseReleased(ApplicationContext *context)
{
    const UIContext *uiContext{context->uiContext()};

    if (uiContext->appEvent()->button() == Qt::LeftButton) {
        auto spatialContext{context->spatialContext()};
        auto renderingContext{context->renderingContext()};
        auto selectionContext{context->selectionContext()};
        auto commandHistory{spatialContext->commandHistory()};

        QList<std::shared_ptr<Item>> erasedItems;
        for (const std::shared_ptr<Item> &item : m_toBeErased) {
            selectionContext->removeFromSelection(item);

            // reset opacity
            item->setProperty(Property::Type::Opacity, Property{Common::maxItemOpacity, Property::Type::Opacity});
            erasedItems.push_back(item);
        }

        if (!erasedItems.empty()) {
            commandHistory->push(std::make_shared<RemoveItemCommand>(erasedItems));
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
    m_context->uiContext()->appEvent()->setButton(Qt::LeftButton);
    mouseReleased(m_context);

    m_context->renderingContext()->canvas()->setOverlayBg(Qt::transparent);

    m_context->renderingContext()->markForUpdate();
}

Tool::Type EraserTool::type() const
{
    return Tool::Type::Eraser;
}

QString EraserTool::icon() const
{
    return u"tool_eraser"_s;
}
