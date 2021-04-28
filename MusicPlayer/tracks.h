
#ifndef TRACKS_H
#define TRACKS_H

using namespace std;
#include <QString>
#include <memory>
#include <string>
#include <fstream>
#include <vector>
class Track
{


public:
    Track(const QString& path);
    double progress = 0;
    virtual bool ReadHeader() = 0;
    virtual char* ReadData() = 0;
    virtual ~Track(){}
    virtual size_t GetBitDepth() const = 0;
    virtual size_t GetChannelCount() const = 0;
    virtual size_t GetSamplFreq() const = 0;
    virtual unsigned long GetSize() const = 0;
    char* data;
    QString path;
    ifstream dataStream;



};

class WavTrack final :
        public Track
{

    struct  WavHeader{
        char                riffHeader[4];        // RIFF Header      Magic header
        unsigned long       chunkSize;      // RIFF Chunk Size
        char                waveHeader[4];        // WAVE Header
        char                fmtHeader[4];         // FMT header
        unsigned long       subchunk1Size;  // Size of the fmt chunk
        unsigned short      audioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        unsigned short      numOfChan;      // Number of channels 1=Mono 2=Sterio
        unsigned long       samplesPerSec;  // Sampling Frequency in Hz
        unsigned long       bytesPerSec;    // bytes per second
        unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo
        unsigned short      bitsPerSample;  // Number of bits per sample
        char                subchunk2ID[4]; // "data"  string
        unsigned long       subchunk2Size;  // Sampled data length

    };
    WavHeader header;
 public:
    WavTrack(const QString& path);

    bool ReadHeader() override;
    char* ReadData() override;
    virtual size_t GetBitDepth() const override;

    virtual size_t GetChannelCount() const override;

    virtual size_t GetSamplFreq() const override;
    virtual unsigned long GetSize() const override;

};

class Mp3Track final:
        public Track
{
public:
    Mp3Track(const QString& path);
    bool ReadHeader() override;
    char* ReadData() override;
    virtual size_t GetBitDepth() const override;

    virtual size_t GetChannelCount() const override;

    virtual size_t GetSamplFreq() const override;
    virtual unsigned long GetSize() const override;
};

class TrackFactory
{
private:
    QString GetPathSuffix(const QString& path);

    QString GetPathPrefix(const QString& path);


public:
    unique_ptr<Track> CreateTrack(const QString& path);

};
#endif // TRACKS_H
