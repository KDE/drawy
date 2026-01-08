// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPainter>
#include <QRect>

#include "item.hpp"

class TextItem : public Item
{
public:
    TextItem();
    ~TextItem() override;

    [[nodiscard]] bool intersects(const QRectF &rect) override;
    [[nodiscard]] bool intersects(const QLineF &rect) override;

    void draw(QPainter &painter, const QPointF &offset) override;

    void translate(const QPointF &amount) override;

    void createTextBox(const QPointF position);

    enum class Mode : int8_t {
        Edit = 0,
        Normal
    };

    [[nodiscard]] Mode mode() const;
    void setMode(Mode mode);

    [[nodiscard]] int getLineFromY(double yPos) const;
    [[nodiscard]] qsizetype getIndexFromX(double xPos, int lineNumber) const;

    [[nodiscard]] qsizetype caret() const;
    void setCaret(qsizetype index, bool updatePosInLine = true);
    void setCaret(const QPointF &cursorPos);
    [[nodiscard]] qsizetype caretPosInLine() const;

    [[nodiscard]] qsizetype selectionStart() const;
    [[nodiscard]] qsizetype selectionEnd() const;
    void setSelectionStart(qsizetype index);
    void setSelectionEnd(qsizetype index);
    [[nodiscard]] const QString selectedText() const;

    [[nodiscard]] const QString &text() const;
    void insertText(const QString &text);
    void deleteSubStr(qsizetype start, qsizetype end);
    void deleteSelection();

    [[nodiscard]] bool hasSelection() const;

    [[nodiscard]] std::pair<qsizetype, qsizetype> getLineRange(int lineNumber) const;
    [[nodiscard]] std::pair<qsizetype, qsizetype> getLineRange(qsizetype position) const;

    [[nodiscard]] qsizetype getPrevBreak(qsizetype pos) const;
    [[nodiscard]] qsizetype getNextBreak(qsizetype pos) const;

    [[nodiscard]] Item::Type type() const override;

    constexpr static int INVALID{-1};

    void updateAfterProperty() override;

    [[nodiscard]] QJsonObject serialize() const override;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    [[nodiscard]] QFont getFont() const;
    [[nodiscard]] QPen getPen() const;

    [[nodiscard]] static QTextOption getTextOptions();
    constexpr static int getTextFlags();

    void renderBoundingBox(QPainter &painter) const;
    void updateBoundingBox();

    QString m_text;
    qsizetype m_caretIndex{};
    qsizetype m_selectionStart{};
    qsizetype m_selectionEnd{};
    qsizetype m_caretPosInLine{};
    Mode m_mode{Mode::Normal};
};
