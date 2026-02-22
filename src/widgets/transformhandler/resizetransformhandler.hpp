// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "transformhandler.hpp"
#include <QTransform>

class Item;

class ResizeTransformHandler : public TransformHandler
{
    Q_GADGET
public:
    void renderHandles(ApplicationContext *context) override;

    [[nodiscard]] bool shouldActivate(ApplicationContext *context) override;
    [[nodiscard]] TransformHandler::State mousePressed(ApplicationContext *context) override;
    [[nodiscard]] TransformHandler::State mouseMoved(ApplicationContext *context) override;
    [[nodiscard]] TransformHandler::State mouseReleased(ApplicationContext *context) override;

private:
    enum class ResizeHandleType : int8_t {
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        TopLeft,
        Top
    };

    Q_ENUM(ResizeHandleType)

    struct ResizeHandle {
        QRectF rect{};
        ResizeHandleType type{};
    };

private:
    QCursor cursorForHandle(const double angle) const;
    static QList<ResizeHandle> getHandles(const QRectF selectionBox, const qreal zoom);

    std::tuple<qreal, qreal, QPointF> topRightHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> rightHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> bottomRightHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> bottomHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> bottomLeftHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> leftHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> topLeftHandler(const QRectF prevRect, const QPointF localCurPos);
    std::tuple<qreal, qreal, QPointF> topHandler(const QRectF prevRect, const QPointF localCurPos);

private:
    QPointF m_viewLastPoint{};
    QRectF m_initialSelectionBox{};
    QTransform m_initialSelectionTransform{};

    QTransform m_lastTransformUpdate{};
    QTransform m_lastLockedTransformUpdate{};

    std::unordered_map<std::shared_ptr<Item>, bool> m_aspectRatioLocked{};

    ResizeHandleType m_activeHandleType{};

    bool m_isActive{false};
};
