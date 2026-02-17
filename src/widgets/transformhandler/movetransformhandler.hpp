#pragma once

#include "transformhandler.hpp"
#include <QPointF>

class MoveTransformHandler : public TransformHandler
{
public:
    void renderHandles(ApplicationContext *context) override;
    [[nodiscard]] bool shouldActivate(const QRectF selectionBox, const QPointF relativeCurPos) override;
    [[nodiscard]] TransformHandler::State mousePressed(ApplicationContext *context) override;
    [[nodiscard]] TransformHandler::State mouseMoved(ApplicationContext *context) override;
    [[nodiscard]] TransformHandler::State mouseReleased(ApplicationContext *context) override;

private:
    QPointF m_lastPos{};
    QPointF m_initialPos{};
    bool m_isActive{false};
};
