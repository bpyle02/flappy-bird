#include "scene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include "pipe.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent), gameOn(false), score(0), highScore(0)
{
    setUpPipeTimer();
}

void Scene::addBird()
{
    bird = new Bird(QPixmap(":/images/Wings_Up.png"));
    addItem(bird);
    bird->setScale(1.8);
    bird->setPos(QPointF(-40, 0));
}

void Scene::startGame()
{
    bird->startFlying();

    if (!pipeTimer->isActive())
    {
        setGameOn(true);
        removePipes();
        hideGameOverGraphics();
        setScore(0);
        pipeTimer->start(1000);
    }
}

void Scene::setGameOn(bool val)
{
    gameOn = val;
}

bool Scene::getGameOn()
{
    return gameOn;
}

void Scene::incrementScore()
{
    score++;

    if (score > highScore)
        highScore = score;

    //qDebug() << "Score: " << score << ", High Score: " << highScore;
}

void Scene::setUpPipeTimer()
{
    pipeTimer = new QTimer(this);
    connect(pipeTimer, &QTimer::timeout, [=](){
        Pipe *pipeItem = new Pipe();

        connect(pipeItem, &Pipe::stopGame, [=](){
            pipeTimer->stop();
            freezeObjects();
            setGameOn(false);
            showGameOverGraphics();
        });
        addItem(pipeItem);
    });
}

void Scene::freezeObjects()
{
    bird->freeze();

    QList<QGraphicsItem*> sceneItems = items();

    foreach (QGraphicsItem *item, sceneItems)
    {
        Pipe *pipe = dynamic_cast<Pipe*>(item);

        if (pipe)
        {
            pipe->freeze();
        }
    }
}

void Scene::removePipes()
{
    QList<QGraphicsItem*> sceneItems = items();

    foreach (QGraphicsItem *item, sceneItems)
    {
        Pipe *pipe = dynamic_cast<Pipe*>(item);
        if (pipe && gameOn)
        {
            removeItem(pipe);
            delete pipe;
        }
    }
}

void Scene::showGameOverGraphics()
{
    gameOverGraphic = new QGraphicsPixmapItem(QPixmap(":/images/Game_Over"));
    addItem(gameOverGraphic);
    gameOverGraphic->setScale(2.3);
    gameOverGraphic->setPos(QPointF(0, 0) - QPointF(gameOverGraphic->boundingRect().width(), gameOverGraphic->boundingRect().height() / 2));

    scoreTextItem = new QGraphicsTextItem();

    QString htmlString = "<p>Score: " + QString::number(score) + "</p>" + "<p>High Score: " + QString::number(highScore) + "</p>";

    QFont mFont("Arial", 25, QFont::Bold);

    scoreTextItem->setHtml(htmlString);
    scoreTextItem->setFont(mFont);
    scoreTextItem->setDefaultTextColor(Qt::white);
    addItem(scoreTextItem);

    scoreTextItem->setPos(QPointF(0, 0) - QPointF(scoreTextItem->boundingRect().width() / 2, -gameOverGraphic->boundingRect().height() / 2));
}

void Scene::hideGameOverGraphics()
{
    if (gameOverGraphic)
    {
        removeItem(gameOverGraphic);
        delete gameOverGraphic;
        gameOverGraphic = nullptr;
    }

    if (scoreTextItem)
    {
        removeItem(scoreTextItem);
        delete scoreTextItem;
        scoreTextItem = nullptr;
    }
}

void Scene::setScore(int s)
{
    score = s;
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space && gameOn)
    {
        bird->shootUp();
    }

    QGraphicsScene::keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && gameOn)
    {
        bird->shootUp();
    }

    QGraphicsScene::mousePressEvent(event);
}
