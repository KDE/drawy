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
    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString commandTitle() const override;
};

void CustomCommand::execute([[maybe_unused]] ApplicationContext *context)
{
}

void CustomCommand::undo([[maybe_unused]] ApplicationContext *context)
{
}

QString CustomCommand::commandTitle() const
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
    QVERIFY(!t.hasRedo());
    QVERIFY(!t.hasUndo());
}

void CommandHistoryTest::shouldTestInsertCommands()
{
    CommandHistory t(nullptr);
    QSignalSpy spy(&t, &CommandHistory::undoRedoChanged);
    t.insert(std::make_shared<CustomCommand>(QList<std::shared_ptr<Item>>()));
    QVERIFY(!t.hasRedo());
    QVERIFY(t.hasUndo());
    QCOMPARE(spy.count(), 1);

    t.insert(std::make_shared<CustomCommand>(QList<std::shared_ptr<Item>>()));
    QVERIFY(!t.hasRedo());
    QVERIFY(t.hasUndo());
    QCOMPARE(spy.count(), 2);
}

void CommandHistoryTest::shouldTestUndoCommands()
{
    CommandHistory t(nullptr);
    QSignalSpy spy(&t, &CommandHistory::undoRedoChanged);
    t.insert(std::make_shared<CustomCommand>(QList<std::shared_ptr<Item>>()));
    QVERIFY(!t.hasRedo());
    QVERIFY(t.hasUndo());
    spy.clear();
    t.undo();

    QCOMPARE(spy.count(), 1);
    QVERIFY(t.hasRedo());
    QVERIFY(!t.hasUndo());

    spy.clear();
    // No stock
    t.undo();

    QCOMPARE(spy.count(), 0);
    QVERIFY(t.hasRedo());
    QVERIFY(!t.hasUndo());
}

void CommandHistoryTest::shouldTestClearCommands()
{
    CommandHistory t(nullptr);
    QSignalSpy spy(&t, &CommandHistory::undoRedoChanged);
    for (int i = 0; i < 10; i++) {
        t.insert(std::make_shared<CustomCommand>(QList<std::shared_ptr<Item>>()));
    }
    QCOMPARE(spy.count(), 10);

    t.undo();
    t.undo();
    spy.clear();

    // We have undo and redo
    QVERIFY(t.hasRedo());
    QVERIFY(t.hasUndo());

    t.clear();
    QCOMPARE(spy.count(), 1);
    QVERIFY(!t.hasRedo());
    QVERIFY(!t.hasUndo());
}
#include "moc_commandhistorytest.cpp"
