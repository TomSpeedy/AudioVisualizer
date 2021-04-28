#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <musicplayer.h>
#include <mutex>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "audioVisualizer.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
     MusicPlayer musicPlayer;
     AudioVisualizer visualizer;
     unique_ptr<QGraphicsScene> scene;
     unique_ptr<QGraphicsView> view;
     QGraphicsRectItem* rect;
     int temp = 0;
public:
    QTimer progressTimer;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startMyTimer();
    mutex mtx_lock;

private slots:
    void on_addToQueueButton_clicked();

    void on_removeFromQueueButton_clicked();

    void on_previousButton_clicked();

    void on_playButton_clicked();

    void on_stopButton_clicked();

    void on_nextButton_clicked();
    void on_timerTicked();

    void on_progressSlider_sliderReleased();

    void on_volumeSlider_sliderReleased();

    void on_progressSlider_sliderPressed();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
