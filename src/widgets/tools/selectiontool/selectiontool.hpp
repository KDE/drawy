// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"
#include "tools/tool.hpp"
#include "transformhandler/transformhandler.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT SelectionTool : public Tool
{
public:
    explicit SelectionTool(ApplicationContext *context);
    ~SelectionTool() override = default;

    [[nodiscard]] QString tooltip() const override;
    [[nodiscard]] QString icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void keyPressed(ApplicationContext *context) override;

    [[nodiscard]] QList<Property::Type> properties() const override;

    [[nodiscard]] Tool::Type type() const override;

private:
    void updateCurrentHandler(ApplicationContext *context);

    TransformHandler *m_curHandler{};
    TransformHandler::State m_curHandlerState;

    QPointF m_lastPos{};
    bool m_isSelecting{false};
};
