// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "text.hpp"

#include "common/utils/math.hpp"
#include "common/utils/spellcheckhighlighter.hpp"
#include <QAbstractTextDocumentLayout>
#include <QFontMetricsF>
#include <QJsonObject>
#include <QTextBlock>
#include <QTextLayout>
#include <QTextList>
#include <QTextListFormat>
#include <utility>

#include "common/constants.hpp"
#include "item/itemutils.hpp"
#include "properties/property.hpp"
#include "serializer/textdeserializer.hpp"
#include "serializer/textserializer.hpp"

using namespace Qt::Literals::StringLiterals;
TextItem::TextItem()
    : m_highlighter(new SpellCheckHighlighter(&m_document))
    , m_cursor(&m_document)
{
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::white), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{255, Property::Type::Opacity};
    m_properties[Property::Type::FontSize] = Property{18, Property::Type::FontSize};
    m_properties[Property::Type::FontStyle] = Property{u"Normal"_s, Property::Type::FontStyle};
    m_properties[Property::Type::FontFamily] = Property{QStringLiteral("Fuzzy Bubbles"), Property::Type::FontFamily};
    m_properties[Property::Type::TextAlignment] = Property{u"AlignLeft"_s, Property::Type::TextAlignment};
    m_properties[Property::Type::ListStyle] = Property{u"None"_s, Property::Type::ListStyle};

    m_document.setDefaultFont(getFont());

    QTextCharFormat fmt;
    fmt.setForeground(m_properties[Property::Type::StrokeColor].value<QColor>());
    fmt.setFont(getFont());
    m_currentFormat = fmt;
    updateBoundingBox();

    QObject::connect(&m_document, &QTextDocument::contentsChanged, &m_document, [this] {
        m_minWrapWidth = -1;
        updateBoundingBox();
    });
}

TextItem::~TextItem()
{
}

void TextItem::createTextBox(const QPointF position)
{
    m_document.setDefaultFont(getFont());
    updateBoundingBox();
    m_boundingBox.moveTopLeft(QPointF(position.x() - m_document.indentWidth(), position.y()));
    setDirty(true);
}

void TextItem::setBoundingBoxTopLeft(const QPointF &topLeft)
{
    m_boundingBox.moveTopLeft(topLeft);
    setDirty(true);
}

bool TextItem::intersects(const QRectF &rect)
{
    return m_transform.map(m_boundingBox).intersects(rect);
}

void TextItem::draw(QPainter &painter, const QPointF &offset)
{
    painter.save();
    painter.translate(m_boundingBox.topLeft() - offset);
    painter.setOpacity(painter.opacity() * (property(Property::Type::Opacity).value<int>() / 255.0));

    if (m_isHorizontalResize) {
        QTransform transform{m_transform};
        const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(transform)};
        painter.scale(1.0 / scaleX, 1.0);
    }

    QAbstractTextDocumentLayout::PaintContext ctx;
    if (m_mode == Mode::Edit && m_cursor.hasSelection()) {
        // draw selection
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = m_cursor;
        selection.format.setBackground(Common::selectionBackgroundColor);
        selection.format.setFontUnderline(false);
        ctx.selections.append(selection);
    }

    if (m_mode == Mode::Edit) {
        if (m_wrapWidth > 0) {
            QPen pen(Common::selectionBorderColor);
            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawRect(QRectF(0, 0, m_boundingBox.width(), m_boundingBox.height()));
        }

        // draw caret
        const QRectF rect = cursorRect();
        if (rect.isValid()) {
            painter.setPen(property(Property::Type::StrokeColor).value<QColor>());
            painter.drawLine(rect.topLeft(), rect.bottomLeft());
        }
    }
    m_document.documentLayout()->draw(&painter, ctx);
    painter.restore();
}

