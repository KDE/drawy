// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"

#include "polygon.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT LineItem : public PolygonItem
{
public:
    LineItem();
    ~LineItem() override = default;

    void setEndWithShift(QPointF end) override;

    [[nodiscard]] bool intersects(const QRectF &rect) override;

    [[nodiscard]] Item::FormType formType() const override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    QList<TransformHandler::Type> transformHandlers() const override;

private:
    void drawItem(QPainter &painter, const QPointF &offset) const override;
};
