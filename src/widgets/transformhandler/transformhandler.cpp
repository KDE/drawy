// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "transformhandler.hpp"
#include "linemovetransformhandler.hpp"
#include "lineresizetransformhandler.hpp"
#include "movetransformhandler.hpp"
#include "resizetransformhandler.hpp"
#include "rotatetransformhandler.hpp"

TransformHandler *TransformHandler::getHandler(TransformHandlerUtils::Type handlerType)
{
    switch (handlerType) {
    case TransformHandlerUtils::Type::MoveTransformHandler: {
        static MoveTransformHandler s_moveHandler{};
        return &s_moveHandler;
    }
    case TransformHandlerUtils::Type::RotateTransformHandler: {
        static RotateTransformHandler s_rotateHandler{};
        return &s_rotateHandler;
    }
    case TransformHandlerUtils::Type::ResizeTransformHandler: {
        static ResizeTransformHandler s_resizeHandler{};
        return &s_resizeHandler;
    }
    case TransformHandlerUtils::Type::LineMoveTransformHandler: {
        static LineMoveTransformHandler s_lineMoveHandler{};
        return &s_lineMoveHandler;
    }
    case TransformHandlerUtils::Type::LineResizeTransformHandler: {
        static LineResizeTransformHandler s_lineResizeHandler{};
        return &s_lineResizeHandler;
    }
    }

    return nullptr;
}

void TransformHandler::renderHandles([[maybe_unused]] ApplicationContext *context)
{
    // do nothing by default
}

TransformHandler::State TransformHandler::mousePressed([[maybe_unused]] ApplicationContext *context)
{
    return TransformHandler::State::Unlocked;
}

TransformHandler::State TransformHandler::mouseMoved([[maybe_unused]] ApplicationContext *context)
{
    return TransformHandler::State::Unlocked;
}

TransformHandler::State TransformHandler::mouseReleased([[maybe_unused]] ApplicationContext *context)
{
    return TransformHandler::State::Unlocked;
}
