#include "tracks.h"
#include <QTextStream>

Track::Track(const QString& path):path(path), dataStream(path.toStdString(), std::ios::binary)
{

}
WavTrack::WavTrack(const QString& path):Track(path)
{

}
bool WavTrack::ReadHeader()
{

    try
    {
        dataStream.read(this->header.riffHeader,4);
        dataStream.read(reinterpret_cast<char *>(&this->header.chunkSize), sizeof(this->header.chunkSize));
        dataStream.read(this->header.waveHeader,4);
        dataStream.read(this->header.fmtHeader,4);
        dataStream.read(reinterpret_cast<char *>(&this->header.subchunk1Size), sizeof(this->header.subchunk1Size));
        dataStream.read(reinterpret_cast<char *>(&this->header.audioFormat), sizeof(this->header.audioFormat));
        dataStream.read(reinterpret_cast<char *>(&this->header.numOfChan), sizeof(this->header.numOfChan));
        dataStream.read(reinterpret_cast<char *>(&this->header.samplesPerSec), sizeof(this->header.samplesPerSec));
        dataStream.read(reinterpret_cast<char *>(&this->header.bytesPerSec), sizeof(this->header.bytesPerSec));
        dataStream.read(reinterpret_cast<char *>(&this->header.blockAlign), sizeof(this->header.blockAlign));
        dataStream.read(reinterpret_cast<char *>(&this->header.bitsPerSample), sizeof(this->header.bitsPerSample));
        dataStream.read(this->header.subchunk2ID,4);
        dataStream.read(reinterpret_cast<char *>(&this->header.subchunk2Size), sizeof(this->header.subchunk2Size));
        this->header.numOfChan = this->header.numOfChan == 0 ? 1 : this->header.numOfChan;
        this->header.bitsPerSample = this->header.bitsPerSample == 0 ? 16 : this->header.bitsPerSample;
        this->header.samplesPerSec = this->header.samplesPerSec == 0 ? 48000 : this->header.samplesPerSec;
        return true;
    }
    catch(...)
    {
      return false;
    }
}
char* WavTrack::ReadData()
{
    QTextStream degub (stdout);
   char* data = new char[this->GetSize()];
   this->dataStream.read(data, this->GetSize());
   this->data = data;
   degub << this->GetSize() << Qt::endl;
   return data;
}
size_t WavTrack::GetBitDepth() const
{
    return header.bitsPerSample;
}
size_t WavTrack::GetChannelCount() const
{
    return header.numOfChan;
}
size_t WavTrack::GetSamplFreq() const
{
    return header.samplesPerSec;
}
unsigned long WavTrack::GetSize() const
{
    return header.chunkSize;
}

Mp3Track::Mp3Track(const QString& path):Track(path)
{

}
bool Mp3Track::ReadHeader()
{
    return true;
}
char* Mp3Track::ReadData()
{
    char* data = new char[this->GetSize()];
    return data;
}
size_t Mp3Track::GetBitDepth() const
{
    return 0;
}
size_t Mp3Track::GetChannelCount() const
{
    return 0;
}
size_t Mp3Track::GetSamplFreq() const
{
    return 0;
}
unsigned long Mp3Track::GetSize() const
{
    return 0;
}
QString TrackFactory::GetPathSuffix(const QString& path)
{
    return path.right(path.length() - 1 - path.indexOf('.'));
}
QString TrackFactory::GetPathPrefix(const QString& path)
{
    char dirDelim;
    if(path.contains('/'))
        dirDelim = '/';
    else
        dirDelim = '\\';
    return path.left(path.lastIndexOf(dirDelim) + 1);
}
unique_ptr<Track> TrackFactory::CreateTrack(const QString& path)
{
    auto suffix = TrackFactory::GetPathSuffix(path);
    if(suffix == QString::fromStdString("wav"))
        return make_unique<WavTrack>(path);

    if (suffix == QString::fromStdString("mp3"))
        return make_unique<Mp3Track>(path);
    throw invalid_argument(string("This file extesion is not supported: ") + "\"" + suffix.toStdString() +"\"");
}
