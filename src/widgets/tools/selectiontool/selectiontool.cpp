// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectiontool.hpp"
#include <KLocalizedString>

#include <set>

#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/deselectcommand.hpp"
#include "command/moveitemcommand.hpp"
#include "command/selectcommand.hpp"
#include "common/constants.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "data-structures/quadtree.hpp"
#include "event/event.hpp"
#include "item/arrow/arrow.hpp"
#include "item/item.hpp"

using namespace Qt::Literals::StringLiterals;
SelectionTool::SelectionTool(ApplicationContext *context)
    : Tool(context)
    , m_curHandlerState{TransformHandler::State::Unlocked}
{
    m_cursor = QCursor(Qt::ArrowCursor);
}

void SelectionTool::mousePressed(ApplicationContext *context)
{
    updateCurrentHandler(context);

    if (m_curHandler) {
        m_curHandlerState = m_curHandler->mousePressed(context);
        return;
    }

    auto uiContext{context->uiContext()};
    auto event{uiContext->appEvent()};

    if (event->button() == Qt::LeftButton) {
        m_lastPos = uiContext->appEvent()->pos();

        auto spatialContext{context->spatialContext()};
        auto selectionContext{context->selectionContext()};
        auto renderingContext{context->renderingContext()};
        auto commandHistory{spatialContext->commandHistory()};
        auto transformer{spatialContext->coordinateTransformer()};

        constexpr qreal cursorHitSize{10.0};
        const QPointF worldPos{transformer.viewToWorld(m_lastPos)};
        const QRectF cursorRegion{worldPos.x() - cursorHitSize / 2.0, worldPos.y() - cursorHitSize / 2.0, cursorHitSize, cursorHitSize};

        QList<std::shared_ptr<Item>> intersectingItems{spatialContext->quadtree().queryItems(cursorRegion, [](const std::shared_ptr<Item> &item, auto &region) {
            return item->intersects(region);
        })};

        auto lockState{TransformHandler::State::Locked};

        const auto &selectedItems{selectionContext->selectedItems()};

        if (!(event->modifiers() & Qt::ShiftModifier)) {
            const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};

            if (!items.empty()) {
                commandHistory->push(std::make_shared<DeselectCommand>(items));
            }
        }

        if (intersectingItems.empty()) {
            m_isSelecting = true;
        } else {
            auto &item{intersectingItems.back()};

            if (!item->locked()) {
                if ((event->modifiers() & Qt::ShiftModifier) && selectedItems.contains(item)) {
                    // deselect the item if selected
                    commandHistory->push(std::make_shared<DeselectCommand>(QList<std::shared_ptr<Item>>{item}));
                } else {
                    commandHistory->push(std::make_shared<SelectCommand>(QList<std::shared_ptr<Item>>{item}));
                }
            }

            m_isSelecting = false;
            lockState = TransformHandler::State::Unlocked;
        }

        renderingContext->markForRender();
        renderingContext->markForUpdate();

        m_curHandlerState = lockState;
        return;
    }

    m_curHandlerState = TransformHandler::State::Locked;
}

void SelectionTool::mouseMoved(ApplicationContext *context)
{
    updateCurrentHandler(context);

    if (m_curHandler) {
        m_curHandlerState = m_curHandler->mouseMoved(context);
        return;
    }

    auto renderingContext{context->renderingContext()};
    renderingContext->canvas()->setCursor(Qt::ArrowCursor);

    if (!m_isSelecting) {
        return;
    }

    auto spatialContext{context->spatialContext()};
    auto uiContext{context->uiContext()};
    auto selectionContext{context->selectionContext()};
    auto &transformer{spatialContext->coordinateTransformer()};

    renderingContext->canvas()->setOverlayBg(Qt::transparent);

    const QPointF curPos{uiContext->appEvent()->pos()};

    const QRectF selectionBox{m_lastPos, curPos};
    const QRectF worldSelectionBox{transformer.viewToWorld(selectionBox)};

    QList<std::shared_ptr<Item>> intersectingItems{
        spatialContext->quadtree().queryItems(worldSelectionBox, [](const std::shared_ptr<Item> &item, const QRectF &rect) {
            return rect.contains(item->boundingBox());
        })};

    selectionContext->setSelectedItems(intersectingItems.begin(), intersectingItems.end());

    // TODO: Remove magic numbers
    renderingContext->canvas()->paintOverlay([&](QPainter &painter) -> void {
        const QPen pen{Common::selectionBorderColor};
        painter.setPen(pen);

        painter.drawRect(selectionBox);
        painter.fillRect(selectionBox, Common::selectionBackgroundColor);
    });

    renderingContext->markForRender();
    renderingContext->markForUpdate();
}

