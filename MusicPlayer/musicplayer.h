
#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include "tracks.h"
#include <deque>
#include <memory>
#include <QString>
#include <algorithm>
#include <QTextStream>
#include <al.h>
#include <alc.h>
#include <alg.h>
#include <type_traits>
#include <new>
#include <string>
#include <iostream>
#include <thread>

using namespace std;

class MusicPlayer
{
public:
    unique_ptr<Track> currentSong = nullptr;
    TrackFactory trackFactory;
    deque<unique_ptr<Track>> queue;    
    size_t currentProgressValidationCounter = 0;
    double volume = 0.8;
    std::thread playingThread;
    bool isRunning = false;
    MusicPlayer();
    void AddToQueue(const QString& path);
    void RemoveFromQueue(const QString& path);
    void GetNextTrack();
    bool Play();


};


bool checkErrorsForAlc(const std::string& filename, const size_t line, ALCdevice* device);
bool checkErrorsForAl(const std::string& filename, const size_t line);

template<typename alcFunction, typename... Params> auto callAlcWrap(const char* filename,
                 const size_t line,
                 alcFunction function,
                 ALCdevice* device,
                 Params... params)
->typename std::enable_if_t<std::is_same<void,decltype(function(params...))>::value, bool>
{

    function(std::forward<Params>(params)...);
    return checkErrorsForAlc(filename,line,device);
}

template<typename alcFunction, typename ReturnType, typename... Params>
auto callAlcWrap(const char* filename,
                 const size_t line,
                 alcFunction function,
                 ReturnType& returnValue,
                 ALCdevice* device,
                 Params... params)
->typename std::enable_if_t<!is_same<void,decltype(function(params...))>::value,bool>
{
    returnValue = function(std::forward<Params>(params)...);
    return checkErrorsForAlc(filename,line,device);
}

template<typename alFunction, typename... Params>
auto callAlWrap(const char* filename,
    const std::uint_fast32_t line,
    alFunction function,
    Params... params)
    ->typename std::enable_if_t<!is_same<void, decltype(function(params...))>::value, decltype(function(params...))>
{
    auto ret = function(std::forward<Params>(params)...);
    checkErrorsForAl(filename, line);
    return ret;
}

template<typename alFunction, typename... Params>
auto callAlWrap(const char* filename,
    const std::uint_fast32_t line,
    alFunction function,
    Params... params)
    ->typename std::enable_if_t<is_same<void, decltype(function(params...))>::value, bool>
{
    function(std::forward<Params>(params)...);
    return checkErrorsForAl(filename, line);
}


#endif // MUSICPLAYER_H
