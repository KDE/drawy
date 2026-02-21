// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "transformhandler.hpp"
#include <QCursor>
#include <QPointF>

class RotateTransformHandler : public TransformHandler
{
public:
    RotateTransformHandler();

    [[nodiscard]] bool shouldActivate(ApplicationContext *context) override;
    [[nodiscard]] TransformHandler::State mousePressed(ApplicationContext *context) override;
    [[nodiscard]] TransformHandler::State mouseMoved(ApplicationContext *context) override;
    [[nodiscard]] TransformHandler::State mouseReleased(ApplicationContext *context) override;

private:
    QPointF m_worldLastPos{};
    QPointF m_worldCenterPos{};

    bool m_isActive{false};

    qreal m_lastRotationAngle{};
    qreal m_initialAngle{};

    QCursor m_cursor{};
};
