// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectiontool.hpp"
#include <KLocalizedString>

#include <qnamespace.h>

#include <set>

#include "command/commandhistory.hpp"
#include "command/moveitemcommand.hpp"
#include "common/constants.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "event/event.hpp"
#include "item/item.hpp"
#include "selectiontoolmovestate.hpp"
#include "selectiontoolresizestate.hpp"
#include "selectiontoolrotatestate.hpp"
#include "selectiontoolselectstate.hpp"
#include "selectiontoolstate.hpp"

using namespace Qt::Literals::StringLiterals;
SelectionTool::SelectionTool(ApplicationContext *context)
    : Tool(context)
{
    m_cursor = QCursor(Qt::ArrowCursor);

    m_moveState = std::make_shared<SelectionToolMoveState>();
    m_selectState = std::make_shared<SelectionToolSelectState>();
    m_rotateState = std::make_shared<SelectionToolRotateState>();
    m_resizeState = std::make_shared<SelectionToolResizeState>();
}

void SelectionTool::mousePressed(ApplicationContext *context)
{
    m_stateLocked = getCurrentState(context)->mousePressed(context);
}

void SelectionTool::mouseMoved(ApplicationContext *context)
{
    getCurrentState(context)->mouseMoved(context);
}

void SelectionTool::mouseReleased(ApplicationContext *context)
{
    m_stateLocked = getCurrentState(context)->mouseReleased(context);
}

std::shared_ptr<SelectionToolState> SelectionTool::getCurrentState(ApplicationContext *context)
{
    if (m_stateLocked) {
        return m_curState;
    }

    auto selectionContext{context->selectionContext()};
    auto uiContext{context->uiContext()};
    auto transformer{context->spatialContext()->coordinateTransformer()};

    const QPointF viewCurPos{uiContext->appEvent()->pos()};
    const QPolygonF viewSelection{transformer.worldToView(selectionContext->selectionBox())};

    // move state
    if (viewSelection.containsPoint(viewCurPos, Qt::OddEvenFill) && !(uiContext->appEvent()->modifiers() & Qt::ShiftModifier)) {
        return m_curState = m_moveState;
    }

    // resize and rotate states
    constexpr qreal rotationHandleSize{100.0}, resizeHandleSize{20.0};
    for (QPointF point : viewSelection) {
        QRectF resizeHandle{point.x() - resizeHandleSize / 2.0, point.y() - resizeHandleSize / 2.0, resizeHandleSize, resizeHandleSize};
        if (resizeHandle.contains(viewCurPos)) {
            return m_curState = m_resizeState;
        }

        QRectF rotationHandle{point.x() - rotationHandleSize / 2.0, point.y() - rotationHandleSize / 2.0, rotationHandleSize, rotationHandleSize};
        if (rotationHandle.contains(viewCurPos)) {
            return m_curState = m_rotateState;
        }
    }

    // select state
    return m_curState = m_selectState;
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

    bool updated{true};
    switch (event->key()) {
    case Qt::Key_Left:
        commandHistory->insert(std::make_shared<MoveItemCommand>(items, QPoint{-delta, 0}));
        break;
    case Qt::Key_Right:
        commandHistory->insert(std::make_shared<MoveItemCommand>(items, QPoint{delta, 0}));
        break;
    case Qt::Key_Up:
        commandHistory->insert(std::make_shared<MoveItemCommand>(items, QPoint{0, -delta}));
        break;
    case Qt::Key_Down:
        commandHistory->insert(std::make_shared<MoveItemCommand>(items, QPoint{0, delta}));
        break;
    default:
        updated = false;
    }

    if (updated) {
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    }
}

QList<Property::Type> SelectionTool::properties() const
{
    if (m_context) {
        const auto &selectedItems{m_context->selectionContext()->selectedItems()};

        std::set<Property::Type> result;
        for (const auto &item : selectedItems) {
            for (const auto &property : item->propertyTypes()) {
                result.insert(property);
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

QString SelectionTool::tooltip() const
{
    return i18nc("@info:tooltip", "Selection Tool");
}

QString SelectionTool::icon() const
{
    return u"tool_rect_selection"_s;
}
