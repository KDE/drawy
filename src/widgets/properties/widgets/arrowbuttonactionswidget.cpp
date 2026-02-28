/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "arrowbuttonactionswidget.hpp"
#include <QGridLayout>
#include <QMenu>
#include <QStyle>
#include <QToolButton>
#include <QWidgetAction>

ArrowButtonActionsWidget::ArrowButtonActionsWidget(QWidget *parent)
    : QToolButton(parent)
{
    setPopupMode(QToolButton::InstantPopup);
    fillMenu();
    const int iconSize{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    setIconSize(QSize{iconSize, iconSize});
}

ArrowButtonActionsWidget::~ArrowButtonActionsWidget() = default;

void ArrowButtonActionsWidget::fillMenu()
{
    auto menu = new QMenu(this);
    auto arrowWidget = new QWidget(menu);
    auto widgetAction = new QWidgetAction(menu);
    auto gridLayout = new QGridLayout(arrowWidget);
    widgetAction->setDefaultWidget(arrowWidget);
    const int iconSize{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    int row = 0;
    int col = 0;
    for (auto index = static_cast<int>(ArrowUtils::ArrowType::None); index <= static_cast<int>(ArrowUtils::ArrowType::FullTriangle); index++) {
        const auto arrowType = static_cast<ArrowUtils::ArrowType>(index);
        auto toolButton = new QToolButton(arrowWidget);
        toolButton->setToolTip(ArrowUtils::tooltipFromArrowType(arrowType));
        toolButton->setIcon(QIcon::fromTheme(ArrowUtils::iconFromArrowType(arrowType)));
        toolButton->setAutoRaise(true);
        toolButton->setIconSize(QSize{iconSize, iconSize});

        connect(toolButton, &QToolButton::clicked, this, [this, arrowType]() {
            Q_EMIT selectedArrow(arrowType);
        });
        gridLayout->addWidget(toolButton, row, col);
        col++;
        if (col % 4 == 0) {
            row++;
            col = 0;
        }
    }
    menu->addAction(widgetAction);
    setMenu(menu);
}
#include "moc_arrowbuttonactionswidget.cpp"
