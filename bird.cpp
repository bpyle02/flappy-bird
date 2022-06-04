#include "bird.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>

Bird::Bird(QPixmap pixmap) :
    wingPos(WingPos::Up),
    wingDirection(false)
{
    setPixmap(pixmap);

    QTimer *birdWingsTimer = new QTimer(this);
    connect(birdWingsTimer, &QTimer::timeout, [=](){
        updatePixmap();
    });

    birdWingsTimer->start(80);

    groundPos = scenePos().y() + 290;

    yAnimation = new QPropertyAnimation(this, "y", this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(groundPos);
    yAnimation->setEasingCurve(QEasingCurve::InQuad);
    yAnimation->setDuration(1000);

    rotationAnimation = new QPropertyAnimation(this, "rotation", this);
}

void Bird::updatePixmap()
{
    if (wingPos == WingPos::Middle)
    {
        if (wingDirection)
        {
            //Up
            setPixmap(QPixmap(":/images/Wings_Up.png"));
            wingPos = WingPos::Up;
            wingDirection = false;
        }
        else
        {
            //Down
            setPixmap(QPixmap(":/images/Wings_Down.png"));
            wingPos = WingPos::Down;
            wingDirection = true;
        }
    }
    else
    {
        setPixmap(QPixmap(":/images/Wings_Straight.png"));
        wingPos = WingPos::Middle;
    }
}

qreal Bird::rotation() const
{
    return m_rotation;
}

void Bird::setRotation(qreal newRotation)
{
    m_rotation = newRotation;

    QPointF c = boundingRect().center();

    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(newRotation);
    t.translate(-c.x(), -c.y());
    setTransform(t);
}

void Bird::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation->setStartValue(rotation());
    rotationAnimation->setEndValue(end);
    rotationAnimation->setEasingCurve(curve);
    rotationAnimation->setDuration(duration);

    rotationAnimation->start();
}

void Bird::fall()
{
    if (y() < groundPos)
    {
        rotationAnimation->stop();

        yAnimation->setStartValue(y());
        yAnimation->setEndValue(groundPos);
        yAnimation->setEasingCurve(QEasingCurve::InQuad);
        yAnimation->setDuration(1000);

        yAnimation->start();

        rotateTo(90, 1100, QEasingCurve::InCubic);
    }
}

qreal Bird::y() const
{
    return m_y;
}

void Bird::setY(qreal newY)
{
    moveBy(0, newY - m_y);
    m_y = newY;
}

void Bird::shootUp()
{
    yAnimation->stop();
    rotationAnimation->stop();

    qreal curPosY = y();

    yAnimation->setStartValue(curPosY);
    yAnimation->setEndValue(curPosY - scene()->sceneRect().height() / 8);
    yAnimation->setEasingCurve(QEasingCurve::OutQuad);
    yAnimation->setDuration(285);

    connect(yAnimation, &QPropertyAnimation::finished, [=](){
        fall();
    });

    yAnimation->start();

    rotateTo(-20, 200, QEasingCurve::OutCubic);
}

void Bird::startFlying()
{
    yAnimation->start();
    rotateTo(90, 1200, QEasingCurve::InQuad);
}

void Bird::freeze()
{
    yAnimation->stop();
    rotationAnimation->stop();
}
