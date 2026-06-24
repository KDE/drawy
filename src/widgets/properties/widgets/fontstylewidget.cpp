/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "fontstylewidget.hpp"
#include <QHBoxLayout>
#include <QToolButton>
#include <common/constants.hpp>
#include <item/itemutils.hpp>
#include <klocalizedstring.h>
using namespace Qt::Literals::StringLiterals;

FontStyleWidget::FontStyleWidget(QWidget *parent)
    : PropertyWidget{parent}
{
    m_widget = new QWidget(parent);
    m_widget->setObjectName(u"m_widget"_s);

    auto *layout = new QHBoxLayout(m_widget);
    layout->setObjectName(u"layout"_s);
    layout->setAlignment(Qt::AlignCenter);

    m_bold = new QToolButton(m_widget);
    m_bold->setObjectName("m_bold");
    m_bold->setCheckable(true);
    m_bold->setIcon(QIcon::fromTheme(u"format-text-bold"_s));
    m_bold->setToolTip(i18nc("@info:tooltip", "Bold"));
    m_bold->setIconSize(Common::fontStyleIconSize);
    m_bold->setAutoRaise(true);
    layout->addWidget(m_bold);

    m_italic = new QToolButton(m_widget);
    m_italic->setObjectName("m_italic");
    m_italic->setCheckable(true);
    m_italic->setIcon(QIcon::fromTheme(u"format-text-italic"_s));
    m_italic->setToolTip(i18nc("@info:tooltip", "Italic"));
    m_italic->setIconSize(Common::fontStyleIconSize);
    m_italic->setAutoRaise(true);
    layout->addWidget(m_italic);

    m_underlined = new QToolButton(m_widget);
    m_underlined->setObjectName("m_underlined");
    m_underlined->setCheckable(true);
    m_underlined->setIcon(QIcon::fromTheme(u"format-text-underline"_s));
    m_underlined->setToolTip(i18nc("@info:tooltip", "Underlined"));
    m_underlined->setIconSize(Common::fontStyleIconSize);
    m_underlined->setAutoRaise(true);
    layout->addWidget(m_underlined);

    connect(m_bold, &QToolButton::clicked, this, [this]() {
        Q_EMIT changed(value());
    });
    connect(m_italic, &QToolButton::clicked, this, [this]() {
        Q_EMIT changed(value());
    });
    connect(m_underlined, &QToolButton::clicked, this, [this]() {
        Q_EMIT changed(value());
    });

    m_widget->hide();
}

QString FontStyleWidget::name() const
{
    return i18n("Font Style");
}

Property FontStyleWidget::value() const
{
    int value{0};
    if (m_bold->isChecked()) {
        value |= Property::Bold;
    }
    if (m_italic->isChecked()) {
        value |= Property::Italic;
    }
    if (m_underlined->isChecked()) {
        value |= Property::Underlined;
    }
    return Property{ItemUtils::convertFontStyleToString(value), Property::Type::FontStyle};
}

void FontStyleWidget::setValue(const QVariant &val)
{
    const QSignalBlocker blocker1(m_bold);
    const QSignalBlocker blocker2(m_italic);
    const QSignalBlocker blocker3(m_underlined);

    const int value = ItemUtils::convertStringToFontStyle(val.toString());
    m_bold->setChecked(value & Property::Bold);
    m_italic->setChecked(value & Property::Italic);
    m_underlined->setChecked(value & Property::Underlined);
}
