// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"

#include "item.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT GroupItem : public Item
{
public:
    GroupItem() = default;

    void draw(QPainter &painter, const QPointF &offset) override;
    void erase(QPainter &painter, const QPointF &offset) const override;

    [[nodiscard]] bool intersects(const QRectF &rect) override;
    [[nodiscard]] bool intersects(const QLineF &rect) override;

    void translate(const QPointF &amount) override;

    void group(const QVector<std::shared_ptr<Item>> &items);
    [[nodiscard]] QVector<std::shared_ptr<Item>> unGroup();

    void setProperty(const Property::Type propertyType, Property newObj) override;
    [[nodiscard]] const Property property(const Property::Type propertyType) const override;
    [[nodiscard]] const QVector<Property> properties() const override;
    [[nodiscard]] const QVector<Property::Type> propertyTypes() const override;

    [[nodiscard]] const QRectF boundingBox() const override;

    [[nodiscard]] Item::Type type() const override;

    [[nodiscard]] QJsonObject serialize() const override;
    void deserialize(const QJsonObject &obj) override;

private:
    QVector<std::shared_ptr<Item>> m_items;

    void drawItem(QPainter &painter, const QPointF &offset) const override;
};
