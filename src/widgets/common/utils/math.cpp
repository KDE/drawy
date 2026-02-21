#include "math.hpp"
#include <QTransform>
#include <qmath.h>

namespace Common::Utils::Math
{
int orientation(QPointF a, QPointF b, QPointF c)
{
    QPointF ab{b.x() - a.x(), b.y() - a.y()};
    QPointF ac{c.x() - a.x(), c.y() - a.y()};

    int orient{static_cast<int>(ab.x() * ac.y() - ac.x() * ab.y())};
    return (orient == 0 ? 0 : (orient < 0 ? -1 : 1));
}

bool intersects(const QRectF &rectA, const QRectF &rectB)
{
    return rectA.intersects(rectB);
}

bool intersects(const QLineF &a, const QLineF &b)
{
    QPointF p{a.p1()}, q{a.p2()};
    QPointF r{b.p1()}, s{b.p2()};
    return orientation(p, q, r) != orientation(p, q, s) && orientation(r, s, p) != orientation(r, s, q);
}

bool intersects(const QRectF &rect, const QLineF &line)
{
    QLineF left{rect.topLeft(), rect.bottomLeft()};
    QLineF top{rect.topLeft(), rect.topRight()};
    QLineF right{rect.topRight(), rect.bottomRight()};
    QLineF bottom{rect.bottomRight(), rect.bottomLeft()};

    return (intersects(line, left) || intersects(line, top) || intersects(line, right) || intersects(line, bottom));
}

bool intersects(const QRectF &rect, const QPointF &point)
{
    return rect.contains(point);
}

qreal length(const QPointF &point)
{
    const qreal x{point.x()};
    const qreal y{point.y()};
    return qSqrt(x * x + y * y);
}

QPointF unitVector(const QPointF &point)
{
    const qreal len{length(point)};
    if (len == 0) {
        return point;
    }

    return QPointF{point.x() / len, point.y() / len};
}

QPointF rotateVector(const QPointF &vector, const qreal angle)
{
    const qreal cosTheta{qCos(angle)};
    const qreal sinTheta{qSin(angle)};

    return QPointF{vector.x() * cosTheta - vector.y() * sinTheta, vector.x() * sinTheta + vector.y() * cosTheta};
}

qreal dotProduct(const QPointF &vectorA, const QPointF &vectorB)
{
    return vectorA.x() * vectorB.x() + vectorA.y() * vectorB.y();
}

qreal angle(const QPointF &vectorA, const QPointF &vectorB)
{
    const qreal dot{dotProduct(vectorA, vectorB)};
    const qreal cross{vectorA.x() * vectorB.y() - vectorA.y() * vectorB.x()};

    qreal theta{qAtan2(cross, dot)};

    if (theta < 0) {
        theta += 2 * M_PI;
    }

    return theta;
}

int crossProduct(const QPointF &vectorA, const QPointF &vectorB)
{
    return ((vectorA.x() * vectorB.y() - vectorB.x() * vectorA.y()) > 0 ? 1 : -1);
}

QPointF lerp(const QPointF &pointA, const QPointF &pointB, const qreal dist)
{
    return (1 - dist) * pointA + dist * pointB;
}

qreal angle(QTransform transform)
{
    const qreal angleRad{qAtan2(transform.m21(), transform.m11())};
    return (angleRad >= 0 ? angleRad : 2 * M_PI + angleRad);
}

std::pair<qreal, qreal> extractScale(QTransform &transform)
{
    qreal m11{transform.m11()};
    qreal m12{transform.m12()};
    qreal m21{transform.m21()};
    qreal m22{transform.m22()};

    const qreal scaleX{qHypot(m11, m12)};
    const qreal scaleY{qHypot(m21, m22)};

    if (scaleX > 0) {
        m11 /= scaleX;
        m12 /= scaleX;
    }

    if (scaleY > 0) {
        m21 /= scaleY;
        m22 /= scaleY;
    }

    transform.setMatrix(m11, m12, 0, m21, m22, 0, transform.dx(), transform.dy(), 1.0);

    return {scaleX, scaleY};
}
};
