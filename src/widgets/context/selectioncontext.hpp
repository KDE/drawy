// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "transformhandler/transformhandler.hpp"
#include "transformhandlerutils.hpp"
#include <QObject>
#include <unordered_set>

class Property;
class Item;
class ApplicationContext;

class SelectionContext : public QObject
{
    Q_OBJECT
public:
    explicit SelectionContext(ApplicationContext *context);
    ~SelectionContext() override;

    [[nodiscard]] const std::unordered_set<std::shared_ptr<Item>> &selectedItems() const;
    void addToSelection(const std::shared_ptr<Item> &item);

    template<typename Iterator>
    void addToSelection(Iterator begin, Iterator end)
    {
        m_selectedItems.insert(begin, end);
        Q_EMIT selectionUpdated();
    }

    void removeFromSelection(const std::shared_ptr<Item> &item);

    template<typename Iterator>
    void removeFromSelection(Iterator begin, Iterator end)
    {
        for (auto iter = begin; iter != end; iter++) {
            m_selectedItems.erase(*iter);
        }

        Q_EMIT selectionUpdated();
    }

    [[nodiscard]] QPolygonF selectionBox() const;
    [[nodiscard]] std::pair<QRectF, QTransform> selectionBoxWithTransform() const;

    template<typename Iterator>
    void setSelectedItems(Iterator begin, Iterator end)
    {
        std::unordered_set<std::shared_ptr<Item>> newItems{begin, end};

        if (newItems == m_selectedItems)
            return;

        m_selectedItems = newItems;
        Q_EMIT selectionUpdated();
    }

    void reset();
    void updatePropertyOfSelectedItems(const Property &property);

    void renderHandles();
    [[nodiscard]] QList<TransformHandlerUtils::Type> defaultHandlerTypesForMultiSelection() const;

    [[nodiscard]] bool shouldRenderHandles() const;
    void setShouldRenderHandles(bool value);

Q_SIGNALS:
    void selectionUpdated();

private:
    std::unordered_set<std::shared_ptr<Item>> m_selectedItems{};

    bool m_shouldRenderHandles{true};

    ApplicationContext *const m_applicationContext;
};
