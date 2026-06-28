// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "text.hpp"

#include "common/utils/math.hpp"
#include <QAbstractTextDocumentLayout>
#include <QFontMetricsF>
#include <QJsonObject>
#include <QTextBlock>
#include <QTextLayout>
#include <utility>

#include "common/constants.hpp"
#include "item/itemutils.hpp"
#include "properties/property.hpp"
#include "serializer/textdeserializer.hpp"
#include "serializer/textserializer.hpp"

using namespace Qt::Literals::StringLiterals;
TextItem::TextItem()
    : m_cursor(&m_document)
{
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::white), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{255, Property::Type::Opacity};
    m_properties[Property::Type::FontSize] = Property{18, Property::Type::FontSize};
    m_properties[Property::Type::FontStyle] = Property{u"Normal"_s, Property::Type::FontStyle};
    m_properties[Property::Type::FontFamily] = Property{QStringLiteral("Fuzzy Bubbles"), Property::Type::FontFamily};
    m_properties[Property::Type::TextAlignment] = Property{u"AlignLeft"_s, Property::Type::TextAlignment};

    m_document.setDefaultFont(getFont());
    m_document.setDefaultCursorMoveStyle(Qt::VisualMoveStyle);

    QObject::connect(&m_document, &QTextDocument::contentsChanged, &m_document, [this]() {
        updateBoundingBox();
    });
}

TextItem::~TextItem()
{
}

