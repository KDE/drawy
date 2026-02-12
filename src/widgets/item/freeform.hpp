// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "libdrawywidgets_private_export.h"
#include <QPainterPath>
#include <deque>
#include <memory>

#include "item.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT FreeformItem : public Item, public std::enable_shared_from_this<FreeformItem>
{
public:
    FreeformItem();
    ~FreeformItem() override = default;
    FreeformItem(const FreeformItem &freeform);

    [[nodiscard]] static int minPointDistance();

    void draw(QPainter &painter, const QPointF &offset) override;
    void drawBuffer(QPainter &painter, const QPointF &offset) const;
    [[nodiscard]] bool isBufferFull() const;

    void finalizeStroke();

    [[nodiscard]] bool intersects(const QRectF &rect) override;

    void translate(const QPointF &amount) override;
    void normalize() override;

    [[nodiscard]] qsizetype size() const;

    virtual void addPoint(const QPointF &point, const qreal pressure);

    [[nodiscard]] Item::FormType formType() const override;

    [[nodiscard]] const QList<QPointF> &points() const;
    [[nodiscard]] const QList<qreal> &pressures() const;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    bool needsCaching() const override;

    [[nodiscard]] bool isPressureSimulated() const;
    void setSimulatePressure(bool value);

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

    bool isSolid() const;
    void drawNonSolidStroke(QPainter &painter, const QPointF &offset, bool drawBuffer = false) const;

    QList<QPointF> m_points{};
    QList<qreal> m_pressures{};

    static constexpr qsizetype m_maxBufferSize{50};
    QList<QPointF> m_pointBuffer{};
    QList<qreal> m_pressureBuffer{};

private:
    bool m_simulatePressure{true};
    QPainterPath m_path{};
};

LIBDRAWYWIDGETS_EXPORT QDebug operator<<(QDebug d, const FreeformItem &t);
