// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "libdrawygui_export.h"
#include <QPainterPath>
#include <deque>
#include <memory>

#include "item.hpp"

class LIBDRAWYGUI_EXPORT FreeformItem : public Item, public std::enable_shared_from_this<FreeformItem>
{
public:
    FreeformItem();
    ~FreeformItem() override = default;

    void draw(QPainter &painter, const QPointF &offset) override;
    void drawBuffer(QPainter &painter, const QPointF &offset) const;

    [[nodiscard]] qsizetype size() const;
    [[nodiscard]] bool isBufferFull() const;
    [[nodiscard]] Item::FormType formType() const override;
    [[nodiscard]] bool intersects(const QRectF &rect) override;
    [[nodiscard]] bool needsCaching() const override;

    void commitTransformation() override;
    [[nodiscard]] QList<TransformHandlerUtils::Type> transformHandlers() const override;

    void addPoint(const QPointF &point, const qreal pressure);
    void finalizeStroke();
    [[nodiscard]] static int minPointDistance();
    [[nodiscard]] const QList<QPointF> &points() const;
    [[nodiscard]] const QList<qreal> &pressures() const;

    [[nodiscard]] bool isPressureSimulated() const;
    void setSimulatePressure(bool value);

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    void updateAfterProperty() override;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

    [[nodiscard]] bool isSolid() const;
    void drawNonSolidStroke(QPainter &painter, const QPointF &offset, bool drawBuffer = false) const;

    QList<QPointF> m_points;
    QList<qreal> m_pressures;

    static constexpr qsizetype m_maxBufferSize{50};
    QList<QPointF> m_pointBuffer;
    QList<qreal> m_pressureBuffer;

private:
    bool m_simulatePressure{true};
    QPainterPath m_path;
};

LIBDRAWYGUI_EXPORT QDebug operator<<(QDebug d, const FreeformItem &t);
