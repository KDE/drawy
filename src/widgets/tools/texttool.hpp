// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "drawingtool.hpp"
#include "event/event.hpp"
#include "item/text.hpp"

class TextTool : public DrawingTool
{
public:
    explicit TextTool(ApplicationContext *context);
    ~TextTool() override = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void keyPressed(ApplicationContext *context) override;
    void keyReleased(ApplicationContext *context) override;
    void mouseDoubleClick(ApplicationContext *context) override;
    void mouseTripleClick(ApplicationContext *context) override;

    void cleanup() override;

    void inputMethodInvoked(ApplicationContext *context) override;
    [[nodiscard]] QVariant inputMethodQueryInvoked(ApplicationContext *context, Qt::InputMethodQuery query) override;

    [[nodiscard]] std::shared_ptr<TextItem> curItem() const;
    [[nodiscard]] Tool::Type type() const override;
    [[nodiscard]] QString icon() const override;

private:
    std::shared_ptr<TextItem> m_curItem{nullptr};

    bool m_isSelecting{false};
    bool m_mouseMoved{false};
    bool m_doubleClicked{false};
    bool m_tripleClicked{false};

    void processKey(const Event *ev) const;
};
