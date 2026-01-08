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

    [[nodiscard]] bool intersects(const QRectF &rect) override;
    [[nodiscard]] bool intersects(const QLineF &rect) override;

    [[nodiscard]] Item::Type type() const override;

    [[nodiscard]] QJsonObject serialize() const override;

private:
    void drawItem(QPainter &painter, const QPointF &offset) const override;
};
