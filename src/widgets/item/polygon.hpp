// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"

#include "item.hpp"
class QDebug;
class LIBDRAWYWIDGETS_TESTS_EXPORT PolygonItem : public Item
{
public:
    PolygonItem();

    virtual void setStart(QPointF start);
    virtual void setEnd(QPointF end);

    virtual void setEndWithShift(QPointF end);

    void draw(QPainter &painter, const QPointF &offset) override;

    void normalize() override;

    [[nodiscard]] const QPointF &start() const;
    [[nodiscard]] const QPointF &end() const;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] bool operator==(const PolygonItem &other) const;

protected:
    void prepareBackground(QPainter &painter) const;

private:
    QPointF m_start;
    QPointF m_end;

    void updateBoundingBox();
};
LIBDRAWYWIDGETS_EXPORT QDebug operator<<(QDebug d, const PolygonItem &t);
