#include "audioVisualizer.h"
AudioVisualizer::AudioVisualizer()
{
    player=nullptr;
    rect=nullptr;
}
//initialize the visualizer
AudioVisualizer::AudioVisualizer(MusicPlayer* player, unique_ptr<QGraphicsScene>&& scene, unique_ptr<QGraphicsView>&& view)
{
    this->player = player;
    this->scene = std::move(scene);
    this->view = std::move(view);
    this->view->setScene(this->scene.get());
    const size_t windowWidth = 650;
    const size_t windowHeight = 650;
    const size_t rectangleCount = 200;
    const size_t rectWidth = 3;
    const double hueFactor = 3/(double)4;
    this->view->setFixedSize(windowWidth, windowHeight);
    for(size_t i =0 ; i < rectangleCount;i++)
    {
        QColor color;
        color.setHsv( i*hueFactor, 255, 255);
        QBrush brush(color);
        QPen blackPen(Qt::white);
        blackPen.setWidth(1);
        rectangles.push_back(this->scene->addRect(i*rectWidth,0,rectWidth,0,blackPen, brush));
    }
    this->view->show();
}

QGraphicsView* AudioVisualizer::getView()
{
    return view.get();
}
//manages updating of the visualizer
void AudioVisualizer::updateVisualizer()
{
    auto length= player->currentSong->GetSize();
    size_t tickInterval = 30;
    double epsilon = 1.002;
    auto freq= player->currentSong->GetSamplFreq();
    size_t processCount = 4096;
    size_t syncInterval = 100;
    processCount = processCount - (processCount % 8);
    long offset = (long)(std::min(position * epsilon,(double)1) * length);
    if(length - offset > processCount * player->currentSong->GetBitDepth() * player->currentSong->GetChannelCount() * epsilon/ 8)
    {
        offset = offset - (offset % 8);
        fftw_complex in[processCount];
        readBuffer(offset, processCount, in);
        if(withoutSyncCount == 0)
            position = player->currentSong->progress;
        else
            position+= ((freq*tickInterval/(double)1000) * player->currentSong->GetBitDepth() * player->currentSong->GetChannelCount())/((double)length * 8);
        fftw_complex out[processCount];
        fftw_plan plan;
        plan =  fftw_plan_dft_1d(processCount, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(plan);
        fftw_destroy_plan(plan);
        auto resultFreqs = getFrequencies(processCount, out);
        updateRectangles(resultFreqs);
    }
    withoutSyncCount = (withoutSyncCount + 1) % syncInterval;

}

void AudioVisualizer::resume()
{
    position = player->currentSong->progress;
}
//redraws the rectangles to fit the current fourier transform
void AudioVisualizer::updateRectangles(vector<double> bufferValues)
{
    const size_t rectScale = 100;
    const size_t maxRectLen = 500;
    for(size_t i = 0; i < rectangles.size(); i+=2)
    {
        double value = std::min(( bufferValues[i] + bufferValues[i + 1])/rectScale,(double)maxRectLen);
        rectangles[i]->setRect(i*3, 600 - value, 3,value);
    }
}

//reads the data buffer based on the bitdepth an channelcount
void AudioVisualizer::readBuffer(long offset, long processCount, fftw_complex* samples)
{

    const size_t real = 0;
    const size_t imag = 1;
    switch(player->currentSong->GetBitDepth())
    {
        case 8:
            if(player->currentSong->GetChannelCount() == 1)
                    for(auto i = offset; i < offset + processCount; i++)
                    {
                        samples[i][real] = (double)((int16_t)this->byteArray[i]);
                        samples[i][imag] = 0;
                    }
            else
                for(auto i = offset; i < offset + 2*processCount - 1; i+=2)
                {
                    samples[(i-offset)/2][real] =(double) ((int16_t)this->byteArray[i]);
                    samples[(i-offset)/2][imag] = 0;

                }
            break;
        case 16:
            if(player->currentSong->GetChannelCount() == 1)
                for(auto i = offset; i < offset + 2*processCount - 1; i+=2)
                {
                    //double multiplier = 0.5 * (1 - cos(2*pi*i/2047));
                    int16_t intValue = (int16_t) this->byteArray[i + 1];
                    samples[(i-offset)/2][real] =(double) ((int16_t)this->byteArray[i] | (intValue << 8));
                    samples[(i-offset)/2][imag] = 0;
                }
            else
                for(auto i = offset; i < offset + 4*processCount - 2; i+=4)
                {
                    int16_t intValue = (int16_t) this->byteArray[i + 1];
                    samples[(i-offset)/4][real] = (double)((int16_t)(this->byteArray[i]) + (intValue << 8));
                    samples[(i-offset)/4][imag] = 0;
                }
            break;
    }

}

//prepares the data for playing the next song
void AudioVisualizer::loadNextSong()
{
    byteArray = std::vector<char>(player->currentSong->data, player->currentSong->data + player->currentSong->GetSize());
    position = 0;
}

//calculates the magnitudes of teh frequencies
vector<double> AudioVisualizer::getFrequencies(size_t processCount, fftw_complex* freqs)
{
    vector<double> resultFreqs;
    const size_t real = 0;
    const size_t comp = 1;
    resultFreqs.reserve(processCount);
    for(size_t i = 0; i < processCount; i++)
    {
        resultFreqs.push_back(std::sqrt(freqs[i][real]*freqs[i][real] + freqs[i][comp]*freqs[i][comp])/100);

    }
    return resultFreqs;
}