void TextItem::resize(const QTransform operation)
{
    Item::resize(operation);

    const QSignalBlocker blocker{m_document};
    QTransform transform{m_transform};
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(transform)};

    m_isHorizontalResize = m_mode == Mode::Normal && qFuzzyCompare(1.0, scaleY) && !qFuzzyCompare(1.0, scaleX);

    if (m_isHorizontalResize) {
        const qreal width = m_wrapWidth > 0 ? m_wrapWidth : m_boundingBox.width();
        const qreal targetWidth = std::max(width * scaleX, minWrapWidth());

        if (qRound(m_document.textWidth()) != targetWidth) {
            m_document.setTextWidth(targetWidth);
            m_boundingBox.setHeight(m_document.size().height());
        }
    }
}

void TextItem::commitTransformation()
{
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(m_transform)};
    const QTransform filtered{scaleX, 0, 0, scaleY, 0, 0};

    if (m_isHorizontalResize) {
        const qreal width = m_wrapWidth > 0 ? m_wrapWidth : m_boundingBox.width();
        m_wrapWidth = std::max(width * scaleX, minWrapWidth());
    } else if (m_wrapWidth > 0) {
        m_wrapWidth = std::max(m_wrapWidth * scaleX, minWrapWidth());
    }

    m_boundingBox = filtered.map(m_boundingBox).boundingRect();

    if (!qFuzzyCompare(1.0, scaleY)) {
        const qreal curFontSize{property(Property::Type::FontSize).value<qreal>()};
        scaleTextFragments(scaleY);
        const qreal newFontSize{std::max(1.0, curFontSize * scaleY)};
        Item::setProperty(Property::Type::FontSize, Property{newFontSize, Property::Type::FontSize});
    }
    updateIntendWidth();
    m_isHorizontalResize = false;
}

void TextItem::scaleTextFragments(const qreal scaleY)
{
    const QSignalBlocker blocker{m_document};
    m_cursor.beginEditBlock();
    QTextBlock block = m_document.firstBlock();
    while (block.isValid()) {
        if (block.begin() == block.end()) {
            // empty block -> could be a bullet point with no text
            QTextCharFormat fmt = block.charFormat();
            const qreal size = getFontSize(fmt);
            fmt.setFontPointSize(std::max(1.0, size * scaleY));
            fmt.setProperty(QTextFormat::FontPixelSize, QVariant());
            fmt.setProperty(QTextFormat::FontSizeAdjustment, QVariant());
            m_cursor.setPosition(block.position());
            m_cursor.mergeBlockCharFormat(fmt);
            m_currentFormat.merge(fmt);
        } else {
            for (auto it = block.begin(); !it.atEnd(); ++it) {
                const QTextFragment fragment = it.fragment();
                QTextCharFormat fmt = fragment.charFormat();
                const qreal size = getFontSize(fmt);

                fmt.setFontPointSize(std::max(1.0, size * scaleY));
                fmt.setProperty(QTextFormat::FontPixelSize, QVariant());
                fmt.setProperty(QTextFormat::FontSizeAdjustment, QVariant());

                m_cursor.setPosition(fragment.position());
                m_cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
                m_cursor.mergeBlockCharFormat(fmt);
                m_cursor.mergeCharFormat(fmt);
                m_currentFormat.merge(fmt);
            }
        }
        block = block.next();
    }
    m_cursor.endEditBlock();
    m_minWrapWidth = -1;
}

void TextItem::drawItem([[maybe_unused]] QPainter &painter, [[maybe_unused]] const QPointF &offset) const
{
}

QTextCursor &TextItem::cursor()
{
    return m_cursor;
}

TextItem::Mode TextItem::mode() const
{
    return m_mode;
}

void TextItem::setMode(const Mode mode)
{
    if (m_mode != mode) {
        m_mode = mode;
        if (m_mode == Mode::Normal) {
            m_document.setUndoRedoEnabled(false);
            m_highlighter->setActive(false);
        } else {
            m_document.setUndoRedoEnabled(true);
            m_highlighter->setActive(true);
        }
        setDirty(true);
    }
}

