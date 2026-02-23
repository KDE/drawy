/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QBuffer>
#include <QPainterPath>

#include "serializer/svgpaintdevice.hpp"

using namespace Qt::StringLiterals;

SvgPaintEngine::SvgPaintEngine(QXmlStreamWriter &writer)
    : QPaintEngine(PainterPaths | Antialiasing | AlphaBlend | PaintOutsidePaintEvent)
    , m_writer(writer)
{
}

SvgPaintEngine::~SvgPaintEngine() = default;

bool SvgPaintEngine::begin([[maybe_unused]] QPaintDevice *device)
{
    setActive(true);
    return true;
}

bool SvgPaintEngine::end()
{
    setActive(false);
    return true;
}

void SvgPaintEngine::drawEllipse(const QRectF &rect)
{
    m_writer.writeStartElement("ellipse");

    m_writer.writeAttribute("cx", QString::number(rect.center().x()));
    m_writer.writeAttribute("cy", QString::number(rect.center().y()));

    m_writer.writeAttribute("rx", QString::number(rect.width() / 2));
    m_writer.writeAttribute("ry", QString::number(rect.height() / 2));

    writeStrokeFill();

    m_writer.writeEndElement();
}

void SvgPaintEngine::drawImage(const QRectF &dest, const QImage &image, [[maybe_unused]] const QRectF &src, [[maybe_unused]] Qt::ImageConversionFlags flags)
{
    // TODO: use src.

    m_writer.writeStartElement("image");
    m_writer.writeAttribute("x", QString::number(dest.x()));
    m_writer.writeAttribute("y", QString::number(dest.y()));
    m_writer.writeAttribute("width", QString::number(dest.width()));
    m_writer.writeAttribute("height", QString::number(dest.height()));
    m_writer.writeAttribute("preserveAspectRatio", "none");

    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QBuffer::ReadWrite);
    image.save(&buffer, "PNG");
    buffer.close();

    m_writer.writeAttribute("xlink:href", u"data:image/png;base64,"_s + QString::fromLocal8Bit(data.toBase64()));
    m_writer.writeEndElement();
}

void SvgPaintEngine::drawPixmap(const QRectF &rect, const QPixmap &pixmap, const QRectF &src)
{
    drawImage(rect, pixmap.toImage(), src, Qt::AutoColor);
}

void SvgPaintEngine::drawPath(const QPainterPath &path)
{
    QString svgPath;

    for (int i = 0; i < path.elementCount(); i++) {
        if (i != 0) {
            svgPath += u' ';
        }

        const QPainterPath::Element element = path.elementAt(i);

        switch (element.type) {
        case QPainterPath::MoveToElement:
            svgPath += u"M %1 %2"_s.arg(element.x).arg(element.y);
            break;
        case QPainterPath::LineToElement:
            svgPath += u"L %1 %2"_s.arg(element.x).arg(element.y);
            break;
        case QPainterPath::CurveToElement:
            // [i] = {c1.x, c1.y}
            // [i+1] = {c2.x, c2.y}
            // [i+2] = {e.x, e.y}
            svgPath += u"C %1 %2, %3 %4, %5 %6"_s.arg(element.x)
                           .arg(element.y)
                           .arg(path.elementAt(i + 1).x)
                           .arg(path.elementAt(i + 1).y)
                           .arg(path.elementAt(i + 2).x)
                           .arg(path.elementAt(i + 2).y);
            i += 2;
            break;
        case QPainterPath::CurveToDataElement:
            Q_UNREACHABLE();
            break;
        }
    }

    m_writer.writeStartElement("path");
    m_writer.writeAttribute("d", svgPath);
    writeStrokeFill();
    m_writer.writeEndElement();
}

void SvgPaintEngine::drawPolygon(const QPointF *points, int pointCount, [[maybe_unused]] QPaintEngine::PolygonDrawMode mode)
{
    // TODO: use mode

    QString svgPoints;

    for (int i = 0; i < pointCount; i++) {
        svgPoints += u"%1,%2 "_s.arg(points[i].x()).arg(points[i].y());
    }
    svgPoints.removeLast();

    m_writer.writeStartElement("polygon");
    m_writer.writeAttribute("points", svgPoints);
    writeStrokeFill();
    m_writer.writeEndElement();
}