void TextItem::createTextBox(const QPointF position)
{
    m_boundingBox.setTopLeft(position);
    m_boundingBox.setWidth(Common::defaultTextBoxWidth);

    m_document.setDefaultFont(getFont());
    const QFontMetricsF metrics{getFont()};
    m_boundingBox.setHeight(metrics.height());

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

    QTransform transform{m_transform};
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(transform)};

    if (m_mode == Mode::Normal && qFuzzyCompare(1.0, scaleY) && !qFuzzyCompare(1.0, scaleX)) {
        painter.scale(1.0 / scaleX, 1.0);
        const qreal width = m_wrapWidth > 0 ? m_wrapWidth : m_boundingBox.width();
        const qreal targetWidth = std::max(width * scaleX, minWrapWidth());
        m_document.setTextWidth(targetWidth);
    }

    QAbstractTextDocumentLayout::PaintContext ctx;
    if (m_mode == Mode::Edit && m_cursor.hasSelection()) {
        // draw selection
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = m_cursor;
        selection.format.setBackground(Common::selectionBackgroundColor);
        ctx.selections.append(selection);
    }

    if (m_mode == Mode::Edit) {
        QGuiApplication::inputMethod()->cursorRectangleChanged();
        // draw caret
        const QRectF rect = cursorRect(m_preeditCursorPos);
        if (rect.isValid()) {
            painter.setPen(getPen());
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

    if (m_mode == Mode::Normal && qFuzzyCompare(1.0, scaleY) && !qFuzzyCompare(1.0, scaleX)) {
        const qreal width = m_wrapWidth > 0 ? m_wrapWidth : m_boundingBox.width();
        const qreal targetWidth = std::max(width * scaleX, minWrapWidth());

        m_document.setTextWidth(targetWidth);
        m_boundingBox.setHeight(m_document.size().height());
    }
}

void TextItem::commitTransformation()
{
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(m_transform)};
    const QTransform filtered{scaleX, 0, 0, scaleY, 0, 0};

    if (!qFuzzyCompare(1.0, scaleX)) {
        if (qFuzzyCompare(1.0, scaleY)) {
            const qreal width = m_wrapWidth > 0 ? m_wrapWidth : m_boundingBox.width();
            m_wrapWidth = std::max(width * scaleX, minWrapWidth());
        } else {
            if (m_wrapWidth > 0) {
                m_wrapWidth = std::max(m_wrapWidth * scaleX, minWrapWidth());
            }
        }
    }

    m_boundingBox = filtered.map(m_boundingBox).boundingRect();

    if (!qFuzzyCompare(1.0, scaleY)) {
        const qreal curFontSize{property(Property::Type::FontSize).value<qreal>()};
        const qreal newFontSize{std::max(1.0, curFontSize * scaleY)};
        setProperty(Property::Type::FontSize, Property{newFontSize, Property::Type::FontSize});
    }
    updateBoundingBox();
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

void TextItem::setMode(Mode mode)
{
    m_mode = mode;
    setDirty(true);
}

void TextItem::setCaret(QPointF cursorPos)
{
    if (!m_boundingBox.contains(m_transform.inverted().map(cursorPos))) {
        return;
    }

    m_cursor.setPosition(getIndexFromCursor(cursorPos));
}

qsizetype TextItem::getIndexFromCursor(QPointF cursorPos) const
{
    const QPointF localPos{m_transform.inverted().map(cursorPos)};
    const QPointF docPos = localPos - m_boundingBox.topLeft();

    return m_document.documentLayout()->hitTest(docPos, Qt::FuzzyHit);
}

void TextItem::updateBoundingBox()
{
    m_document.setDefaultTextOption(getTextOptions());

    if (m_wrapWidth > 0) {
        m_document.setTextWidth(m_wrapWidth);
    }

    if (m_document.isEmpty()) {
        QTextCharFormat fmt;
        fmt.setForeground(Item::property(Property::Type::StrokeColor).value<QColor>());

        QFont font;
        font.setPointSize(Item::property(Property::Type::FontSize).value<int>());
        font.setFamily(Item::property(Property::Type::FontFamily).value<QString>());
        const int style = ItemUtils::convertStringToFontStyle(Item::property(Property::Type::FontStyle).value<QString>());
        font.setBold(style & Property::FontStyle::Bold);
        font.setItalic(style & Property::FontStyle::Italic);
        font.setUnderline(style & Property::FontStyle::Underlined);
        fmt.setFont(font);

        m_cursor.setCharFormat(fmt);
    }

    m_boundingBox.setSize(m_document.size());
    setDirty(true);
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
    return font;
}

QPen TextItem::getPen() const
{
    QPen pen;
    pen.setColor(property(Property::Type::StrokeColor).value<QColor>());
    return pen;
}

constexpr int TextItem::getTextFlags()
{
    return Qt::TextExpandTabs;
}

QTextOption TextItem::getTextOptions() const
{
    QTextOption options{};
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

Item::FormType TextItem::formType() const
{
    return Item::FormType::Text;
}

QRectF TextItem::cursorRect(int offset) const
{
    const auto block = m_cursor.block();
    if (const auto *layout = block.layout()) {
        const int pos = m_cursor.positionInBlock() + offset;
        const QTextLine line = layout->lineForTextPosition(pos);
        if (line.isValid()) {
            const qreal x = layout->position().x() + line.cursorToX(pos);
            const qreal y = layout->position().y() + line.y();
            const qreal height = line.height();

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
            range.format = qvariant_cast<QTextCharFormat>(attr.value);
            formats.append(range);
        }
    }
    auto layout = m_cursor.block().layout();
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
        const QColor color = cursor.charFormat().foreground().color();
        return Property{color, Property::Type::StrokeColor};
    }
    case Property::Type::FontSize: {
        const qreal size = cursor.charFormat().font().pointSize();
        return Property{static_cast<int>(size), Property::Type::FontSize};
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
        return Property{ItemUtils::convertFontStyleToString(style), Property::Type::FontStyle};
    }
    default:
        return Item::property(propertyType);
    }
}

void TextItem::setProperty(const Property::Type propertyType, Property newObj)
{
    QTextCharFormat fmt;
    switch (propertyType) {
    case Property::Type::StrokeColor:
        fmt.setForeground(newObj.value<QColor>());
        break;
    case Property::Type::FontSize:
        fmt.setFontPointSize(newObj.value<int>());
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
        break;
    }
    default:
        Item::setProperty(propertyType, newObj);
        return;
    }

    if (m_mode == Mode::Normal) {
        m_cursor.select(QTextCursor::Document);
        m_cursor.mergeCharFormat(fmt);
        m_cursor.clearSelection();
    } else {
        m_cursor.mergeCharFormat(fmt);
    }
    Item::setProperty(propertyType, newObj);
}

QJsonObject TextItem::serialize(int zorder) const
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
    return true;
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

qreal TextItem::minWrapWidth() const
{
    const QFontMetricsF metrics{getFont()};
    return metrics.maxWidth();
}
