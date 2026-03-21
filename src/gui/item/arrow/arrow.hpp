// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "item/line.hpp"
#include "libdrawygui_export.h"
#include <QPainterPath>
class ArrowHead;

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
    [[nodiscard]] bool intersects(const QRectF &rect) override;

    QPointF getLineStart() const;
    QPointF getLineEnd() const;

    void updateAfterProperty() override;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    std::shared_ptr<ArrowHead> m_startArrowHead;
    std::shared_ptr<ArrowHead> m_endArrowHead;

    void updatePath();

    constexpr static qreal arrowHeadMaxWidth{20};

    QPainterPath m_path{};
};

LIBDRAWYGUI_EXPORT QDebug operator<<(QDebug d, const ArrowItem &t);