void SvgPaintEngine::drawRects(const QRectF *rects, int rectCount)
{
    for (int i = 0; i < rectCount; i++) {
        m_writer.writeStartElement("rect");

        m_writer.writeAttribute("x", QString::number(rects[i].x()));
        m_writer.writeAttribute("y", QString::number(rects[i].y()));

        m_writer.writeAttribute("width", QString::number(rects[i].width()));
        m_writer.writeAttribute("height", QString::number(rects[i].height()));

        writeStrokeFill();

        m_writer.writeEndElement();
    }
}

void SvgPaintEngine::drawLines(const QLineF *lines, int lineCount)
{
    for (int i = 0; i < lineCount; i++) {
        m_writer.writeStartElement("line");

        m_writer.writeAttribute("x1", QString::number(lines[i].x1()));
        m_writer.writeAttribute("y1", QString::number(lines[i].y1()));

        m_writer.writeAttribute("x2", QString::number(lines[i].x2()));
        m_writer.writeAttribute("y2", QString::number(lines[i].y2()));

        writeStrokeFill();

        m_writer.writeEndElement();
    }
}

void SvgPaintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
    QPainterPath path;
    path.addText(p, textItem.font(), textItem.text());
    painter()->fillPath(path, state->pen().color());
}

static const char *qtCapStyleToSvg(Qt::PenCapStyle style)
{
    switch (style) {
    case Qt::FlatCap:
        return "butt";
    case Qt::SquareCap:
        return "square";
    case Qt::RoundCap:
        return "round";
    case Qt::MPenCapStyle:
        break;
    }

    return "butt";
}

static const char *qtJoinStyleToSvg(Qt::PenJoinStyle style)
{
    switch (style) {
    case Qt::MiterJoin:
        return "miter";
    case Qt::BevelJoin:
        return "bevel";
    case Qt::RoundJoin:
        return "round";
    case Qt::SvgMiterJoin:
        return "miter";
    case Qt::MPenJoinStyle:
        break;
    }

    return "miter";
}

void SvgPaintEngine::writeStrokeFill()
{
    if (state->pen().style() == Qt::NoPen) {
        m_writer.writeAttribute("stroke", "none");
    } else {
        m_writer.writeAttribute("stroke", state->pen().color().name());
        m_writer.writeAttribute("stroke-width", QString::number(state->pen().widthF()));
        m_writer.writeAttribute("stroke-linecap", qtCapStyleToSvg(state->pen().capStyle()));
        m_writer.writeAttribute("stroke-linejoin", qtJoinStyleToSvg(state->pen().joinStyle()));
        m_writer.writeAttribute("stroke-opacity", QString::number((state->pen().color().alpha() * 100.0) / 255.0) + u'%');

        if (state->pen().style() != Qt::SolidLine) {
            const QList<qreal> pattern{state->pen().dashPattern()};

            QString dashArray;
            for (int i = 0; i < pattern.size(); i++) {
                if (i != 0) {
                    dashArray += u' ';
                }

                dashArray += QString::number(pattern[i] * state->pen().widthF());
            }

            m_writer.writeAttribute("stroke-dasharray", dashArray);
        }
    }

    if (state->brush().style() == Qt::NoBrush) {
        m_writer.writeAttribute("fill", "none");
    } else {
        m_writer.writeAttribute("fill", state->brush().color().name());
        m_writer.writeAttribute("fill-opacity", QString::number((state->brush().color().alpha() * 100.0) / 255.0) + u'%');
    }
}

SvgPaintDevice::SvgPaintDevice(QXmlStreamWriter &writer, int width, int height)
    : m_engine(writer)
    , m_width(width)
    , m_height(height)
{
}

SvgPaintDevice::~SvgPaintDevice() = default;

int SvgPaintDevice::metric(PaintDeviceMetric metric) const
{
    switch (metric) {
    case QPaintDevice::PdmWidth:
    case QPaintDevice::PdmWidthMM: // ???
        return m_width;
    case QPaintDevice::PdmHeight:
    case QPaintDevice::PdmHeightMM: // ???
        return m_height;
    case QPaintDevice::PdmNumColors:
        return INT_MAX;
    case QPaintDevice::PdmDpiX:
    case QPaintDevice::PdmDpiY:
        return 72;
    case QPaintDevice::PdmDevicePixelRatio:
        return 1;
    case QPaintDevice::PdmDevicePixelRatioScaled:
        return 1 * QPaintDevice::devicePixelRatioFScale();

    case QPaintDevice::PdmDevicePixelRatioF_EncodedA:
    case QPaintDevice::PdmDevicePixelRatioF_EncodedB:
        return QPaintDevice::encodeMetricF(metric, 1);
    default:
        return 0;
    }
}
