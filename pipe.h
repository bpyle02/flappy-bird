#ifndef PIPE_H
#define PIPE_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class Pipe : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX);

public:
    explicit Pipe();
    ~Pipe();

    qreal x() const;

    void freeze();

signals:
    void stopGame();

public slots:
    void setX(qreal newX);

private:
    bool collision();

    QGraphicsPixmapItem *topPipe;
    QGraphicsPixmapItem *bottomPipe;
    QPropertyAnimation *xAnimation;

    int yPos;
    qreal m_x;
    bool scorePoint;
};

#endif // PIPE_H
