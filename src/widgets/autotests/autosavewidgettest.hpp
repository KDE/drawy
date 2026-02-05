#pragma once

#include <QObject>

class AutoSaveWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AutoSaveWidgetTest(QObject *parent = nullptr);
    ~AutoSaveWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
