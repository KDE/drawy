// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionbar.hpp"

#include <stdexcept>

#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
using namespace Qt::Literals::StringLiterals;
ActionBar::ActionBar(QWidget *parent)
    : QFrame{parent}
    , m_layout(new QHBoxLayout{this})
{
    m_layout->setSpacing(10);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QPushButton *ActionBar::addButton(const QString &tooltip, IconManager::Icon icon)
{
    if (m_map.contains(icon)) {
        throw std::logic_error("Button with same id exists in the ActionBar.");
    }

    ApplicationContext *context{ApplicationContext::instance()};

    auto button = new QPushButton{this};
    button->setIcon(context->uiContext().iconManager().icon(icon));
    button->setToolTip(tooltip);

    button->setProperty("class", u"drawlyActionButton"_s);
    button->setCursor(Qt::PointingHandCursor);
    m_layout->addWidget(button);
    m_map[icon] = button;
    return button;
}

#include "moc_actionbar.cpp"