void SelectionTool::mouseReleased(ApplicationContext *context)
{
    updateCurrentHandler(context);

    if (m_curHandler) {
        m_curHandlerState = m_curHandler->mouseReleased(context);
        return;
    }

    if (m_isSelecting) {
        auto renderingContext{context->renderingContext()};
        auto &selectedItems{context->selectionContext()->selectedItems()};

        if (!selectedItems.empty()) {
            QList<std::shared_ptr<Item>> items;

            for (const auto &item : selectedItems) {
                if (!item->locked()) {
                    items.push_back(item);
                }
            }

            context->selectionContext()->reset();

            auto commandHistory{context->spatialContext()->commandHistory()};
            commandHistory->push(std::make_shared<SelectCommand>(items));
        }

        renderingContext->canvas()->setOverlayBg(Qt::transparent);
        renderingContext->markForUpdate();

        m_isSelecting = false;
    }

    m_curHandlerState = TransformHandler::State::Unlocked;
}

void SelectionTool::updateCurrentHandler(ApplicationContext *context)
{
    if (m_curHandlerState == TransformHandler::State::Locked) {
        return;
    }

    auto selectionContext{context->selectionContext()};
    auto transformer{context->spatialContext()->coordinateTransformer()};

    const auto &selectedItems{selectionContext->selectedItems()};

    auto handlerTypes{selectionContext->defaultHandlerTypesForMultiSelection()};
    if (selectedItems.size() == 1) {
        handlerTypes = (*selectedItems.begin())->transformHandlers();
    }

    for (const auto &handlerType : std::as_const(handlerTypes)) {
        auto handler{TransformHandler::getHandler(handlerType)};

        if (handler->shouldActivate(context)) {
            m_curHandler = handler;
            return;
        }
    }

    m_curHandler = nullptr;
}

void SelectionTool::keyPressed(ApplicationContext *context)
{
    const auto &selectedItems{context->selectionContext()->selectedItems()};
    if (selectedItems.empty()) {
        return;
    }

    auto event{context->uiContext()->appEvent()};
    auto commandHistory{context->spatialContext()->commandHistory()};
    const QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};

    int delta{Common::translationDelta};
    if (event->modifiers() & Qt::ShiftModifier) {
        delta = Common::shiftTranslationDelta;
    }

    const QPointF worldOriginalPos{context->selectionContext()->selectionBox().boundingRect().topLeft()};

    bool updated{true};
    QPointF worldFinalPos{worldOriginalPos};

    switch (event->key()) {
    case Qt::Key_Left:
        worldFinalPos.setX(worldFinalPos.x() - delta);
        break;
    case Qt::Key_Right:
        worldFinalPos.setX(worldFinalPos.x() + delta);
        break;
    case Qt::Key_Up:
        worldFinalPos.setY(worldFinalPos.y() - delta);
        ;
        break;
    case Qt::Key_Down:
        worldFinalPos.setY(worldFinalPos.y() + delta);
        ;
        break;
    default:
        updated = false;
    }

    if (updated) {
        commandHistory->push(std::make_shared<MoveItemCommand>(items, worldOriginalPos, worldFinalPos));
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    }
}

QList<Property::Type> SelectionTool::properties() const
{
    if (m_context) {
        const auto &selectedItems{m_context->selectionContext()->selectedItems()};

        std::set<Property::Type> result;
        bool hasArrow = false;
        for (const auto &item : selectedItems) {
            for (const auto &property : item->propertyTypes()) {
                result.insert(property);
                if (!hasArrow && dynamic_cast<const ArrowItem *>(item.get())) {
                    hasArrow = true;
                }
            }
        }

        QList<Property::Type> output(result.begin(), result.end());
        if (selectedItems.size() > 1) {
            output += QList<Property::Type>{Property::Type::Alignment};
        }

        if (!selectedItems.empty()) {
            output += QList<Property::Type>{Property::Type::ZOrder, Property::Type::Actions};
        }

        return output;
    } else {
        return {};
    }
}

Tool::Type SelectionTool::type() const
{
    return Tool::Type::Selection;
}

QString SelectionTool::icon() const
{
    return u"edit-select"_s;
}