void TextItem::setCaret(const QPointF cursorPos)
{
    if (!m_boundingBox.contains(m_transform.inverted().map(cursorPos))) {
        return;
    }

    m_cursor.setPosition(getIndexFromCursor(cursorPos));
    setDirty(true);
}

qsizetype TextItem::getIndexFromCursor(const QPointF cursorPos) const
{
    const QPointF localPos{m_transform.inverted().map(cursorPos)};
    const QPointF docPos = localPos - m_boundingBox.topLeft();

    return m_document.documentLayout()->hitTest(docPos, Qt::FuzzyHit);
}

QString TextItem::getLink(const QPointF cursorPos) const
{
    const QPointF localPos{m_transform.inverted().map(cursorPos)};
    const QPointF docPos = localPos - m_boundingBox.topLeft();

    return m_document.documentLayout()->anchorAt(docPos);
}

void TextItem::updateBoundingBox()
{
    m_document.setDefaultTextOption(getTextOptions());

    if (m_wrapWidth > 0) {
        m_document.setTextWidth(m_wrapWidth);
    } else {
        m_document.setTextWidth(-1);
        m_document.setTextWidth(m_document.size().width());
    }

    if (m_document.isEmpty()) {
        m_cursor.setCharFormat(m_currentFormat);
        if (m_cursor.blockCharFormat() != m_currentFormat) {
            m_cursor.setBlockCharFormat(m_currentFormat);
        }
    }

    const auto oldSize = m_boundingBox.size();
    const auto newSize = m_document.size();
    if (oldSize != newSize) {
        const qreal diff = newSize.width() - oldSize.width();
        const int alignment = m_cursor.blockFormat().alignment();

        auto topLeft = m_boundingBox.topLeft();
        if (alignment & Qt::AlignRight) {
            topLeft.setX(topLeft.x() - diff);
        } else if (alignment & Qt::AlignCenter) {
            topLeft.setX(topLeft.x() - (diff / 2.0));
        }
        m_boundingBox.moveTopLeft(topLeft);
        m_boundingBox.setSize(newSize);
    }
    setDirty(true);
}

qreal TextItem::getFontSize(const QTextCharFormat &fmt)
{
    qreal size = fmt.fontPointSize();
    if (size <= 0) {
        size = fmt.font().pointSizeF();
    }
    if (size <= 0) {
        size = fmt.font().pixelSize() * 72.0 / 96.0;
    }
    return std::max(size, 1.0);
}

void TextItem::updateIntendWidth()
{
    QTextBlock block = m_document.firstBlock();
    QFont font = block.charFormat().font();
    while (block.isValid()) {
        const qreal size = block.charFormat().fontPointSize();
        if (size > font.pointSize()) {
            font = block.charFormat().font();
        }
        block = block.next();
    }
    const QFontMetricsF metrics(font);
    qreal indentWidth = 0.0;

    const int style = ItemUtils::convertStringToListStyle(property(Property::Type::ListStyle).value<QString>());

    if (style == QTextListFormat::ListDisc || style == QTextListFormat::ListCircle || style == QTextListFormat::ListSquare) {
        indentWidth = metrics.horizontalAdvance(QStringLiteral("■ "));
    } else if (style == QTextListFormat::ListDecimal) {
        indentWidth = metrics.horizontalAdvance(QStringLiteral("99. "));
    } else if (style == QTextListFormat::ListLowerRoman || style == QTextListFormat::ListUpperRoman) {
        indentWidth = metrics.horizontalAdvance(QStringLiteral("VIII. "));
    } else if (style == QTextListFormat::ListLowerAlpha || style == QTextListFormat::ListUpperAlpha) {
        indentWidth = metrics.horizontalAdvance(QStringLiteral("W. "));
    }
    m_document.setIndentWidth(indentWidth);
    updateBoundingBox();
}

