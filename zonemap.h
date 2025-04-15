#ifndef ZONEMAP_H
#define ZONEMAP_H

#include <QGraphicsView>
#include <QMap>
#include <QLinearGradient>

class ZoneMap : public QGraphicsView
{
    Q_OBJECT
public:
    enum ZoneType {
        NORTH_STAND, GENERAL_SEATING, PITCH, VIP, MEDIA,
        FIRST_AID, SHOP, ENTRANCE, PARKING, TICKETING,
        FAMILY, AWAY_FANS, LOCKER_ROOM, PENALTY_AREA
    };

    explicit ZoneMap(QWidget *parent = nullptr);
    void setStadiumInfo(const QString& name, int capacity);
    void updateZoneOccupancy(ZoneType type, int change);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    struct Zone {
        QRect area;
        ZoneType type;
        QString name;
        QColor color;
        int capacity;
        int occupied;
    };

    void calculateZones();
    void addZone(ZoneType type, const QRect& area, const QString& name, int capacity);
    QColor getZoneColor(ZoneType type) const;
    void drawZone(QPainter& painter, const Zone& zone, bool highlighted);
    void drawFootballField(QPainter *painter);

    QMap<ZoneType, Zone> m_zones;
    QString m_stadiumName;
    int m_totalCapacity;
    QFont m_font;
    ZoneType m_highlightedZone;
};

#endif // ZONEMAP_H
