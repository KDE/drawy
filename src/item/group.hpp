// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GROUP_H
#define GROUP_H

#include "item.hpp"

class GroupItem : public Item {
public:
    GroupItem() = default;

    void draw(QPainter &painter, const QPointF &offset) override;
    void erase(QPainter &painter, const QPointF &offset) const override;

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    void translate(const QPointF &amount) override;

    void group(const QVector<std::shared_ptr<Item>>& items);
    QVector<std::shared_ptr<Item>> unGroup();

    void setProperty(const Property::Type propertyType, Property newObj) override;
    const Property property(const Property::Type propertyType) const override;
    const QVector<Property> properties() const override;
    const QVector<Property::Type> propertyTypes() const override;

    const QRectF boundingBox() const override;

    Item::Type type() const override;

private:
    QVector<std::shared_ptr<Item>> m_items;

    void m_draw(QPainter &painter, const QPointF &offset) const override;
};

#endif  // GROUP_H
