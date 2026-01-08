// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"
#include "polygon.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT ArrowItem : public PolygonItem
{
public:
    ArrowItem();
    ~ArrowItem() override = default;

    void setStart(QPointF start) override;
    void setEnd(QPointF end) override;

    [[nodiscard]] bool intersects(const QRectF &rect) override;
    [[nodiscard]] bool intersects(const QLineF &rect) override;

    void translate(const QPointF &amount) override;

    [[nodiscard]] Item::Type type() const override;

    [[nodiscard]] QJsonObject serialize() const override;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    QPointF m_arrowP1;
    QPointF m_arrowP2;

    int m_maxArrowSize{15}; // hardcoded for now

    void calcArrowPoints();
};
