// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "polygon.hpp"

class ArrowItem : public PolygonItem
{
public:
    ArrowItem();
    ~ArrowItem() override = default;

    void setStart(QPointF start) override;
    void setEnd(QPointF end) override;

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    void translate(const QPointF &amount) override;

    Item::Type type() const override;

protected:
    void m_draw(QPainter &painter, const QPointF &offset) const override;

private:
    QPointF m_arrowP1;
    QPointF m_arrowP2;

    int m_maxArrowSize{15}; // hardcoded for now

    void calcArrowPoints();
};
