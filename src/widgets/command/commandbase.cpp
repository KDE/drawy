/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "commandbase.hpp"
#include "drawy_command_debug.h"

CommandBase::CommandBase()
{
}

CommandBase::~CommandBase()
{
    qCDebug(DRAWY_COMMAND_LOG) << "Object deleted: CommandBase";
}
