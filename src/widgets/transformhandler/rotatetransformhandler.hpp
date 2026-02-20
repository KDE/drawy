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

    int m_lastRotationAngle{};

    QCursor m_cursor{};
};
