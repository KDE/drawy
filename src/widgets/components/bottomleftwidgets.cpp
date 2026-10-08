// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "bottomleftwidgets.hpp"
#include "clickablelabel.hpp"
#include "context/uicontext.hpp"
#include "frame.hpp"
#include "keybindings/actionmanager.hpp"
#include "page/pagemanager.hpp"
#include "page/pagemodel.hpp"
#include "page/pageswidget.hpp"
#include <KLocalizedString>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
BottomLeftWidgets::BottomLeftWidgets(ActionManager *actionManager, PageManager *pageManager, QWidget *parent)
    : QWidget{parent}
    , m_layout(new QHBoxLayout{this})
    , m_pageManager(pageManager)
    , m_pageModel(new PageModel(pageManager, this))
    , m_pageButton(new QToolButton{this})
    , m_actionManager(actionManager)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_layout->setContentsMargins(0, 0, 0, 0);

    const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    const QSize iconSize{iconWidth, iconWidth};

    // Hamburger menu
    auto hamburgerMenu{new QToolButton{this}};
    hamburgerMenu->setIcon(QIcon::fromTheme(u"application-menu"_s));
    hamburgerMenu->setIconSize(iconSize);

    // Zoom controls
    auto zoomControlFrame{new Frame{this}};
    auto zoomControlLayout{new QHBoxLayout{zoomControlFrame}};

    auto zoomOutButton{new QToolButton{zoomControlFrame}};
    zoomOutButton->setAutoRaise(true);
    zoomOutButton->setIconSize(iconSize);
    zoomOutButton->setDefaultAction(actionManager->action(KStandardActions::ZoomOut));

    auto zoomLabel{new ClickableLabel{zoomControlFrame}};
    zoomLabel->setText(i18nc("@label, percent text", "100%"));
    zoomLabel->setToolTip(i18nc("@info:tooltip", "Reset Zoom"));

    connect(zoomLabel, &ClickableLabel::clicked, this, &BottomLeftWidgets::resetZoom);

    auto zoomInButton{new QToolButton{zoomControlFrame}};
    zoomInButton->setAutoRaise(true);
    zoomInButton->setIconSize(iconSize);
    zoomInButton->setDefaultAction(actionManager->action(KStandardActions::ZoomIn));

    connect(this, &BottomLeftWidgets::zoomFactorChanged, this, [zoomLabel](qreal newZoomFactor) {
        const int zoomValue{qRound(newZoomFactor * 100)};
        zoomLabel->setText(i18nc("@label, percent value", "%1%", zoomValue));
    });

    zoomControlLayout->setSpacing(style()->pixelMetric(QStyle::PM_ToolBarItemMargin));
    zoomControlLayout->addWidget(zoomOutButton);
    zoomControlLayout->addWidget(zoomLabel);
    zoomControlLayout->addWidget(zoomInButton);

    // Page controls
    auto pageControlFrame{new Frame{this}};
    auto pageControlLayout{new QHBoxLayout{pageControlFrame}};

    auto previousPageButton{new QToolButton{pageControlFrame}};
    previousPageButton->setAutoRaise(true);
    previousPageButton->setIconSize(iconSize);
    previousPageButton->setDefaultAction(actionManager->action(ActionManager::Action::PreviousPage));

    m_pageButton->setParent(pageControlFrame);
    m_pageButton->setAutoRaise(true);
    m_pageButton->setIconSize(iconSize);
    m_pageButton->setIcon(QIcon::fromTheme(u"view-list-details"_s));
    m_pageButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_pageButton->setToolTip(i18nc("@info:tooltip", "Manage Pages"));
    connect(m_pageButton, &QToolButton::clicked, this, &BottomLeftWidgets::showPagesPopup);

    auto nextPageButton{new QToolButton{pageControlFrame}};
    nextPageButton->setAutoRaise(true);
    nextPageButton->setIconSize(iconSize);
    nextPageButton->setDefaultAction(actionManager->action(ActionManager::Action::NextPage));

    pageControlLayout->setSpacing(style()->pixelMetric(QStyle::PM_ToolBarItemMargin));
    pageControlLayout->addWidget(previousPageButton);
    pageControlLayout->addWidget(m_pageButton);
    pageControlLayout->addWidget(nextPageButton);

    connect(m_pageModel, &QAbstractItemModel::modelReset, this, &BottomLeftWidgets::updatePageButton);
    connect(m_pageModel, &QAbstractItemModel::dataChanged, this, &BottomLeftWidgets::updatePageButton);
    connect(m_pageModel, &QAbstractItemModel::rowsMoved, this, &BottomLeftWidgets::updatePageButton);
    updatePageButton();

    m_layout->addWidget(zoomControlFrame, 0, Qt::AlignLeft);
    m_layout->addWidget(pageControlFrame, 0, Qt::AlignLeft);
}

void BottomLeftWidgets::updatePageButton()
{
    const int current{m_pageManager->currentPage()};
    const int count{m_pageModel->rowCount()};
    if (current < 0 || current >= count) {
        m_pageButton->setText({});
        return;
    }
    const QString name{m_pageModel->index(current).data(Qt::DisplayRole).toString()};
    m_pageButton->setText(i18nc("@action:button page name (index/count)", "%1 (%2/%3)", name, current + 1, count));
}

void BottomLeftWidgets::showPagesPopup()
{
    if (!m_pagesPopup) {
        auto popup{new QFrame{this, Qt::Popup}};
        popup->setFrameShape(QFrame::StyledPanel);
        m_pagesPopup = popup;
        auto popupLayout{new QVBoxLayout{m_pagesPopup}};
        popupLayout->setContentsMargins({});
        auto pagesWidget{new PagesWidget{m_actionManager, m_pageModel, m_pagesPopup}};
        popupLayout->addWidget(pagesWidget);
        m_pagesPopup->setFocusProxy(pagesWidget);
        m_pagesPopup->resize(250, 300);
    }
    // Open above the button as it is in the bottom of the window
    const QPoint buttonTopLeft{m_pageButton->mapToGlobal(QPoint{0, 0})};
    m_pagesPopup->move(buttonTopLeft.x(), buttonTopLeft.y() - m_pagesPopup->height());
    m_pagesPopup->show();
    m_pagesPopup->setFocus();
}

#include "moc_bottomleftwidgets.cpp"
