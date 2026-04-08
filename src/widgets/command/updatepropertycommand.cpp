// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "updatepropertycommand.hpp"

#include <KLocalizedString>
#include <QRectF>
#include <utility>

#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "drawy_command_debug.h"
#include "item/item.hpp"

UpdatePropertyCommand::UpdatePropertyCommand(QList<std::shared_ptr<Item>> items, Property newProperty)
    : ItemCommand{std::move(items)}
    , m_newProperty{std::move(newProperty)}
{
    qCDebug(DRAWY_COMMAND_LOG) << "UpdatePropertyCommand" << m_items.count();
}

void UpdatePropertyCommand::redo(ApplicationContext *context)
{
    const Property::Type type{m_newProperty.type()};

    QRectF dirtyRegion;
    for (const auto &item : std::as_const(m_items)) {
        if (item->hasProperty(type)) {
            m_properties[item] = item->property(type);
            item->setProperty(type, m_newProperty);
            item->setDirty(true);
            dirtyRegion |= item->boundingBox();
        }
    }

    const QRect gridDirtyRegion{context->spatialContext()->coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->renderingContext()->cacheGrid().markDirty(gridDirtyRegion);
}

void UpdatePropertyCommand::undo(ApplicationContext *context)
{
    const Property::Type type{m_newProperty.type()};

    QRectF dirtyRegion{};
    for (const auto &item : std::as_const(m_items)) {
        if (item->hasProperty(type)) {
            item->setProperty(type, m_properties[item]);
            item->setDirty(true);
            dirtyRegion |= item->boundingBox();
        }
    }

    const QRect gridDirtyRegion{context->spatialContext()->coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->renderingContext()->cacheGrid().markDirty(gridDirtyRegion);
}

QString UpdatePropertyCommand::text() const
{
    return m_newProperty.information(m_newProperty.type());
}
