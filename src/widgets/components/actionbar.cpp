// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionbar.hpp"

using namespace Qt::Literals::StringLiterals;
ActionBar::ActionBar(QWidget *parent)
    : QFrame{parent}
    , m_layout(new QHBoxLayout{this})
{
    m_layout->setSpacing(10);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QPushButton *ActionBar::addButton(const QString &tooltip, const QString &icon)
{
    auto button = new QPushButton{this};
    button->setIcon(QIcon::fromTheme(icon));
    button->setToolTip(tooltip);

    button->setCursor(Qt::PointingHandCursor);
    m_layout->addWidget(button);
    return button;
}

#include "moc_actionbar.cpp"
