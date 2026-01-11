// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "text.hpp"

#include <QFontMetricsF>
#include <QJsonObject>
#include <utility>

#include "common/constants.hpp"
#include "common/utils/math.hpp"
#include "serializer/textdeserializer.hpp"
#include "serializer/textserializer.hpp"

/*
 * TODO: The current implementation is not optimal. A single character insertion
 * costs n operations on average, making it O(n) per character. Use a better
 * data structure like Rope or Gap buffer, although it may be overkill for a
 * simple whiteboard app.
 *
 * TODO: This file needs some refactoring as well, feel free to open a PR
 */
using namespace Qt::Literals::StringLiterals;
TextItem::TextItem()
    : m_selectionStart(INVALID)
    , m_selectionEnd(INVALID)
{
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::white), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{255, Property::Type::Opacity};
    m_properties[Property::Type::FontSize] = Property{18, Property::Type::FontSize};
}

TextItem::~TextItem()
{
}

void TextItem::createTextBox(const QPointF position)
{
    m_boundingBox.setTopLeft(position);
    m_boundingBox.setWidth(Common::defaultTextBoxWidth);

    const QFontMetricsF metrics{getFont()};
    m_boundingBox.setHeight(metrics.height());

    setDirty(true);
}

bool TextItem::intersects(const QRectF &rect)
{
    return m_boundingBox.intersects(rect);
}

bool TextItem::intersects(const QLineF &line)
{
    return Common::Utils::Math::intersects(m_boundingBox, line);
}

void TextItem::draw(QPainter &painter, const QPointF &offset)
{
    painter.save();

    const QRectF curBox{m_boundingBox.translated(-offset)};
    qsizetype cur{caret()};

    if (mode() == Mode::Edit) {
        // Drawing the caret
        // PERF: There is no need to scan the entire text just to place the caret
        // This can be a lot more efficient, so feel free to open a PR
        auto [start, end] = getLineRange(cur);
        const QString &curLine{m_text.mid(start, cur - start)};

        int lineCount{0};
        for (int pos{0}; pos < cur; pos++) {
            if (m_text[pos] == u'\n')
                lineCount++;
        }

        QFontMetrics metrics{getFont()};

        int width{metrics.size(getTextFlags(), curLine).width()};
        int lineHeight{metrics.height()};

        QPointF caretTop{curBox.topLeft().x() + width, curBox.topLeft().y() + lineHeight * lineCount};

        QPointF caretBottom{caretTop.x(), caretTop.y() + lineHeight};

        QLineF line{caretTop, caretBottom};

        painter.setPen(getPen());
        painter.drawLine(caretTop, caretBottom);

        // Drawing selection
        // PERF: Can be optimized but.. me lazy
        if (hasSelection()) {
            qsizetype selStart = qMin(selectionStart(), selectionEnd());
            qsizetype selEnd = qMax(selectionStart(), selectionEnd());

            painter.setBrush(Common::selectionBackgroundColor);
            painter.setPen(Qt::NoPen);

            qsizetype currentLineStartPos = 0;
            int lineIndex = 0;

            for (qsizetype pos{0}; pos <= m_text.length(); pos++) {
                if (pos == m_text.length() || m_text[pos] == u'\n') {
                    qsizetype currentLineEndPos = pos;

                    qsizetype selectionRectStart = qMax(selStart, currentLineStartPos);
                    qsizetype selectionRectEnd = qMin(selEnd, currentLineEndPos);

                    if (selectionRectStart < selectionRectEnd) {
                        const QString linePrefix = m_text.mid(currentLineStartPos, selectionRectStart - currentLineStartPos);
                        const QString selectedTextOnLine = m_text.mid(selectionRectStart, selectionRectEnd - selectionRectStart);

                        const int prefixWidth = metrics.size(getTextFlags(), linePrefix).width();
                        const int selectionWidth = metrics.size(getTextFlags(), selectedTextOnLine).width();

                        const qreal x{curBox.left() + prefixWidth};
                        const qreal y{curBox.top() + (lineIndex * lineHeight)};

                        QRectF selectionRect(x, y, selectionWidth, lineHeight);
                        painter.drawRect(selectionRect);
                    }

                    currentLineStartPos = pos + 1;
                    lineIndex++;
                }
            }
        }
    }

    painter.setFont(getFont());
    painter.setPen(getPen());
    painter.drawText(curBox, getTextFlags(), m_text);
    painter.restore();
}

