// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "polygon.hpp"

class LineItem : public PolygonItem {
public:
    LineItem();
    ~LineItem() override = default;

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    Item::Type type() const override;

private:
    void m_draw(QPainter &painter, const QPointF &offset) const override;
};
