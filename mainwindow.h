#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTime>
namespace Ui {
class MainWindow;
}
struct GRANULE
{
    int curX;
    int curY;
    int x[1000];//ˮƽλ��
    int y[1000];//��ֱλ��
    int vx[1000];//ˮƽ�ٶ�
    int vy[1000];//��ֱ�ٶ�
    bool isShow[1000];//�Ƿ���ʾ
    //��ɫ
    int r[1000];
    int g[1000];
    int b[1000];
    int acc;//���ٶ�
    int count;//�ܵ��� 1000

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    // ��굥���¼�
   void mousePressEvent(QMouseEvent *event);
   void paintEvent(QPaintEvent *);
   void timerEvent(QTimerEvent *event);//ʱ������
   void changeEvent(QEvent *);
   void drawView(QPainter &painter);
   void makeFire();

private:
    Ui::MainWindow *ui;
public:
    QVector<GRANULE> fire;
    int curX;
    int curY;
    int appTimerId;
    bool paintFlag;
    int starX[50];
    int starY[50];
};

#endif // MAINWINDOW_H
