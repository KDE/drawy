// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class ApplicationContext;

class Command
{
public:
    virtual ~Command() = default;
    virtual void execute(ApplicationContext *context) = 0;
    virtual void undo(ApplicationContext *context) = 0;
    virtual QString commandTitle() const = 0;
};
