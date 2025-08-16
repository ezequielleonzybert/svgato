#include "svgparser.h"
#include <QRegularExpression>
#include <cmath>

SvgParser::SvgParser(QString filePath, Canvas *canvas)
{
    file = new QFile(filePath);
    file->open(QIODeviceBase::ReadOnly);
    reader = new QXmlStreamReader(file);
    QXmlStreamAttributes a;

    QRegularExpression re("([MmLlHhVvCcSsQqTtAaZz])|(-?\\d*\\.?\\d+)");
    while(!reader->atEnd()){
        reader->readNextStartElement();
        if(reader->name() == "path"){

            QPainterPath path;

            a = reader->attributes();
            QString pathCode = a.value("d").toString();
            mi = re.globalMatch(pathCode);

            while(mi.hasNext()){
                QString captured = nextC();

                if(captured == 'M' || captured == 'm'){
                    float x = nextF();
                    float y = nextF();
                    path.moveTo(x,y);
                }

                if(captured == 'L' || captured == 'l'){
                    float x = nextF();
                    float y = nextF();
                    path.lineTo(x,y);
                }

                if(captured == 'A' || captured == 'a'){
                    float rx = nextF();
                    float ry = nextF();
                    float angle = nextF();
                    int lFlag = nextI();
                    int sFlag = nextI();
                    float x = nextF();
                    float y = nextF();

                    QPointF p = path.currentPosition();
                    ArcQt arc = captured.isUpper() ?
                        svgArcToCenter(p.x(), p.y(), x, y, rx, ry, angle, lFlag, sFlag):
                        svgArcToCenter(p.x(), p.y(), p.x()+x, p.y()+y, rx, ry, angle, lFlag, sFlag);
                    path.arcTo(arc.cx - rx, arc.cy - ry, 2*rx, 2*ry, arc.theta, arc.delta);
                }
            }
            canvas->paths.append(path);
        }
    }
    canvas->update();
}

int SvgParser::nextI(){
    return mi.next().captured(0).toInt();
}

float SvgParser::nextF(){
    return mi.next().captured(0).toFloat();
}

QString SvgParser::nextC(){
    return mi.next().captured(0);
}


constexpr double deg2rad(double deg) { return deg * M_PI / 180.0; }

constexpr double rad2deg(double rad) { return rad * 180.0 / M_PI; }

ArcQt SvgParser::svgArcToCenter(
    double x1, double y1, double x2, double y2,
    double rx, double ry, double phiDeg,
    int fA, int fS)
{
    double phi = deg2rad(phiDeg);

    // --- Step 1: Traslación y rotación ---
    double dx = (x1 - x2) / 2.0;
    double dy = (y1 - y2) / 2.0;

    double x1p =  cos(phi)*dx + sin(phi)*dy;
    double y1p = -sin(phi)*dx + cos(phi)*dy;

    // --- Step 2: Centro en coordenadas primadas ---
    double rx2 = rx*rx;
    double ry2 = ry*ry;
    double x1p2 = x1p*x1p;
    double y1p2 = y1p*y1p;

    double radicand = (rx2*ry2 - rx2*y1p2 - ry2*x1p2) / (rx2*y1p2 + ry2*x1p2);
    // if(radicand < 0) radicand = 0; // corrección por errores numéricos
    double coef = (fA != fS ? 1 : -1) * std::sqrt(radicand);

    double cxp = coef *  ( rx*y1p / ry );
    double cyp = coef * -( ry*x1p / rx );

    // --- Step 3: Calcular centro de elipse ---
    double cx =  cos(phi)*cxp - sin(phi)*cyp + (x1+x2)/2.0;
    double cy =  sin(phi)*cxp + cos(phi)*cyp + (y1+y2)/2.0;

    // --- Step 4: Calcular θ1 ---
    double ux = (x1p - cxp) / rx;
    double uy = (y1p - cyp) / ry;
    double theta = rad2deg(std::atan2(uy, ux));

    // --- Step 4b: Calcular Δθ ---
    double vx = (-x1p - cxp) / rx;
    double vy = (-y1p - cyp) / ry;

    double dot = ux*vx + uy*vy;
    double det = ux*vy - uy*vx;
    double delta = rad2deg(std::atan2(det, dot)) - theta;

    // Ajustar según sweep flag
    if(fS == 0 && delta > 0) delta -= 360.0;
    else if(fS == 1 && delta < 0) delta += 360.0;

    return { cx, cy, theta, -delta };
}

/*ArcQt endpointToCenterArc(
    float x1, float y1, float x2, float y2,
    float rx, float ry, float angle,
    int lFlag, int sFlag)
{
    float rangle = deg2rad(angle);

    float dx = (x1-x2)/2;
    float dy = (y1-y2)/2;
    float xp = cos(rangle)*dx + sin(rangle)*dy;
    float yp = -sin(rangle)*dx + cos(rangle)*dy;

    float rx2 = rx*rx;
    float ry2 = ry*ry;
    float xp2 = xp*xp;
    float yp2 = yp*yp;
    int sign = lFlag != sFlag ? 1 : -1;
    float coef = sign * sqrt( rx2*ry2-rx2*yp2-ry2*xp2 / rx2*yp2+ry2*xp2 );

    float cxp = coef * rx*yp/ry;
    float cyp = coef * -ry*xp/rx;

    float cx = cos(rangle)*cxp - sin(rangle)*cyp + (x1+x2)/2;
    float cy = sin(rangle)*cxp + cos(rangle)*cyp + (y1+y2)/2;

}*/
