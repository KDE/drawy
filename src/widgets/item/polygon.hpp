// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "item.hpp"

class PolygonItem : public Item
{
public:
    PolygonItem();

    virtual void setStart(QPointF start);
    virtual void setEnd(QPointF end);

    void draw(QPainter &painter, const QPointF &offset) override;
    void erase(QPainter &painter, const QPointF &offset) const override;

    void translate(const QPointF &amount) override;

    [[nodiscard]] const QPointF &start() const;
    [[nodiscard]] const QPointF &end() const;

    [[nodiscard]] QJsonObject serialize() const override;

private:
    QPointF m_start{};
    QPointF m_end{};

    void m_updateBoundingBox();
};
