#pragma once

class QPainter;
class Event;
class ApplicationContext;

class TransformHandler
{
    // structs
public:
    enum class State {
        Locked,
        Unlocked
    };

    enum class Type {
        MoveTransformHandler,
        ResizeTransformHandler,
        RotateTransformHandler,
    };

    // static members
public:
    /// @brief It returns a singleton instance of the provided handlerType since these handlers are stateless
    static TransformHandler *getHandler(TransformHandler::Type handlerType);

    // virtual members
public:
    virtual ~TransformHandler() = default;

    virtual void renderHandles(ApplicationContext *context);

    [[nodiscard]] virtual bool shouldActivate(const QRectF selectionBox, const QPointF relativeCurPos) = 0;
    [[nodiscard]] virtual TransformHandler::State mousePressed(ApplicationContext *context);
    [[nodiscard]] virtual TransformHandler::State mouseMoved(ApplicationContext *context);
    [[nodiscard]] virtual TransformHandler::State mouseReleased(ApplicationContext *context);
};
