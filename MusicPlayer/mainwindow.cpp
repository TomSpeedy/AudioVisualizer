#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <thread>
#include <mutex>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    const double defaultVolume = 0.8;
    this->ui->volumeSlider->setValue((int)(this->ui->volumeSlider->maximum() * defaultVolume));
    scene = make_unique<QGraphicsScene>(this);
    view =  make_unique<QGraphicsView>(this->ui->graphicsView);
    //this->ui->graaddWidget(view.get());
    visualizer = AudioVisualizer(&this->musicPlayer, std::move(scene), std::move(view));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_addToQueueButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select song"), "D:/source/repos/Qt/MusicPlayer", tr("Sound files (*.mp3 *.wav)"));
    //add to mp3Player
    if(!fileName.isEmpty()&& !fileName.isNull()){
        musicPlayer.AddToQueue(fileName);
        this->ui->queueListWidget->addItem(fileName);
    }

}

void MainWindow::on_removeFromQueueButton_clicked()
{
    auto toRemove= this->ui->queueListWidget->selectedItems();

    for (auto && itemToRemove : toRemove)
    {
        //remove From mp3Player
        musicPlayer.RemoveFromQueue(itemToRemove->text());

        //remove From UI

        delete this->ui->queueListWidget->takeItem(this->ui->queueListWidget->row(itemToRemove));

    }

}

void MainWindow::on_previousButton_clicked()
{

}
void MainWindow::startMyTimer()
{
    this->progressTimer.start();
}
void MainWindow::on_playButton_clicked()
{
    QTextStream debug( stdout );
    if( musicPlayer.queue.size() > 0 && (musicPlayer.currentSong == nullptr || musicPlayer.currentSong->progress == 1))
    {        
        this->musicPlayer.GetNextTrack();        
        delete this->ui->queueListWidget->item(0);
        this->musicPlayer.currentSong->ReadHeader();
        this->musicPlayer.currentSong->ReadData();
        auto currentSongPath = this->musicPlayer.currentSong->path;
        this->ui->currentlyPlayingLabel->setText("Currently playing: " + currentSongPath.right(currentSongPath.length() - currentSongPath.lastIndexOf("/") - 1));
        if(this->musicPlayer.playingThread.joinable())
            this->musicPlayer.playingThread.join();
        this->musicPlayer.playingThread = std::thread(&MusicPlayer::Play, &musicPlayer);
        if(this->progressTimer.interval() != 30)
            {
                this->progressTimer.setInterval(30);
                this->progressTimer.callOnTimeout([this](){this->on_timerTicked();});
            }
        this->visualizer.loadNextSong();
        this->progressTimer.start();

    }
    if (musicPlayer.currentSong != nullptr && musicPlayer.currentSong->progress != 1)
    {
        this->musicPlayer.isRunning = true;
        visualizer.resume();
        this->progressTimer.start();

    }

    //musicPlayer.playingThread.join();
}


void MainWindow::on_timerTicked()
{   
    temp = (temp + 1) % 50;
    if(this->musicPlayer.currentSong != nullptr && !!this->musicPlayer.isRunning)
    {
        visualizer.updateVisualizer();
        this->ui->progressSlider->setValue((int)(this->ui->progressSlider->maximum() * this->musicPlayer.currentSong->progress));
    if (this->musicPlayer.currentSong->progress == 1)
    {
        this->progressTimer.stop();
          this->musicPlayer.playingThread.join();
           if(this->musicPlayer.queue.size() > 0)
               this->on_playButton_clicked();
           else
               this->musicPlayer.currentSong = nullptr;
    }
    }

}

void MainWindow::on_stopButton_clicked()
{
    this->musicPlayer.isRunning = false;
    this->progressTimer.stop();
}

void MainWindow::on_nextButton_clicked()
{

    if(this->musicPlayer.queue.size() > 0)
        {
            if(musicPlayer.currentSong != nullptr)
            {
                const std::lock_guard<std::mutex> lock(mtx_lock);
                musicPlayer.currentProgressValidationCounter++;
                musicPlayer.currentSong->progress = 1;
            }
            if(this->musicPlayer.playingThread.joinable())
                this->musicPlayer.playingThread.join();
            this->on_playButton_clicked();
        }

}

void MainWindow::on_progressSlider_sliderReleased()
{
    const std::lock_guard<std::mutex> lock(mtx_lock);
    musicPlayer.currentSong->progress = this->ui->progressSlider->value()/(double)this->ui->progressSlider->maximum();
    musicPlayer.currentProgressValidationCounter++;

}

void MainWindow::on_volumeSlider_sliderReleased()
{
    double newVolume = this->ui->volumeSlider->value() / (double)this->ui->volumeSlider->maximum();
    this->musicPlayer.volume = newVolume;

}

void MainWindow::on_progressSlider_sliderPressed()
{
    this->musicPlayer.isRunning = false;
    this->progressTimer.stop();
}
