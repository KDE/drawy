// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "linemovetransformhandler.hpp"
#include "canvas/canvas.hpp"
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
#include "item/line.hpp"
#include <QPainter>
#include <QRectF>

namespace
{
std::shared_ptr<LineItem> getSelectedLineItem(ApplicationContext *context)
{
    const auto &selectedItems{context->selectionContext()->selectedItems()};
    Q_ASSERT(selectedItems.size() == 1);
    return std::dynamic_pointer_cast<LineItem>(*selectedItems.begin());
}
}

void LineMoveTransformHandler::renderHandles(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto item{getSelectedLineItem(context)};

    context->renderingContext()->canvas()->paintCanvas([item, &transformer](QPainter &painter) {
        painter.setPen(Common::selectionBorderPen());

        const QPointF start{item->transformObj().map(item->start())};
        const QPointF end{item->transformObj().map(item->end())};
        painter.drawLine(transformer.worldToView(start), transformer.worldToView(end));
    });

    context->renderingContext()->markForUpdate();
}

bool LineMoveTransformHandler::shouldActivate(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    const QPointF worldPos{transformer.viewToWorld(context->uiContext()->appEvent()->pos())};

    constexpr qreal cursorHitSize{20.0};
    const QRectF cursorHandle{createHandle(worldPos, cursorHitSize)};

    return getSelectedLineItem(context)->intersects(cursorHandle);
}

#include "moc_linemovetransformhandler.cpp"
