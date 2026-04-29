/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libdrawywidgets_export.h"
#include <KUserFeedback/Provider>

class LIBDRAWYWIDGETS_EXPORT DrawyUserFeedbackProvider : public KUserFeedback::Provider
{
    Q_OBJECT
public:
    explicit DrawyUserFeedbackProvider(QObject *parent = nullptr);
    ~DrawyUserFeedbackProvider() override;
};
