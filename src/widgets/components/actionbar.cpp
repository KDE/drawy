// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionbar.hpp"
#include <QStyle>

using namespace Qt::Literals::StringLiterals;
ActionBar::ActionBar(QWidget *parent)
    : QFrame{parent}
    , m_layout(new QHBoxLayout{this})
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QToolButton *ActionBar::addButton(const QString &tooltip, const QString &icon)
{
    auto button = new QToolButton{this};
    button->setIcon(QIcon::fromTheme(icon));
    button->setToolTip(tooltip);

    button->setCursor(Qt::PointingHandCursor);
    m_layout->addWidget(button);
    return button;
}

#include "moc_actionbar.cpp"
