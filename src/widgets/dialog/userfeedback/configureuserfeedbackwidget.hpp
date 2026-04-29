/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libdrawywidgets_private_export.h"

namespace KUserFeedback
{
class FeedbackConfigWidget;
}
class LIBDRAWYWIDGETS_TESTS_EXPORT ConfigureUserFeedbackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureUserFeedbackWidget(QWidget *parent = nullptr);
    ~ConfigureUserFeedbackWidget() override;

    void save();
    void load();
    void restoreToDefaults();

private:
    KUserFeedback::FeedbackConfigWidget *const mUserFeedbackWidget;
};
