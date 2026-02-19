#pragma once

#include "movetransformhandler.hpp"
#include <QPointF>

class LineMoveTransformHandler : public MoveTransformHandler
{
public:
    void renderHandles(ApplicationContext *context) override;
    [[nodiscard]] bool shouldActivate(ApplicationContext *context) override;
};
