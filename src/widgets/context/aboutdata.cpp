/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "aboutdata.hpp"
#include "config-drawy.hpp"
using namespace Qt::Literals::StringLiterals;
AboutData::AboutData()
    : KAboutData(u"Drawy"_s,
                 QObject::tr("Drawy"),
                 QStringLiteral(DRAWY_VERSION),
                 QObject::tr("Your handy, infinite  brainstorming tool!"),
                 KAboutLicense::GPL_V3,
                 QObject::tr("(c) 2025-%1 Drawy authors").arg(u"2026"_s))
{
    addAuthor(QObject::tr("Prayag Jain"), QObject::tr("Maintainer"), u"prayagjain2@gmail.com"_s);
    addAuthor(QObject::tr("Laurent Montel"), QObject::tr("Developer"), u"montel@kde.org"_s);
}