QFont TextItem::getFont() const
{
    QFont font;
    font.setPointSize(property(Property::Type::FontSize).value<int>());
    font.setFamily(property(Property::Type::FontFamily).value<QString>());

    const int style = ItemUtils::convertStringToFontStyle(property(Property::Type::FontStyle).value<QString>());
    font.setBold(style & Property::FontStyle::Bold);
    font.setItalic(style & Property::FontStyle::Italic);
    font.setUnderline(style & Property::FontStyle::Underlined);
    font.setStrikeOut(style & Property::FontStyle::StrikeOut);
    return font;
}

QTextOption TextItem::getTextOptions() const
{
    QTextOption options;
    options.setTabStopDistance(Common::tabStopDistance);
    options.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    if (m_properties.contains(Property::Type::TextAlignment)) {
        options.setAlignment(static_cast<Qt::Alignment>(ItemUtils::convertStringToTextAlignment(property(Property::Type::TextAlignment).value<QString>())));
    } else {
        options.setAlignment(Qt::AlignLeft);
    }

    return options;
}

QString TextItem::text() const
{
    return m_document.toPlainText();
}

QString TextItem::html() const
{
    return m_document.toHtml();
}

void TextItem::setHtml(const QString &html)
{
    m_document.setHtml(html);
}

SpellCheckHighlighter *TextItem::highlighter() const
{
    return m_highlighter;
}

Item::FormType TextItem::formType() const
{
    return Item::FormType::Text;
}

QRectF TextItem::cursorRect() const
{
    const auto block = m_cursor.block();
    if (const auto *layout = block.layout()) {
        const int pos = m_cursor.positionInBlock() + m_preeditCursorPos;
        const QTextLine line = layout->lineForTextPosition(pos);
        if (line.isValid()) {
            const QFontMetricsF fm(m_cursor.charFormat().font());
            const qreal x = layout->position().x() + line.cursorToX(pos);
            const qreal y = layout->position().y() + line.y() + line.ascent() - fm.ascent();
            const qreal height = fm.height();

            return QRectF(x, y, 1.0, height);
        }
    }
    return QRectF();
}

void TextItem::updatePreedit(const QString &preedit, const QList<QInputMethodEvent::Attribute> &attributes)
{
    m_preeditString = preedit;
    m_preeditCursorPos = preedit.length();

    QList<QTextLayout::FormatRange> formats;
    for (const auto &attr : attributes) {
        if (attr.type == QInputMethodEvent::Cursor) {
            m_preeditCursorPos = attr.start;
        } else if (attr.type == QInputMethodEvent::TextFormat) {
            QTextLayout::FormatRange range;
            range.start = attr.start + m_cursor.positionInBlock();
            range.length = attr.length;
            range.format = m_cursor.charFormat();
            range.format.merge(qvariant_cast<QTextCharFormat>(attr.value));
            formats.append(range);
        }
    }
    const auto layout = m_cursor.block().layout();
    if (!m_preeditString.isEmpty()) {
        layout->setPreeditArea(m_cursor.positionInBlock(), m_preeditString);
        layout->setFormats(formats);
    } else {
        layout->setPreeditArea(-1, QString());
        layout->clearFormats();
    }
    updateBoundingBox();
}

void TextItem::updateAfterProperty()
{
    updateBoundingBox();
}

