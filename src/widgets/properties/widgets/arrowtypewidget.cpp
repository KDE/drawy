// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrowtypewidget.hpp"
#include "context/uicontext.hpp"
#include "iconmanager/iconmanager.hpp"
#include "item/arrow/arrowhead.hpp"
#include "item/arrow/arrowutils.hpp"

#include <KLocalizedString>
#include <QButtonGroup>
#include <QGridLayout>
#include <QMenu>
#include <QToolButton>
using namespace Qt::StringLiterals;

ArrowTypeWidget::ArrowTypeWidget(ApplicationContext *context, QWidget *parent)
    : PropertyWidget{parent}
    , m_group{new QButtonGroup{parent}}
{
    m_widget = new QWidget{parent};

    auto *layout = new QHBoxLayout{m_widget};
    m_widget->setLayout(layout);

    // Menu
    QMenu *menu = new QMenu{m_widget};

    auto *gridLayout = new QGridLayout{menu};
    m_group = new QButtonGroup{menu};

    menu->setLayout(gridLayout);

    // arrow data required for a button
    struct ArrowData {
        ArrowHead::Type type;
        QString icon;
        QString tooltip;
    };

    // buttons
    const QList<ArrowData> arrowHeads{{ArrowHead::Type::None, u"none-arrow"_s, i18nc("Tooltip for arrow head", "None")},
                                      {ArrowHead::Type::Open, u"open-arrow"_s, i18nc("Tooltip for arrow head", "Open Arrow")},
                                      {ArrowHead::Type::Unfilled, u"unfilled-arrow"_s, i18nc("Tooltip for arrow head", "Triangle")},
                                      {ArrowHead::Type::Filled, u"filled-arrow"_s, i18nc("Tooltip for arrow head", "Filled Triangle")},
                                      {ArrowHead::Type::Circle, u"circle-arrow"_s, i18nc("Tooltip for arrow head", "Circle")},
                                      {ArrowHead::Type::Diamond, u"diamond-arrow"_s, i18nc("Tooltip for arrow head", "Diamond")}};

    constexpr int maxCols{4};
    int gridRow{0};
    int gridCol{0};

    auto iconManager{context->uiContext()->iconManager()};

    for (const auto &head : arrowHeads) {
        auto *btn = new QToolButton{menu};
        btn->setProperty("arrow-head", ArrowUtils::toString(head.type));
        btn->setToolTip(head.tooltip);
        btn->setCheckable(true);
        btn->setProperty("icon-name", head.icon);
        iconManager->setIcon(btn, head.icon);

        m_group->addButton(btn, static_cast<int>(head.type));
        gridLayout->addWidget(btn, gridRow, gridCol);

        if (gridCol + 1 == maxCols) {
            gridCol = 0;
            gridRow++;
        } else {
            gridCol++;
        }
    }

    auto *expandButton = new QToolButton{m_widget};
    expandButton->setMenu(menu);
    expandButton->setPopupMode(QToolButton::InstantPopup);
    expandButton->setToolTip(i18nc("Tooltip for a button to change arrow head", "Select Arrow Head"));

    layout->setContentsMargins({});
    layout->addWidget(expandButton);
    layout->addStretch();

    connect(m_group, &QButtonGroup::idToggled, this, [this, expandButton, iconManager, menu]() -> void {
        iconManager->setIcon(expandButton, m_group->checkedButton()->property("icon-name").value<QString>());
        menu->hide();
        Q_EMIT changed(value());
    });

    m_widget->hide();
}

void ArrowTypeWidget::setValue(const QVariant &value)
{
    const auto stringValue{value.value<QString>()};

    for (auto *button : m_group->buttons()) {
        if (button->property("arrow-head").value<QString>() == stringValue) {
            button->setChecked(true);
            return;
        }
    }
}

#include "moc_arrowtypewidget.cpp"
