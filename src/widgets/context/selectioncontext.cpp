// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selectioncontext.hpp"

#include <cmath>
#include <utility>

#include "applicationcontext.hpp"
#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/updatepropertycommand.hpp"
#include "common/utils/math.hpp"
#include "drawy_debug.h"
#include "item/item.hpp"
#include "properties/property.hpp"
#include "renderingcontext.hpp"
#include "spatialcontext.hpp"

SelectionContext::SelectionContext(ApplicationContext *context)
    : QObject{context}
    , m_applicationContext{context}
{
}

SelectionContext::~SelectionContext()
{
    qCDebug(DRAWY_LOG) << "Object deleted: SelectionContext";
}

const std::unordered_set<std::shared_ptr<Item>> &SelectionContext::selectedItems() const
{
    return m_selectedItems;
}

void SelectionContext::addToSelection(const std::shared_ptr<Item> &item)
{
    if (m_selectedItems.contains(item)) {
        return;
    }

    m_selectedItems.insert(item);
    Q_EMIT selectionUpdated();
}

void SelectionContext::removeFromSelection(const std::shared_ptr<Item> &item)
{
    if (!m_selectedItems.contains(item)) {
        return;
    }

    m_selectedItems.erase(item);
    Q_EMIT selectionUpdated();
}

QPolygonF SelectionContext::selectionBox() const
{
    if (m_selectedItems.empty()) {
        return {};
    }

    if (m_selectedItems.size() == 1) {
        return (*m_selectedItems.begin())->displayBoundingBox();
    }

    const auto angleOpt{sharedOrientationAngle()};
    if (angleOpt.has_value()) {
        const qreal theta{*angleOpt};
        if (std::abs(theta) > 1e-4 && std::abs(theta - 2.0 * M_PI) > 1e-4) {
            QTransform unrotateTransform;
            unrotateTransform.rotateRadians(-theta);

            QRectF alignedBox;
            for (const auto &item : std::as_const(m_selectedItems)) {
                alignedBox |= unrotateTransform.map(item->displayBoundingBox()).boundingRect();
            }

            QTransform rotateTransform;
            rotateTransform.rotateRadians(theta);
            return rotateTransform.map(QPolygonF(alignedBox));
        }
    }

    // always return a unified rectangle if there are more items
    QRectF selectionBox;
    for (const auto &item : std::as_const(m_selectedItems)) {
        selectionBox |= item->displayBoundingBox().boundingRect();
    }
    return static_cast<QPolygonF>(selectionBox);
}

std::pair<QRectF, QTransform> SelectionContext::selectionBoxWithTransform() const
{
    if (m_selectedItems.empty()) {
        return {};
    }

    if (m_selectedItems.size() == 1) {
        const auto &item{*m_selectedItems.begin()};
        return std::make_pair(item->normalizedBoundingBox(), item->transformObj());
    }

    const auto angleOpt{sharedOrientationAngle()};
    if (angleOpt.has_value()) {
        const qreal theta{*angleOpt};
        if (std::abs(theta) > 1e-4 && std::abs(theta - 2.0 * M_PI) > 1e-4) {
            QTransform unrotateTransform;
            unrotateTransform.rotateRadians(-theta);

            QRectF alignedBox;
            for (const auto &item : std::as_const(m_selectedItems)) {
                alignedBox |= unrotateTransform.map(item->displayBoundingBox()).boundingRect();
            }

            QTransform rotateTransform;
            rotateTransform.rotateRadians(theta);
            const QTransform transform{QTransform::fromTranslate(alignedBox.topLeft().x(), alignedBox.topLeft().y()) * rotateTransform};

            return std::make_pair(QRectF{0, 0, alignedBox.width(), alignedBox.height()}, transform);
        }
    }

    const QRectF boundingRect{selectionBox().boundingRect()};
    QTransform transform;
    transform.translate(boundingRect.topLeft().x(), boundingRect.topLeft().y());

    return std::make_pair(boundingRect.translated(-boundingRect.topLeft()), transform);
}