void TextItem::translate(const QPointF &amount)
{
    m_boundingBox.translate(amount);
}

void TextItem::drawItem([[maybe_unused]] QPainter &painter, [[maybe_unused]] const QPointF &offset) const
{
}

TextItem::Mode TextItem::mode() const
{
    return m_mode;
}

void TextItem::setMode(Mode mode)
{
    m_mode = mode;
    setDirty(true);
}

qsizetype TextItem::caret() const
{
    return m_caretIndex;
}

qsizetype TextItem::caretPosInLine() const
{
    return m_caretPosInLine;
}

void TextItem::setCaret(qsizetype index, bool updatePosInLine)
{
    if (index < 0 || index > m_text.size())
        return;

    setDirty(true);
    setSelectionStart(index);
    setSelectionEnd(INVALID);

    m_caretIndex = index;
    if (updatePosInLine) {
        qsizetype firstCharOfCurLine{m_text.lastIndexOf(u'\n', m_caretIndex - 1)};
        m_caretPosInLine = m_caretIndex - firstCharOfCurLine;
    }
}

int TextItem::getLineFromY(double yPos) const
{
    const QFontMetricsF metrics{getFont()};
    const double lineHeight{metrics.height()};

    if (lineHeight <= 0)
        return 0;

    const double distFromTop{std::max(yPos - m_boundingBox.y(), 0.0)};
    return static_cast<int>(std::ceil(distFromTop / lineHeight));
}

