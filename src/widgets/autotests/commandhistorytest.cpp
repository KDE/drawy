/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "commandhistorytest.hpp"
#include "command/commandhistory.hpp"
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(CommandHistoryTest)
using namespace Qt::Literals::StringLiterals;
class CustomCommand : public ItemCommand
{
    // Command interface
public:
    CustomCommand(QList<std::shared_ptr<Item>> items)
        : ItemCommand{std::move(items)}
    {
    }
    ~CustomCommand() override = default;
    void redo(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString text() const override;
};

void CustomCommand::redo([[maybe_unused]] ApplicationContext *context)
{
}

void CustomCommand::undo([[maybe_unused]] ApplicationContext *context)
{
}

QString CustomCommand::text() const
{
    return u"title"_s;
}

CommandHistoryTest::CommandHistoryTest(QObject *parent)
    : QObject{parent}
{
}

void CommandHistoryTest::shouldHaveDefaultValues()
{
    const CommandHistory t(nullptr);
    QVERIFY(!t.canRedo());
    QVERIFY(!t.canUndo());
}

void CommandHistoryTest::shouldTestInsertCommands()
{
    CommandHistory t(nullptr);
    QSignalSpy spy(&t, &CommandHistory::undoRedoChanged);
    t.push(std::make_shared<CustomCommand>(QList<std::shared_ptr<Item>>()));
    QVERIFY(!t.canRedo());
    QVERIFY(t.canUndo());
    QCOMPARE(spy.count(), 1);

    t.push(std::make_shared<CustomCommand>(QList<std::shared_ptr<Item>>()));
    QVERIFY(!t.canRedo());
    QVERIFY(t.canUndo());
    QCOMPARE(spy.count(), 2);
}

void CommandHistoryTest::shouldTestUndoCommands()
{
    CommandHistory t(nullptr);
    QSignalSpy spy(&t, &CommandHistory::undoRedoChanged);
    t.push(std::make_shared<CustomCommand>(QList<std::shared_ptr<Item>>()));
    QVERIFY(!t.canRedo());
    QVERIFY(t.canUndo());
    spy.clear();
    t.undo();

    QCOMPARE(spy.count(), 1);
    QVERIFY(t.canRedo());
    QVERIFY(!t.canUndo());

    spy.clear();
    // No stock
    t.undo();

    QCOMPARE(spy.count(), 0);
    QVERIFY(t.canRedo());
    QVERIFY(!t.canUndo());
}

void CommandHistoryTest::shouldTestClearCommands()
{
    CommandHistory t(nullptr);
    QSignalSpy spy(&t, &CommandHistory::undoRedoChanged);
    for (int i = 0; i < 10; i++) {
        t.push(std::make_shared<CustomCommand>(QList<std::shared_ptr<Item>>()));
    }
    QCOMPARE(spy.count(), 10);

    t.undo();
    t.undo();
    spy.clear();

    // We have undo and redo
    QVERIFY(t.canRedo());
    QVERIFY(t.canUndo());

    t.clear();
    QCOMPARE(spy.count(), 1);
    QVERIFY(!t.canRedo());
    QVERIFY(!t.canUndo());
}
#include "moc_commandhistorytest.cpp"
