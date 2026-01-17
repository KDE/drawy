// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "updatepropertycommand.hpp"

#include <QRectF>
#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "item/item.hpp"

UpdatePropertyCommand::UpdatePropertyCommand(QList<std::shared_ptr<Item>> items, Property newProperty)
    : ItemCommand{std::move(items)}
    , m_newProperty{std::move(newProperty)}
{
}

void UpdatePropertyCommand::execute(ApplicationContext *context)
{
    const Property::Type type{m_newProperty.type()};

    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        try {
            m_properties[item] = item->property(type);
            item->setProperty(type, m_newProperty);
            dirtyRegion |= item->boundingBox();
        } catch (const std::logic_error &e) {
            // Ignore if not found
        }
    }

    const QRect gridDirtyRegion{context->spatialContext()->coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->renderingContext()->cacheGrid().markDirty(gridDirtyRegion);
};

void UpdatePropertyCommand::undo(ApplicationContext *context)
{
    const Property::Type type{m_newProperty.type()};

    QRectF dirtyRegion{};
    for (const auto &item : std::as_const(m_items)) {
        try {
            item->setProperty(type, m_properties[item]);
            dirtyRegion |= item->boundingBox();
        } catch (const std::logic_error &e) {
            // Ignore if not found
        }
    }

    const QRect gridDirtyRegion{context->spatialContext()->coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->renderingContext()->cacheGrid().markDirty(gridDirtyRegion);
};
