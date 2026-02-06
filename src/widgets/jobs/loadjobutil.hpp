/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "jobs/loadjob.hpp"
class ApplicationContext;
namespace LoadJobUtil
{
void loadFile(ApplicationContext *context, const LoadJob::LoadInfo &info);
};
