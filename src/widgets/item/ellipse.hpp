// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "polygon.hpp"

class EllipseItem : public PolygonItem
{
public:
    EllipseItem();
    ~EllipseItem() override = default;

    [[nodiscard]] bool intersects(const QRectF &rect) override;
    [[nodiscard]] bool intersects(const QLineF &rect) override;

    [[nodiscard]] Item::Type type() const override;

    [[nodiscard]] QJsonObject serialize() const override;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    [[nodiscard]] bool onEllipse(QLineF line) const;
};
