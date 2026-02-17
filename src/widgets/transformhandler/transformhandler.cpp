#include "transformhandler.hpp"
#include "movetransformhandler.hpp"
#include "resizetransformhandler.hpp"
#include "rotatetransformhandler.hpp"

TransformHandler *TransformHandler::getHandler(TransformHandler::Type handlerType)
{
    switch (handlerType) {
    case TransformHandler::Type::MoveTransformHandler: {
        static MoveTransformHandler s_moveHandler{};
        return &s_moveHandler;
    };
    case TransformHandler::Type::RotateTransformHandler: {
        static RotateTransformHandler s_rotateHandler{};
        return &s_rotateHandler;
    };
    case TransformHandler::Type::ResizeTransformHandler: {
        static ResizeTransformHandler s_resizeHandler{};
        return &s_resizeHandler;
    };
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
