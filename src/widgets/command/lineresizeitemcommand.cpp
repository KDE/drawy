// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "lineresizeitemcommand.hpp"

#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "item/item.hpp"
#include "item/line.hpp"
#include <KLocalizedString>

LineResizeItemCommand::LineResizeItemCommand(QList<std::shared_ptr<Item>> items, const QPointF initialPos, const QPointF finalPos, const HandleType handleType)
    : ItemCommand{std::move(items)}
    , m_initialPos{initialPos}
    , m_finalPos{finalPos}
    , m_handleType{handleType}
{
    Q_ASSERT(m_items.size() == 1);
}

void LineResizeItemCommand::execute(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    auto item{std::dynamic_pointer_cast<LineItem>(m_items.front())};

    cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

    switch (m_handleType) {
    case HandleType::Start: {
        item->setStart(m_finalPos);
        break;
    };
    case HandleType::End: {
        item->setEnd(m_finalPos);
        break;
    };
    }

    cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
}

void LineResizeItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    auto item{std::dynamic_pointer_cast<LineItem>(m_items.front())};

    cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

    switch (m_handleType) {
    case HandleType::Start: {
        item->setStart(m_initialPos);
        break;
    };
    case HandleType::End: {
        item->setEnd(m_initialPos);
        break;
    };
    }

    cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
}

QString LineResizeItemCommand::commandTitle() const
{
    return i18n("Resize Line");
}
