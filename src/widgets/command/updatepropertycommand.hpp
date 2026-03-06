// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "itemcommand.hpp"
#include "properties/property.hpp"
class ApplicationContext;

class UpdatePropertyCommand : public ItemCommand
{
public:
    UpdatePropertyCommand(QList<std::shared_ptr<Item>> items, Property newProperty);

    void redo(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString text() const override;

    [[nodiscard]] bool verifyDifferentProperty() const;

private:
    Property m_newProperty{};
    std::unordered_map<std::shared_ptr<Item>, Property> m_properties{};
};
