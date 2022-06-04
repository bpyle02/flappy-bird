#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include "pipe.h"
#include "bird.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void addBird();
    void startGame();
    void setGameOn(bool val);
    bool getGameOn();
    void incrementScore();
    void hideGameOverGraphics();

signals:

public slots:

private:
    void setUpPipeTimer();
    void freezeObjects();
    void removePipes();
    void showGameOverGraphics();
    void setScore(int s);

    bool gameOn;
    bool showGraphics;
    QTimer *pipeTimer;
    Bird *bird;
    int score;
    int highScore;

    QGraphicsPixmapItem *gameOverGraphic;
    QGraphicsTextItem *scoreTextItem;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // SCENE_H