Property TextItem::property(const Property::Type propertyType) const
{
    if (m_document.isEmpty()) {
        return Item::property(propertyType);
    }

    QTextCursor cursor = m_cursor;
    if (m_mode == Mode::Normal) {
        cursor.setPosition(0);
    }
    if (cursor.hasSelection()) {
        cursor.setPosition(cursor.selectionStart() + 1);
    }

    switch (propertyType) {
    case Property::Type::StrokeColor: {
        if (!cursor.charFormat().hasProperty(QTextFormat::ForegroundBrush)) {
            return Item::property(Property::Type::StrokeColor);
        }
        const QColor color = cursor.charFormat().foreground().color();
        return Property{color, Property::Type::StrokeColor};
    }
    case Property::Type::FontSize: {
        const qreal size = getFontSize(cursor.charFormat());
        return Property{qRound(size), Property::Type::FontSize};
    }
    case Property::Type::FontFamily: {
        const QString font = cursor.charFormat().font().family();
        return Property{font, Property::Type::FontFamily};
    }
    case Property::Type::FontStyle: {
        const QFont font = cursor.charFormat().font();
        int style = 0;
        if (font.bold()) {
            style |= Property::FontStyle::Bold;
        }
        if (font.italic()) {
            style |= Property::FontStyle::Italic;
        }
        if (font.underline()) {
            style |= Property::FontStyle::Underlined;
        }
        if (font.strikeOut()) {
            style |= Property::FontStyle::StrikeOut;
        }
        return Property{ItemUtils::convertFontStyleToString(style), Property::Type::FontStyle};
    }
    case Property::Type::TextAlignment: {
        const int alignment = cursor.blockFormat().alignment();
        return Property{ItemUtils::convertTextAlignmentToString(alignment), Property::Type::TextAlignment};
    }
    case Property::Type::ListStyle: {
        if (m_cursor.currentList()) {
            return Property{ItemUtils::convertListStyleToString(m_cursor.currentList()->format().style()), Property::Type::ListStyle};
        } else {
            return Property{u"None"_s, Property::Type::ListStyle};
        }
    }
    default:
        return Item::property(propertyType);
    }
}

bool TextItem::needsPropertyUpdate(const Property &property) const
{
    QTextCursor cursor = m_cursor;
    if (m_mode == Mode::Normal) {
        cursor.select(QTextCursor::Document);
    }
    if (!cursor.hasSelection()) {
        return property != this->property(property.type());
    }
    const int start = cursor.selectionStart();
    const int end = cursor.selectionEnd();

    cursor.setPosition(start + 1);
    const QTextCharFormat firstFormat = cursor.charFormat();
    const qreal firstSize = getFontSize(firstFormat);

    QTextBlock block = m_document.findBlock(start);
    while (block.isValid() && block.position() < end) {
        for (auto it = block.begin(); !it.atEnd(); ++it) {
            const QTextFragment fragment = it.fragment();
            const int fragStart = fragment.position();
            const int fragEnd = fragStart + fragment.length();
            if (fragEnd <= start) {
                continue;
            }
            if (fragStart >= end) {
                break;
            }
            const QTextCharFormat fmt = fragment.charFormat();
            switch (property.type()) {
            case Property::Type::StrokeColor:
                if (fmt.foreground().color() != firstFormat.foreground().color()) {
                    return true;
                }
                break;
            case Property::Type::FontSize: {
                if (qRound(getFontSize(fmt)) != qRound(firstSize)) {
                    return true;
                }
                break;
            }
            case Property::Type::FontFamily:
                if (fmt.font().family() != firstFormat.font().family()) {
                    return true;
                }
                break;
            default:
                break;
            }
        }
        block = block.next();
    }
    return property != this->property(property.type());
}

