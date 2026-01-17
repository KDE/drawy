// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectiontool.hpp"

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
#include "selectiontoolselectstate.hpp"
#include "selectiontoolstate.hpp"

SelectionTool::SelectionTool()
{
    m_cursor = QCursor(Qt::ArrowCursor);

    m_moveState = std::make_shared<SelectionToolMoveState>();
    m_selectState = std::make_shared<SelectionToolSelectState>();
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
    if (m_stateLocked)
        return m_curState;

    auto selectionContext{context->selectionContext()};
    auto uiContext{context->uiContext()};
    auto transformer{context->spatialContext()->coordinateTransformer()};

    QPointF worldCurPos{transformer.viewToWorld(uiContext->appEvent().pos())};

    // TODO: Implement resizing and rotation as well
    if (selectionContext->selectionBox().contains(worldCurPos) && !(uiContext->appEvent().modifiers() & Qt::ShiftModifier)) {
        return m_curState = m_moveState;
    } else {
        return m_curState = m_selectState;
    }
}

void SelectionTool::keyPressed(ApplicationContext *context)
{
    auto &selectedItems{context->selectionContext()->selectedItems()};
    if (selectedItems.empty())
        return;

    auto &event{context->uiContext()->appEvent()};
    auto &commandHistory{context->spatialContext()->commandHistory()};
    QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};

    int delta{Common::translationDelta};
    if (event.modifiers() & Qt::ShiftModifier)
        delta = Common::shiftTranslationDelta;

    bool updated{true};
    switch (event.key()) {
    case Qt::Key_Left:
        commandHistory.insert(std::make_shared<MoveItemCommand>(items, QPoint{-delta, 0}));
        break;
    case Qt::Key_Right:
        commandHistory.insert(std::make_shared<MoveItemCommand>(items, QPoint{delta, 0}));
        break;
    case Qt::Key_Up:
        commandHistory.insert(std::make_shared<MoveItemCommand>(items, QPoint{0, -delta}));
        break;
    case Qt::Key_Down:
        commandHistory.insert(std::make_shared<MoveItemCommand>(items, QPoint{0, delta}));
        break;
    default:
        updated = false;
    }

    if (updated) {
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    }
}

const QList<Property::Type> SelectionTool::properties() const
{
    ApplicationContext *context{ApplicationContext::instance()};
    auto &selectedItems{context->selectionContext()->selectedItems()};

    std::set<Property::Type> result{};
    for (const auto &item : selectedItems) {
        for (const auto &property : item->propertyTypes()) {
            result.insert(property);
        }
    }

    QList<Property::Type> output(result.begin(), result.end());

    if (!selectedItems.empty()) {
        output += QList<Property::Type>{Property::Type::Actions};
    }

    return output;
}

Tool::Type SelectionTool::type() const
{
    return Tool::Type::Selection;
}

QString SelectionTool::tooltip() const
{
    return QObject::tr("Selection Tool");
}

IconManager::Icon SelectionTool::icon() const
{
    return IconManager::Icon::TOOL_SELECTION;
}
