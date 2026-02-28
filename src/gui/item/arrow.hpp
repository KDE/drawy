// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "item/arrowutils.hpp"
#include "item/line.hpp"
#include "libdrawygui_export.h"

class LIBDRAWYGUI_EXPORT ArrowItem : public LineItem
{
public:
    ArrowItem();
    ~ArrowItem() override = default;

    void setStart(QPointF start) override;
    void setEnd(QPointF end) override;

    void commitTransformation() override;

    [[nodiscard]] Item::FormType formType() const override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;

    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] ArrowUtils::ArrowType startArrow() const;
    void setStartArrow(ArrowUtils::ArrowType newStartArrow);

    [[nodiscard]] ArrowUtils::ArrowType endArrow() const;
    void setEndArrow(ArrowUtils::ArrowType newEndArrow);

    [[nodiscard]] bool operator==(const ArrowItem &other) const;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    void calcArrowPoints();
    QPointF m_arrowEndP1;
    QPointF m_arrowEndP2;
    QPointF m_arrowStartP1;
    QPointF m_arrowStartP2;
    ArrowUtils::ArrowType m_startArrow = ArrowUtils::ArrowType::None;
    ArrowUtils::ArrowType m_endArrow = ArrowUtils::ArrowType::Arrow;

    int m_maxArrowSize{15}; // hardcoded for now
};
LIBDRAWYGUI_EXPORT QDebug operator<<(QDebug d, const ArrowItem &t);
