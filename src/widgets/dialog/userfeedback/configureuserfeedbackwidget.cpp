/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureuserfeedbackwidget.hpp"

#include <QVBoxLayout>

#include "userfeedback/userfeedbackmanager.hpp"
#include <KUserFeedback/FeedbackConfigWidget>
#include <KUserFeedback/Provider>

using namespace Qt::Literals::StringLiterals;
ConfigureUserFeedbackWidget::ConfigureUserFeedbackWidget(QWidget *parent)
    : QWidget(parent)
    , mUserFeedbackWidget(new KUserFeedback::FeedbackConfigWidget(this))
{
    auto userFeedBackLayout = new QVBoxLayout(this);
    userFeedBackLayout->setObjectName(u"userFeedBackLayout"_s);
    userFeedBackLayout->setContentsMargins({});
    mUserFeedbackWidget->setObjectName(u"mUserFeedbackWidget"_s);
    userFeedBackLayout->addWidget(mUserFeedbackWidget);
}

ConfigureUserFeedbackWidget::~ConfigureUserFeedbackWidget() = default;

void ConfigureUserFeedbackWidget::save()
{
    // set current active mode + write back the config for future starts
    UserFeedBackManager::self()->userFeedbackProvider()->setTelemetryMode(mUserFeedbackWidget->telemetryMode());
    UserFeedBackManager::self()->userFeedbackProvider()->setSurveyInterval(mUserFeedbackWidget->surveyInterval());
}

void ConfigureUserFeedbackWidget::load()
{
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
}

void ConfigureUserFeedbackWidget::restoreToDefaults()
{
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
}

#include "moc_configureuserfeedbackwidget.cpp"
