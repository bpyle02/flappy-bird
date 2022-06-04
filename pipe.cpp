#include "pipe.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QGraphicsScene>
#include "bird.h"
#include "scene.h"

Pipe::Pipe() : topPipe(new QGraphicsPixmapItem(QPixmap(":/images/Pipe.png"))), bottomPipe(new QGraphicsPixmapItem(QPixmap(":/images/Pipe.png"))), scorePoint(false)
{
    //Set starting position
    topPipe->setPos(QPointF(0, 0) - QPointF(topPipe->boundingRect().width() + 3, topPipe->boundingRect().height() + 280));
    bottomPipe->setPos(QPointF(0, 0) + QPointF(bottomPipe->boundingRect().width() + 5, 420));

    //Flip bottom pipe
    bottomPipe->setRotation(bottomPipe->rotation() + 180);

    //Enlarge the pipes to fit screen
    topPipe->setScale(2.3);
    bottomPipe->setScale(2.3);

    addToGroup(topPipe);
    addToGroup(bottomPipe);

    //Create random starting x and y positions
    yPos = QRandomGenerator::global()->bounded(150);
    int xRandomizer = QRandomGenerator::global()->bounded(200);

    setPos(QPoint(0, 0) + QPoint(260 + xRandomizer, yPos));

    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(-260);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(1500);

    connect(xAnimation, &QPropertyAnimation::finished, [=](){
        scene()->removeItem(this);
        delete this;
    });

    xAnimation->start();
}

Pipe::~Pipe()
{
    delete topPipe;
    delete bottomPipe;
}

qreal Pipe::x() const
{
    return m_x;
}

void Pipe::freeze()
{
    xAnimation->stop();
}

void Pipe::setX(qreal newX)
{
    m_x = newX;

    if (newX < 0 && !scorePoint)
    {
        scorePoint = true;
        QGraphicsScene *mScene = scene();
        Scene *myScene = dynamic_cast<Scene*>(mScene);

        if (myScene)
        {
            myScene->incrementScore();
        }
    }

    if (collision())
    {
        emit stopGame();
    }

    setPos(QPointF(0, 0) + QPointF(newX, yPos));
}

bool Pipe::collision()
{
    QList<QGraphicsItem*> collidingItems = topPipe->collidingItems();
    collidingItems.append(bottomPipe->collidingItems());

    foreach (QGraphicsItem *item, collidingItems)
    {
        Bird *bird = dynamic_cast<Bird*>(item);

        if (bird)
        {
            return true;
        }
    }

    return false;
}
