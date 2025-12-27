// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "../properties/property.hpp"
#include "itemcommand.hpp"
class ApplicationContext;

class UpdatePropertyCommand : public ItemCommand
{
public:
    UpdatePropertyCommand(QVector<std::shared_ptr<Item>> items, Property newProperty);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

private:
    Property m_newProperty{};
    std::unordered_map<std::shared_ptr<Item>, Property> m_properties{};
};
