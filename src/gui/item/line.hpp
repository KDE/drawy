// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawygui_export.h"

#include "polygon.hpp"

class LIBDRAWYGUI_EXPORT LineItem : public Item
{
public:
    LineItem();
    ~LineItem() override = default;

    virtual void initPoints(QPointF start);
    virtual void setStart(QPointF start);
    virtual void setEnd(QPointF end);

    void setEndWithShift(QPointF end);

    void draw(QPainter &painter, const QPointF &offset) override;

    void commitTransformation() override;

    [[nodiscard]] const QPointF &start() const;
    [[nodiscard]] const QPointF &end() const;

    [[nodiscard]] bool intersects(const QRectF &rect) override;

    [[nodiscard]] Item::FormType formType() const override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] QList<TransformHandlerUtils::Type> transformHandlers() const override;

    [[nodiscard]] bool operator==(const LineItem &other) const;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    LIBDRAWYGUI_NO_EXPORT void updateBoundingBox();
    QPointF m_start{};
    QPointF m_end{};
};
