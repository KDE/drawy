#pragma once

#include "transformhandler.hpp"
#include <QCursor>
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
    enum class ResizeHandleType {
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
    QList<ResizeHandle> getHandles(const std::shared_ptr<LineItem> &item) const;

    QPointF m_lastPos{};
    bool m_isActive{false};

    ResizeHandleType m_curHandleType{};
};
