// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawygui_export.h"

#include "item.hpp"

class LIBDRAWYGUI_EXPORT GroupItem : public Item
{
public:
    GroupItem() = default;

    void draw(QPainter &painter, const QPointF &offset) override;
    void erase(QPainter &painter, const QPointF &offset) const override;

    [[nodiscard]] bool intersects(const QRectF &rect) override;

    void translate(const QPointF &amount) override;
    void commitTransformation() override;
    [[nodiscard]] bool lockAspectRatioWhenResizing() const override;

    void group(const QList<std::shared_ptr<Item>> &items);
    [[nodiscard]] QList<std::shared_ptr<Item>> unGroup();

    void setProperty(const Property::Type propertyType, Property newObj) override;
    [[nodiscard]] Property property(const Property::Type propertyType) const override;
    [[nodiscard]] QList<Property> properties() const override;
    [[nodiscard]] QList<Property::Type> propertyTypes() const override;

    [[nodiscard]] QRectF boundingBox() const override;
    [[nodiscard]] QRectF normalizedBoundingBox() const override;
    [[nodiscard]] QPolygonF displayBoundingBox() const override;

    [[nodiscard]] Item::FormType formType() const override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] bool needsCaching() const override;

    [[nodiscard]] QList<std::shared_ptr<Item>> items() const;

    void setItems(const QList<std::shared_ptr<Item>> &newItems);

private:
    QList<std::shared_ptr<Item>> m_items;

    void drawItem(QPainter &painter, const QPointF &offset) const override;
};
