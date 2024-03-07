#include "graphicsscene.h"
#include <QKeyEvent>

GraphicsScene::GraphicsScene(QRectF rect, QObject * parent): QGraphicsScene(rect, parent)
{
}

void GraphicsScene::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Up){
        emit signalSetDirection(0);
    }else if (keyEvent->key() == Qt::Key_Right){
        emit signalSetDirection(3);
    }else if (keyEvent->key() == Qt::Key_Down){
        emit signalSetDirection(1);
    }else if (keyEvent->key() == Qt::Key_Left){
        emit signalSetDirection(2);
    }

}