void TextItem::setProperty(const Property::Type propertyType, const Property newObj)
{
    QTextCharFormat fmt;
    switch (propertyType) {
    case Property::Type::StrokeColor:
        fmt.setForeground(newObj.value<QColor>());
        break;
    case Property::Type::FontSize:
        fmt.setFontPointSize(newObj.value<int>());

        // these properties take priority over point size preventing size change
        fmt.setProperty(QTextFormat::FontPixelSize, QVariant());
        fmt.setProperty(QTextFormat::FontSizeAdjustment, QVariant());
        break;
    case Property::Type::FontFamily:
        fmt.setFontFamilies(QStringList{newObj.value<QString>()});
        break;
    case Property::Type::FontStyle: {
        const int newStyle = ItemUtils::convertStringToFontStyle(newObj.value<QString>());
        const int currentStyle = ItemUtils::convertStringToFontStyle(property(Property::Type::FontStyle).value<QString>());
        const int toggledBit = newStyle ^ currentStyle;

        if (toggledBit & Property::FontStyle::Bold) {
            fmt.setFontWeight((newStyle & Property::FontStyle::Bold) ? QFont::Bold : QFont::Normal);
        }
        if (toggledBit & Property::FontStyle::Italic) {
            fmt.setFontItalic(newStyle & Property::FontStyle::Italic);
        }
        if (toggledBit & Property::FontStyle::Underlined) {
            fmt.setFontUnderline(newStyle & Property::FontStyle::Underlined);
        }
        if (toggledBit & Property::FontStyle::StrikeOut) {
            fmt.setFontStrikeOut(newStyle & Property::FontStyle::StrikeOut);
        }
        break;
    }
    case Property::Type::TextAlignment: {
        QTextBlockFormat blockFmt;
        blockFmt.setAlignment(static_cast<Qt::Alignment>(ItemUtils::convertStringToTextAlignment(newObj.value<QString>())));
        QTextCursor cursor = m_cursor;
        cursor.select(QTextCursor::Document);
        cursor.mergeBlockFormat(blockFmt);
        Item::setProperty(propertyType, newObj);
        return;
    }
    case Property::Type::ListStyle: {
        const auto style = static_cast<QTextListFormat::Style>(ItemUtils::convertStringToListStyle(newObj.value<QString>()));
        QTextCursor cursor = m_cursor;
        cursor.select(QTextCursor::Document);
        QTextBlockFormat blockFmt;
        blockFmt.setIndent(0);

        cursor.mergeBlockFormat(blockFmt);
        if (style != QTextListFormat::ListStyleUndefined) {
            QTextListFormat listFmt;
            listFmt.setStyle(style);
            cursor.createList(listFmt);
        } else {
            QTextBlock block = m_document.firstBlock();
            while (block.isValid()) {
                if (QTextList *list = block.textList()) {
                    list->remove(block);
                }
                block = block.next();
            }
        }
        cursor.mergeBlockCharFormat(cursor.charFormat());
        break;
    }
    default:
        Item::setProperty(propertyType, newObj);
        return;
    }

    m_cursor.beginEditBlock();
    if (m_mode == Mode::Normal) {
        m_cursor.select(QTextCursor::Document);
        m_cursor.mergeCharFormat(fmt);
        m_cursor.mergeBlockCharFormat(fmt);
        m_cursor.clearSelection();
    } else {
        m_cursor.mergeCharFormat(fmt);
        if (m_cursor.hasSelection()) {
            m_cursor.mergeBlockCharFormat(fmt);
        }
    }
    m_cursor.endEditBlock();

    m_currentFormat.merge(fmt);
    Item::setProperty(propertyType, newObj);

    updateIntendWidth();
}

QJsonObject TextItem::serialize(const int zorder) const
{
    const TextSerializer serialize(this);
    return serialize.serialize(zorder);
}

void TextItem::deserialize(const QJsonObject &obj)
{
    TextDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool TextItem::needsCaching() const
{
    return !m_isHorizontalResize;
}

QDebug operator<<(QDebug d, const TextItem &t)
{
    d.space() << "text:" << t.text();
    d.space() << "Item:" << static_cast<const Item &>(t);
    return d;
}

bool TextItem::lockAspectRatioWhenResizing() const
{
    return true;
}

qreal TextItem::wrapWidth() const
{
    return m_wrapWidth;
}

void TextItem::setWrapWidth(const qreal wrapWidth)
{
    m_wrapWidth = wrapWidth;
}

qreal TextItem::minWrapWidth() const
{
    if (m_minWrapWidth >= 0) {
        return m_minWrapWidth;
    }

    const std::unique_ptr<QTextDocument> copy(m_document.clone());
    QTextOption options = copy->defaultTextOption();
    options.setWrapMode(QTextOption::WrapAnywhere);
    copy->setDefaultTextOption(options);

    copy->setTextWidth(0);
    m_minWrapWidth = copy->size().width() + 2 * m_document.documentMargin();
    return m_minWrapWidth;
}
