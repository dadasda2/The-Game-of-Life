#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QColor>
#include <QWidget>

class GameWidget : public QWidget
{
    Q_OBJECT
public:
     GameWidget(QWidget *parent = 0);
    ~GameWidget();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

public slots:
    void startGame();
    void stopGame();
    void clear();
    int interval(); // interval between generations
    void setInterval(int msec); // set interval between generations

private slots:
    void paintGrid(QPainter &p);
    void paintUniverse(QPainter &p);
    void newGeneration();

private:
    int color[3];
    int gridSize;
    QTimer* timer;
    bool   grid[50][50];
    bool  ngrid[50][50];
    bool isAlive(int x, int y); // return true if cell accept rules
};

#endif // GAMEWIDGET_H
