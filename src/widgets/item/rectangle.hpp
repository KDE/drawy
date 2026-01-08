// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "polygon.hpp"

class RectangleItem : public PolygonItem
{
public:
    RectangleItem();
    ~RectangleItem() override = default;

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    Item::Type type() const override;

    [[nodiscard]] QJsonObject serialize() const override;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;
};
