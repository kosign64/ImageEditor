#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>

namespace Ui {
class MainWindow;
}

class MyLabel;

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    MyLabel *myLabel;
    QImage monoImg;
    QMovie animation;
    QString code;
    QString path;
    QSettings *settings;
    bool anim;
    bool paint;
    bool resize;
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_action_2_triggered();

    void on_action_triggered();

    void on_getCodeButton_clicked();

    void getCoordinates(int x, int y);

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_actionAbout_Qt_triggered();

    void on_action_6_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
