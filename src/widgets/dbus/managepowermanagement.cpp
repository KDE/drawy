/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "managepowermanagement.hpp"
#include "drawy_powermanager_debug.h"
#include <KLocalizedString>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingReply>
#include <QDBusUnixFileDescriptor>

#include <unistd.h>

ManagePowerManagement::ManagePowerManagement(QObject *parent)
    : QObject{parent}
{
}

ManagePowerManagement::~ManagePowerManagement()
{
    allowPowerManagement();
}

void ManagePowerManagement::inhibitPowerManagement()
{
    const QString reason = i18nc(
        "Reason for inhibiting the screensaver activation, when the "
        "presentation mode is active",
        "Giving a presentation");

    if (!mScreenInhibitCookie) {
        QDBusMessage message = QDBusMessage::createMethodCall(QStringLiteral("org.freedesktop.ScreenSaver"),
                                                              QStringLiteral("/ScreenSaver"),
                                                              QStringLiteral("org.freedesktop.ScreenSaver"),
                                                              QStringLiteral("Inhibit"));
        message << QCoreApplication::applicationName();
        message << reason;

        QDBusPendingReply<uint> reply = QDBusConnection::sessionBus().asyncCall(message);
        reply.waitForFinished();
        if (reply.isValid()) {
            mScreenInhibitCookie = reply.value();
            qCDebug(DRAWY_POWERMANAGER_LOG) << "Screen inhibition cookie" << mScreenInhibitCookie;
        } else {
            qCWarning(DRAWY_POWERMANAGER_LOG) << "Unable to inhibit screensaver" << reply.error();
        }
    }

    if (mSleepInhibitFd != -1) {
        QDBusMessage message = QDBusMessage::createMethodCall(QStringLiteral("org.freedesktop.login1"),
                                                              QStringLiteral("/org/freedesktop/login1"),
                                                              QStringLiteral("org.freedesktop.login1.Manager"),
                                                              QStringLiteral("Inhibit"));
        message << QStringLiteral("sleep");
        message << QCoreApplication::applicationName();
        message << reason;
        message << QStringLiteral("block");

        QDBusPendingReply<QDBusUnixFileDescriptor> reply = QDBusConnection::systemBus().asyncCall(message);
        reply.waitForFinished();
        if (reply.isValid()) {
            mSleepInhibitFd = dup(reply.value().fileDescriptor());
        } else {
            qCWarning(DRAWY_POWERMANAGER_LOG) << "Unable to inhibit sleep" << reply.error();
        }
    }
}

void ManagePowerManagement::allowPowerManagement()
{
    if (mSleepInhibitFd != -1) {
        ::close(mSleepInhibitFd);
        mSleepInhibitFd = -1;
    }

    if (mScreenInhibitCookie) {
        QDBusMessage message = QDBusMessage::createMethodCall(QStringLiteral("org.freedesktop.ScreenSaver"),
                                                              QStringLiteral("/ScreenSaver"),
                                                              QStringLiteral("org.freedesktop.ScreenSaver"),
                                                              QStringLiteral("UnInhibit"));
        message << mScreenInhibitCookie;

        QDBusPendingReply<uint> reply = QDBusConnection::sessionBus().asyncCall(message);
        reply.waitForFinished();

        mScreenInhibitCookie = 0;
    }
}

#include "moc_managepowermanagement.cpp"
