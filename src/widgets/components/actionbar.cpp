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

void ActionBar::addButton(const QString &tooltip, IconManager::Icon icon)
{
    if (m_map.contains(icon)) {
        throw std::logic_error("Button with same id exists in the ActionBar.");
    }

    ApplicationContext *context{ApplicationContext::instance()};

    m_map[icon] = new QPushButton{this};
    m_map[icon]->setIcon(context->uiContext()->iconManager()->icon(icon));
    m_map[icon]->setToolTip(tooltip);

    m_map[icon]->setProperty("class", u"drawlyActionButton"_s);
    m_map[icon]->setCursor(Qt::PointingHandCursor);
    m_layout->addWidget(m_map[icon]);
}

QPushButton *ActionBar::button(IconManager::Icon id)
{
    if (!m_map.contains(id)) {
        throw std::logic_error(QString(u"Button with id %1 was not found."_s.arg(static_cast<int>(id))).toStdString());
    }

    return m_map[id];
}

#include "moc_actionbar.cpp"
