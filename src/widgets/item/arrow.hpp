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

    enum class ArrowType : int8_t {
        None,
        Arrow,
        Triangle,
        // TODO add more in the future
    };
    Q_ENUM(ArrowType)

    void setStart(QPointF start) override;
    void setEnd(QPointF end) override;

    [[nodiscard]] bool intersects(const QRectF &rect) override;
    [[nodiscard]] bool intersects(const QLineF &rect) override;

    void translate(const QPointF &amount) override;

    [[nodiscard]] Item::Type type() const override;

    [[nodiscard]] QJsonObject serialize() const override;

    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] ArrowType startArrow() const;
    void setStartArrow(const ArrowType &newStartArrow);

    [[nodiscard]] ArrowType endArrow() const;
    void setEndArrow(const ArrowType &newEndArrow);

    [[nodiscard]] bool operator==(const ArrowItem &other) const;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    void calcArrowPoints();
    QPointF m_arrowEndP1;
    QPointF m_arrowEndP2;
    QPointF m_arrowStartP1;
    QPointF m_arrowStartP2;
    ArrowType m_startArrow = ArrowType::None;
    ArrowType m_endArrow = ArrowType::Arrow;

    int m_maxArrowSize{15}; // hardcoded for now
};