bool SelectionContext::hasSameOrientation() const
{
    return sharedOrientationAngle().has_value();
}

std::optional<qreal> SelectionContext::sharedOrientationAngle() const
{
    if (m_selectedItems.empty()) {
        return std::nullopt;
    }

    if (m_selectedItems.size() == 1) {
        return Common::Utils::Math::angle((*m_selectedItems.begin())->transformObj().inverted());
    }

    auto it{m_selectedItems.begin()};
    const qreal targetAngle{Common::Utils::Math::angle((*it)->transformObj().inverted())};
    ++it;

    constexpr qreal epsilon{1e-4};
    for (; it != m_selectedItems.end(); ++it) {
        const qreal curAngle{Common::Utils::Math::angle((*it)->transformObj().inverted())};
        qreal diff{std::abs(curAngle - targetAngle)};
        diff = std::min(diff, 2.0 * M_PI - diff);

        if (diff > epsilon) {
            return std::nullopt;
        }
    }

    return targetAngle;
}

// PUBLIC SLOTS
void SelectionContext::updatePropertyOfSelectedItems(const Property &property)
{
    if (m_selectedItems.empty()) {
        return;
    }

    QList<std::shared_ptr<Item>> items{m_selectedItems.begin(), m_selectedItems.end()};

    bool shouldUpdateProperty{false}; // we must only insert it in the command
                                      // history if the new property is different

    const auto propertyType{property.type()};
    for (const auto &item : items) {
        if (item->hasProperty(propertyType) && item->needsPropertyUpdate(property)) {
            shouldUpdateProperty = true;
            break;
        }
    }

    if (shouldUpdateProperty) {
        bool editingText{false};
        if (items.size() == 1) {
            if (items[0]->formType() == Item::FormType::Text) {
                const auto textItem = std::dynamic_pointer_cast<TextItem>(items[0]);
                if (textItem && textItem->mode() == TextItem::Mode::Edit) {
                    editingText = true;
                }
            }
        }

        if (editingText) {
            // apply directly history is handled by Qtextdocument
            UpdatePropertyCommand(std::move(items), property).redo(m_applicationContext);
        } else {
            const auto command{std::make_shared<UpdatePropertyCommand>(std::move(items), property)};
            const auto commandHistory{m_applicationContext->spatialContext()->commandHistory()};
            commandHistory->push(command);
        }

        m_applicationContext->renderingContext()->canvas()->setFocus(Qt::OtherFocusReason);
        m_applicationContext->renderingContext()->markForRender();
        m_applicationContext->renderingContext()->markForUpdate();
    }
}

void SelectionContext::reset()
{
    if (m_selectedItems.empty()) {
        return;
    }

    m_selectedItems.clear();
    Q_EMIT selectionUpdated();
}

void SelectionContext::renderHandles()
{
    if (!m_shouldRenderHandles) {
        return;
    }

    const auto &items{selectedItems()};

    if (items.empty()) {
        return;
    }

    auto handlerTypes{defaultHandlerTypesForMultiSelection()};
    if (items.size() == 1) {
        handlerTypes = (*items.begin())->transformHandlers();
    }

    // reversing because the first item must be drawn at the top as
    // it has higher priority
    std::reverse(handlerTypes.begin(), handlerTypes.end());

    for (const auto handlerType : std::as_const(handlerTypes)) {
        auto handler{TransformHandler::getHandler(handlerType)};

        handler->renderHandles(m_applicationContext);
    }

    m_applicationContext->renderingContext()->markForUpdate();
}

QList<TransformHandlerUtils::Type> SelectionContext::defaultHandlerTypesForMultiSelection() const
{
    using enum TransformHandlerUtils::Type;
    return {ResizeTransformHandler, MoveTransformHandler, RotateTransformHandler};
}

bool SelectionContext::shouldRenderHandles() const
{
    return m_shouldRenderHandles;
}

void SelectionContext::setShouldRenderHandles(bool value)
{
    m_shouldRenderHandles = value;
}

#include "moc_selectioncontext.cpp"
