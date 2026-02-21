// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
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

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString commandTitle() const override;

private:
    QTransform m_transform{};
    QTransform m_lockedTransform{};
    std::unordered_map<std::shared_ptr<Item>, bool> m_useLocked{};
};