qsizetype TextItem::getIndexFromX(double xPos, int lineNumber) const
{
    const QFontMetricsF metrics{getFont()};

    auto [start, end] = getLineRange(lineNumber);
    const QString line{m_text.mid(start, end - start + 1)};

    const double distanceFromLeft{std::max(xPos - m_boundingBox.x(), 0.0)};
    const double lineWidth{metrics.boundingRect(m_boundingBox, getTextFlags(), line).width()};

    if (distanceFromLeft > lineWidth) {
        if (end == m_text.size() - 1)
            return m_text.size();
        return end;
    }

    qsizetype low{0}, high{m_text.size()}, index{0};
    while (low <= high) {
        qsizetype mid{low + (high - low) / 2};

        const double prefixWidth{metrics.boundingRect(m_boundingBox, getTextFlags(), line.left(mid)).width()};

        if (prefixWidth <= distanceFromLeft) {
            index = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (index < line.size() - 1) {
        const double widthBefore{metrics.boundingRect(m_boundingBox, getTextFlags(), line.left(index)).width()};
        const double widthAfter{metrics.boundingRect(m_boundingBox, getTextFlags(), line.left(index + 1)).width()};

        const double midPoint{(widthBefore + widthAfter) / 2.0};
        if (distanceFromLeft > midPoint)
            index++;
    }

    return start + index;
}

void TextItem::setCaret(const QPointF &cursorPos)
{
    if (!m_boundingBox.contains(cursorPos))
        return;

    const int lineNumber{getLineFromY(cursorPos.y())};
    const qsizetype index{getIndexFromX(cursorPos.x(), lineNumber)};

    setCaret(index);
}

qsizetype TextItem::selectionStart() const
{
    return m_selectionStart;
}

qsizetype TextItem::selectionEnd() const
{
    return m_selectionEnd;
}

void TextItem::setSelectionStart(qsizetype index)
{
    setDirty(true);
    m_selectionStart = index;
}

void TextItem::setSelectionEnd(qsizetype index)
{
    setDirty(true);
    m_selectionEnd = index;
}

const QString TextItem::selectedText() const
{
    if (!hasSelection())
        return {};

    qsizetype selStart{selectionStart()}, selEnd{selectionEnd()};
    return m_text.mid(std::min(selStart, selEnd), selEnd - selStart + 1);
}

void TextItem::insertText(const QString &text)
{
    if (text.isEmpty())
        return;

    setDirty(true);
    const qsizetype textSize{text.size()};
    const qsizetype cur{caret()};

    m_text.insert(cur, text);
    setCaret(cur + textSize);

    updateBoundingBox();
}

void TextItem::updateBoundingBox()
{
    QFontMetricsF metrics{getFont()};
    QSizeF size{metrics.size(getTextFlags(), m_text)};

    m_boundingBox.setWidth(size.width());
    m_boundingBox.setHeight(size.height());
    setDirty(true);
}

void TextItem::deleteSubStr(qsizetype start, qsizetype end)
{
    if (start < 0 || start >= m_text.size() || end < 0 || end >= m_text.size())
        return;

    if (end < start)
        std::swap(start, end);

    m_text.erase(m_text.begin() + start, m_text.begin() + end + 1);

    QFontMetricsF metrics{getFont()};
    QSizeF size{metrics.size(getTextFlags(), m_text)};

    m_boundingBox.setWidth(std::max(size.width(), Common::defaultTextBoxWidth));
    m_boundingBox.setHeight(size.height());
    setDirty(true);
}

void TextItem::deleteSelection()
{
    if (!hasSelection())
        return;

    qsizetype selStart{selectionStart()}, selEnd{selectionEnd()};
    if (selStart > selEnd)
        std::swap(selStart, selEnd);

    setSelectionStart(TextItem::INVALID);
    setSelectionEnd(TextItem::INVALID);

    deleteSubStr(selStart, selEnd - 1);
    setCaret(selStart);
    setDirty(true);
}

bool TextItem::hasSelection() const
{
    if (selectionStart() == selectionEnd())
        return false;

    return selectionStart() != INVALID && selectionEnd() != INVALID;
}

QFont TextItem::getFont() const
{
    QFont font{};
    font.setPointSize(property(Property::Type::FontSize).value<int>());
    font.setFamily(u"Fuzzy Bubbles"_s);

    return font;
}

QPen TextItem::getPen() const
{
    QPen pen{};

    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    color.setAlpha(property(Property::Type::Opacity).value<int>());
    pen.setColor(color);

    return pen;
}

std::pair<qsizetype, qsizetype> TextItem::getLineRange(int lineNumber) const
{
    const qsizetype len{m_text.length()};

    qsizetype startIndex{0};
    for (qsizetype pos{0}; pos < len; pos++) {
        if (lineNumber == 1)
            break;

        if (m_text[pos] == u'\n') {
            startIndex = pos + 1;
            lineNumber--;
        }
    }

    qsizetype endIndex{m_text.indexOf(u'\n', startIndex)};
    if (endIndex == -1)
        endIndex = len - 1;

    return std::make_pair(startIndex, endIndex);
}

std::pair<qsizetype, qsizetype> TextItem::getLineRange(qsizetype position) const
{
    qsizetype start{m_text.lastIndexOf(u'\n', position - 1)};
    if (start == -1 || position == 0)
        start = 0;

    qsizetype end{m_text.indexOf(u'\n', position)};
    if (end == -1)
        end = m_text.size() - 1;

    return std::make_pair(start, end);
}

qsizetype TextItem::getPrevBreak(qsizetype position) const
{
    auto isBreak = [&](qsizetype pos) {
        for (const auto &sep : Common::wordSeparators) {
            if (m_text[pos] == sep)
                return true;
        }

        return false;
    };

    while (position > 0 && isBreak(position)) {
        position--;
    }

    for (qsizetype pos{position - 1}; pos >= 0; pos--) {
        if (isBreak(pos))
            return pos + 1;
    }

    return 0;
};

qsizetype TextItem::getNextBreak(qsizetype position) const
{
    qsizetype len{m_text.length()};
    for (qsizetype pos{position + 1}; pos < len; pos++) {
        for (auto &sep : Common::wordSeparators) {
            if (m_text[pos] == sep) {
                return pos;
            }
        }
    }

    return len;
};

constexpr int TextItem::getTextFlags()
{
    return Qt::TextExpandTabs;
}

QTextOption TextItem::getTextOptions()
{
    QTextOption options{};
    options.setTabStopDistance(Common::tabStopDistance);

    return options;
}

const QString &TextItem::text() const
{
    return m_text;
}

Item::Type TextItem::type() const
{
    return Item::Type::Text;
}

void TextItem::updateAfterProperty()
{
    updateBoundingBox();
}

QJsonObject TextItem::serialize() const
{
    const TextSerializer serialize(this);
    return serialize.serialize();
}

void TextItem::deserialize(const QJsonObject &obj)
{
    TextDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool TextItem::needsCaching() const
{
    return true;
}

QDebug operator<<(QDebug d, const TextItem &t)
{
    d.space() << "text:" << t.text();
    d.space() << "Item:" << static_cast<const Item &>(t);
    return d;
}
