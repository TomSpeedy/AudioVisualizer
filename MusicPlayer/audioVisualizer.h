#ifndef AUDIOVISUALIZER_H
#define AUDIOVISUALIZER_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QGraphicsScene>
#include "musicplayer.h"
#include "fftw3.h"
class AudioVisualizer
{
private:
    MusicPlayer* player;
    unique_ptr<QGraphicsScene> scene;
    unique_ptr<QGraphicsView> view;
    vector<QGraphicsRectItem*> rectangles;
    QGraphicsRectItem* rect;
    vector<char> byteArray;
    double position = 0;
    size_t withoutSyncCount = 1;
public:
    AudioVisualizer();
    AudioVisualizer(MusicPlayer* player, unique_ptr<QGraphicsScene>&& scene, unique_ptr<QGraphicsView>&& view);

    QGraphicsView* getView();
    void updateVisualizer();

    void resume();
    void updateRectangles(vector<double> bufferValues);
    void readBuffer(long offset, long processCount, fftw_complex* samples);

    void loadNextSong();
    vector<double> getFrequencies(size_t processCount, fftw_complex* freqs);
};


#endif // AUDIOVISUALIZER_H
