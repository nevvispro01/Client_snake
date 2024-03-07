#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QObject>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(QRectF rect, QObject * parent = nullptr);

signals:
    void signalSetDirection(int direction);
protected:

    virtual void	keyPressEvent(QKeyEvent *keyEvent) override;
};

#endif // GRAPHICSSCENE_H
