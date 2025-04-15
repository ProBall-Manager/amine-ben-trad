#include "zonemap.h"
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QRadialGradient>

const int SCENE_WIDTH = 1000;
const int SCENE_HEIGHT = 700;

ZoneMap::ZoneMap(QWidget *parent) : QGraphicsView(parent),
    m_totalCapacity(0), m_highlightedZone(GENERAL_SEATING)
{
    auto* scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, this);
    setScene(scene);
    m_font.setPointSize(10);
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing);
    setMinimumSize(800, 600);
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void ZoneMap::setStadiumInfo(const QString& name, int capacity)
{
    m_stadiumName = name;
    m_totalCapacity = capacity;
    calculateZones();
    update();
}

void ZoneMap::addZone(ZoneType type, const QRect& area, const QString& name, int capacity)
{
    Zone zone;
    zone.area = area;
    zone.type = type;
    zone.name = name;
    zone.color = getZoneColor(type);
    zone.capacity = capacity;
    zone.occupied = 0;
    m_zones.insert(type, zone);
}

QColor ZoneMap::getZoneColor(ZoneType type) const
{
    static const QColor colors[] = {
        QColor(70,130,180), QColor(100,200,100), QColor(0,180,0), QColor(255,215,0),
        QColor(150,150,255), QColor(255,50,50), QColor(255,150,50), QColor(100,255,100),
        QColor(150,150,150), QColor(220,220,100), QColor(255,165,0), QColor(220,20,60),
        QColor(200,200,200), QColor(255,255,255)
    };
    return colors[type];
}

void ZoneMap::calculateZones()
{
    m_zones.clear();
    if (m_totalCapacity <= 0) return;

    int w = SCENE_WIDTH;
    int h = SCENE_HEIGHT;

    QRect pitchRect(w * 0.2, h * 0.2, w * 0.6, h * 0.6);
    addZone(PITCH, pitchRect, "Terrain", 0);

    addZone(NORTH_STAND, QRect(pitchRect.left(), 0, pitchRect.width(), h * 0.1), "TRIBUNE NORD", 0);
    addZone(GENERAL_SEATING, QRect(0, pitchRect.top(), w * 0.15, pitchRect.height()), "PLACES GÉNÉRALES", m_totalCapacity * 0.7);
    addZone(VIP, QRect(pitchRect.right() + 10, pitchRect.top() + h * 0.05, w * 0.15, h * 0.08), "ZONE VIP", m_totalCapacity * 0.1);
    addZone(MEDIA, QRect(pitchRect.right() + 10, pitchRect.top(), w * 0.15, h * 0.05), "MEDIA", 24);
    addZone(FAMILY, QRect(pitchRect.left() + pitchRect.width() / 4, pitchRect.bottom() + 10, w * 0.1, h * 0.08), "FAMILLE", m_totalCapacity * 0.05);
    addZone(AWAY_FANS, QRect(pitchRect.left() + pitchRect.width() / 2, pitchRect.bottom() + 10, w * 0.1, h * 0.08), "SUPPORTERS", m_totalCapacity * 0.05);

    addZone(FIRST_AID, QRect(pitchRect.right() + 10, pitchRect.bottom() - h * 0.15, w * 0.1, h * 0.05), "SECOURS", 0);
    addZone(SHOP, QRect(pitchRect.right() + 10, pitchRect.bottom() - h * 0.1, w * 0.1, h * 0.05), "BOUTIQUE", 0);
    addZone(ENTRANCE, QRect(w * 0.1, h - h * 0.08, w * 0.2, h * 0.05), "ENTRÉES", 0);
    addZone(PARKING, QRect(w * 0.35, h - h * 0.08, w * 0.3, h * 0.05), "PARKING", 0);
    addZone(TICKETING, QRect(w * 0.7, h - h * 0.08, w * 0.2, h * 0.05), "BILLETTERIE", 0);

    addZone(PENALTY_AREA, QRect(pitchRect.center().x() - 40, pitchRect.center().y() - 40, 80, 80), "", 0);
}

void ZoneMap::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
    gradient.setColorAt(0, QColor(30, 100, 30));
    gradient.setColorAt(1, QColor(0, 60, 0));
    painter->fillRect(rect, gradient);

    drawFootballField(painter);

    for (const Zone& zone : m_zones) {
        drawZone(*painter, zone, zone.type == m_highlightedZone);
    }

    painter->setFont(QFont("Arial", 16, QFont::Bold));
    painter->setPen(Qt::white);
    painter->drawText(rect, Qt::AlignTop | Qt::AlignHCenter, m_stadiumName);
}

void ZoneMap::drawFootballField(QPainter *painter)
{
    int w = SCENE_WIDTH;
    int h = SCENE_HEIGHT;
    QRect fieldRect(w*0.2, h*0.2, w*0.6, h*0.6);

    QRadialGradient fieldGradient(fieldRect.center(), fieldRect.width()/2);
    fieldGradient.setColorAt(0, QColor(0, 180, 0));
    fieldGradient.setColorAt(1, QColor(0, 120, 0));
    painter->setBrush(fieldGradient);
    painter->setPen(QPen(Qt::white, 2));
    painter->drawRect(fieldRect);

    painter->drawEllipse(fieldRect.center(), w*0.08, w*0.08);
    painter->drawLine(QPoint(w/2, h*0.2), QPoint(w/2, h*0.8));
    painter->drawRect(QRect(w*0.2, h*0.4, w*0.1, h*0.2));
    painter->drawRect(QRect(w*0.7, h*0.4, w*0.1, h*0.2));
    painter->drawArc(w*0.2, h*0.4, w*0.2, h*0.2, 0, 180*16);
    painter->drawArc(w*0.6, h*0.4, w*0.2, h*0.2, 0, -180*16);
}

void ZoneMap::drawZone(QPainter& painter, const Zone& zone, bool highlighted)
{
    if (zone.type == PENALTY_AREA) return;

    QRect area = zone.area.adjusted(5,5,-5,-5);
    QColor color = highlighted ? zone.color.lighter(150) : zone.color;

    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(color);
    painter.drawRoundedRect(area, 10, 10);

    painter.setFont(m_font);
    painter.setPen(Qt::white);
    QString info = QString("%1\n%2/%3")
                       .arg(zone.name)
                       .arg(zone.occupied)
                       .arg(zone.capacity);
    painter.drawText(area, Qt::AlignCenter | Qt::TextWordWrap, info);
}

void ZoneMap::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
    bool found = false;

    for (auto it = m_zones.begin(); it != m_zones.end(); ++it) {
        if (it->area.contains(pos.toPoint())) {
            m_highlightedZone = it.key();
            found = true;
            QToolTip::showText(event->globalPosition().toPoint(),
                               QString("<b>%1</b><hr>Capacity: %2<br>Occupied: %3<br>Available: %4")
                                   .arg(it->name)
                                   .arg(it->capacity)
                                   .arg(it->occupied)
                                   .arg(it->capacity - it->occupied),
                               this);
            break;
        }
    }

    if (!found && m_highlightedZone != GENERAL_SEATING) {
        m_highlightedZone = GENERAL_SEATING;
        QToolTip::hideText();
    }

    update();
}

void ZoneMap::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    if (m_totalCapacity > 0) calculateZones();
}

void ZoneMap::updateZoneOccupancy(ZoneType type, int change)
{
    if (m_zones.contains(type)) {
        Zone& zone = m_zones[type];
        zone.occupied = qBound(0, zone.occupied + change, zone.capacity);
        update();
    }
}
