/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "aboutdata.hpp"
#include "config-drawy.hpp"
#include <KLocalizedString>
using namespace Qt::Literals::StringLiterals;
AboutData::AboutData()
    : KAboutData(u"drawy"_s,
                 i18n("Drawy"),
                 QStringLiteral(DRAWY_VERSION),
                 i18n("Your handy, infinite  brainstorming tool!"),
                 KAboutLicense::GPL_V3,
                 i18nc("Copyright text, keep the © symbol and the en dash for the year range", "© 2025–%1 Drawy authors", u"2026"_s))
{
    setProductName("drawy"_ba);
    setOrganizationDomain("kde.org"_ba);
}
