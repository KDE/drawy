// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPointF>

#include "selectiontool.hpp"
#include "selectiontoolstate.hpp"
class Item;

class SelectionToolResizeState : public SelectionToolState
{
public:
    [[nodiscard]] bool mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    [[nodiscard]] bool mouseReleased(ApplicationContext *context) override;

    void setHandle(SelectionTool::SelectionHandle handle);

    std::tuple<qreal, qreal, QPointF> topRightHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> rightHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> bottomRightHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> bottomHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> bottomLeftHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> leftHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> topLeftHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> topHandler(const QRectF prevRect, const QPointF localCurPos);

private:
    QCursor cursorForHandle(const double angle) const;

    QPointF m_viewLastPoint{};
    SelectionTool::SelectionHandle m_handle{};

    QRectF m_initialSelectionBox{};
    QTransform m_initialSelectionTransform{};
    QHash<std::shared_ptr<Item>, QTransform> m_initialTransform{};
};
