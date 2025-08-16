#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <QFile>
#include <QXmlStreamReader>
#include <QRegularExpressionMatchIterator>
#include <QPainterPath>
#include "canvas.h"

struct Point {
    int index;
    float x, y;
    Point(){};
    Point(float x, float y, int i=0):
        x(x), y(y), index(i) {}
};
struct Line {
    int index;
    Point p1, p2;
    Line(Point p1, Point p2, int i=0):
        p1(p1), p2(p2), index(i) {}
};
struct Arc{
    int index;
    float rx, ry;
    float angle;
    int largeArcFlag, sweepFlag; // 0 or 1
    float x, y;
    float cx, cy;
    Arc(float rx, float ry, float angle, int lflag, int sflag, float x, float y, int i=0) :
        rx(rx), ry(ry), angle(angle), largeArcFlag(lflag), sweepFlag(sflag), x(x), y(y), index(i) {}
};
struct ArcQt {
    double cx, cy;   // centro
    double theta;   // ángulo inicial en grados
    double delta;    // ángulo barrido en grados
};
struct Path{
    int index;
    QList<Line> lines;
    QList<Arc> arcs;
    Path(QList<Line> lines, QList<Arc> arcs, int i=0):
        lines(lines), arcs(arcs), index(i){}
};

class SvgParser
{
public:
    QFile *file;
    QXmlStreamReader *reader;
    QRegularExpressionMatchIterator mi;

    QList<QPainterPath> paths;

    SvgParser(QString filePath, Canvas *canvas);
    int nextI();
    float nextF();
    QString nextC();

    ArcQt svgArcToCenter(
        double x1, double y1, double x2, double y2,
        double rx, double ry, double phiDeg,
        int fA, int fS);
};

#endif // SVGPARSER_H
