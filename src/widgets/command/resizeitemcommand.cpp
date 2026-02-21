// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "resizeitemcommand.hpp"

#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "item/item.hpp"
#include <KLocalizedString>

ResizeItemCommand::ResizeItemCommand(QList<std::shared_ptr<Item>> items,
                                     const QTransform transform,
                                     const QTransform lockedTransform,
                                     std::unordered_map<std::shared_ptr<Item>, bool> useLocked)
    : ItemCommand{std::move(items)}
    , m_transform{transform}
    , m_lockedTransform{lockedTransform}
    , m_useLocked{std::move(useLocked)}
{
}

void ResizeItemCommand::execute(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

        if (m_useLocked[item]) {
            item->resize(m_lockedTransform);
        } else {
            item->resize(m_transform);
        }

        item->commitTransformation();

        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

void ResizeItemCommand::undo(ApplicationContext *context)
{
    auto &transformer{context->spatialContext()->coordinateTransformer()};
    auto &cacheGrid{context->renderingContext()->cacheGrid()};

    for (const auto &item : std::as_const(m_items)) {
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

        if (m_useLocked[item]) {
            item->resize(m_lockedTransform.inverted());
        } else {
            item->resize(m_transform.inverted());
        }

        item->commitTransformation();

        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

QString ResizeItemCommand::commandTitle() const
{
    return i18n("Resize Object");
}
