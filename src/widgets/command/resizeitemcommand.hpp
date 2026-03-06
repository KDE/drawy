// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QTransform>

#include "itemcommand.hpp"
class ApplicationContext;

class ResizeItemCommand : public ItemCommand
{
public:
    ResizeItemCommand(QList<std::shared_ptr<Item>> items,
                      const QTransform transform,
                      const QTransform lockedTransform,
                      std::unordered_map<std::shared_ptr<Item>, bool> useLocked);

    void redo(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString text() const override;

private:
    QTransform m_transform{};
    QTransform m_lockedTransform{};
    std::unordered_map<std::shared_ptr<Item>, bool> m_useLocked{};
};
