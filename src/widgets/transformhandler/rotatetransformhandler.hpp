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
    QPointF m_lastPos{};
    QPointF m_centerPos{};
    bool m_isActive{false};

    QCursor m_cursor{};
};
