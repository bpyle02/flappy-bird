#ifndef BIRD_H
#define BIRD_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class Bird : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation);
    Q_PROPERTY(qreal y READ y WRITE setY);

public:
    explicit Bird(QPixmap pixmap);

    qreal rotation() const;
    qreal y() const;

    void shootUp();
    void startFlying();
    void freeze();

signals:

public slots:
    void setY(qreal newY);
    void setRotation(qreal newRotation);
    void rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve);
    void fall();

private:
    enum WingPos
    {
        Up,
        Middle,
        Down
    };

    void updatePixmap();

    WingPos wingPos;
    bool wingDirection;
    qreal m_rotation;
    qreal m_y;
    QPropertyAnimation *yAnimation;
    QPropertyAnimation *rotationAnimation;
    qreal groundPos;
};

#endif // BIRD_H
