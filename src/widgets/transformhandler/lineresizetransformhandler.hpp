// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "transformhandler.hpp"
#include <QCursor>
#include <QObject>
#include <QPointF>

class LineItem;

class LineResizeTransformHandler : public TransformHandler
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
        Start,
        End,
        None
    };

    Q_ENUM(ResizeHandleType)

    struct ResizeHandle {
        QRectF rect{};
        ResizeHandleType handleType{};
    };

private:
    [[nodiscard]] QList<ResizeHandle> getHandles(const std::shared_ptr<LineItem> &item) const;

    QPointF m_initialPos{};
    QPointF m_lastPos{};

    bool m_isActive{false};

    ResizeHandleType m_curHandleType{};
};
