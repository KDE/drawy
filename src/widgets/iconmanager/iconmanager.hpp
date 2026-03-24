#pragma once

#include "context/applicationcontext.hpp"
class QAction;
class QAbstractButton;

class IconManager : public QObject
{
public:
    explicit IconManager(ApplicationContext *context);

    void setIcon(QAction *action, const QString &iconName);
    void setIcon(QAbstractButton *action, const QString &iconName);
    void slotUpdateIcons(bool isDarkTheme);

private:
    ApplicationContext *m_context;
    QHash<QAction *, QString> m_actionMap;
    QHash<QAbstractButton *, QString> m_buttonMap;
};
