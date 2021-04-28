#include <musicplayer.h>
#include <mainwindow.h>
#include <ui_mainwindow.h>
MusicPlayer::MusicPlayer()
{
}
#define callAlc(function, device, ...) callAlcWrap(__FILE__, __LINE__, function, device, __VA_ARGS__)
#define callAl(function, ...) callAlWrap(__FILE__, __LINE__, function, __VA_ARGS__)
bool MusicPlayer::Play()
{
    QTextStream deb( stdout );
    isRunning = true;
    size_t currentState = this->currentProgressValidationCounter;
    ALCdevice* openALDevice = alcOpenDevice(nullptr);
    if(!openALDevice)
        return false;

    ALCcontext* openALContext;
    if(!callAlc(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext)
    {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
        return false;
    }
    ALCboolean contextMadeCurrent = false;
    if(!callAlc(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext)
       || contextMadeCurrent != ALC_TRUE)
    {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
        return false;
    }
    ALuint buffer;
    callAl(alGenBuffers, 1, &buffer);
    ALenum alFormat;

    if(this->currentSong->GetChannelCount() == 1 && this->currentSong->GetBitDepth() == 8)
        alFormat = AL_FORMAT_MONO8;
    else if(this->currentSong->GetChannelCount() == 1 && this->currentSong->GetBitDepth() == 16)
        alFormat = AL_FORMAT_MONO16;
    else if(this->currentSong->GetChannelCount() == 2 && this->currentSong->GetBitDepth() == 8)
        alFormat = AL_FORMAT_STEREO8;
    else if(this->currentSong->GetChannelCount() == 2 && this->currentSong->GetBitDepth() == 16)
        alFormat = AL_FORMAT_STEREO16;
    else
    {
        cerr
            << "ERROR: given wave format is not recognized: "
            << this->currentSong->GetChannelCount() << " channels, "
            << this->currentSong->GetBitDepth() << " bits depth" << std::endl;
        return false;
    }


    callAl(alBufferData, buffer, alFormat, this->currentSong->data ,this->currentSong->GetSize(), this->currentSong->GetSamplFreq());
    ALuint source;
        callAl(alGenSources, 1, &source);
        callAl(alSourcef, source, AL_PITCH, 1);
        callAl(alSource3f, source, AL_POSITION, 0, 0, 0);
        callAl(alSource3f, source, AL_VELOCITY, 0, 0, 0);
        callAl(alSourcei, source, AL_LOOPING, AL_FALSE);
        callAl(alSourcei, source, AL_BUFFER, buffer);

        callAl(alSourcePlay, source);
        ALint state = AL_PLAYING;
        float currentOffset = 0;
        while(state == AL_PLAYING || state == AL_PAUSED)
        {
            if(!this->isRunning)
            {
                if (state == AL_PLAYING)
                    callAl(alSourcePause, source);
                state = AL_PAUSED;
            }
            if(this->isRunning)
             {
                if (state == AL_PAUSED)                    
                    callAl(alSourcePlay, source);

                state = AL_PLAYING;
              }
            if(this->currentSong->progress != 1)
                callAl(alGetSourcef, source, AL_BYTE_OFFSET, &currentOffset);

            if(currentState != this->currentProgressValidationCounter)
            {
                if(this->currentSong->progress == 1)
                {
                    state = AL_STOPPED;
                }
                else
                {
                    callAl(alSourcef, source, AL_BYTE_OFFSET, (int)(this->currentSong->GetSize() * this->currentSong->progress));
                    currentState = this->currentProgressValidationCounter;
                    callAl(alGetSourcef, source, AL_BYTE_OFFSET, &currentOffset);
                }
            }
            if(this->currentSong->progress != 1)
            {
                state = AL_STOPPED;

            this->currentSong->progress = currentOffset / this->currentSong->GetSize();
            callAl(alSourcef, source, AL_GAIN, this->volume);
            callAl(alGetSourcei, source, AL_SOURCE_STATE, &state);
            }
            //deb << currentOffset << Qt::endl;

        }
        this->currentSong->progress = 1;
        callAl(alDeleteSources, 1, &source);
        callAl(alDeleteBuffers, 1, &buffer);

        callAlc(alcMakeContextCurrent, contextMadeCurrent, openALDevice, nullptr);
        callAlc(alcDestroyContext, openALDevice, openALContext);

        ALCboolean closed;
        //callAlc(alcCloseDevice, closed, openALDevice, openALDevice);

        return true;
}
bool checkErrorsForAlc(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device)
{
    ALCenum error = alcGetError(device);
    if(error != ALC_NO_ERROR)
    {
        cerr << "***ERROR*** (" << filename << ": " << line << ")\n" ;
        switch(error)
        {
        case ALC_INVALID_VALUE:
            cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case ALC_INVALID_DEVICE:
            cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
            break;
        case ALC_INVALID_CONTEXT:
            cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
            break;
        case ALC_INVALID_ENUM:
            cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
            break;
        case ALC_OUT_OF_MEMORY:
           cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
            break;
        default:
           cerr << "UNKNOWN ALC ERROR: " << error;
        }

        return false;
    }
    return true;
}
bool checkErrorsForAl(const std::string& filename, const std::uint_fast32_t line)
{
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n" ;
        switch(error)
        {
        case AL_INVALID_NAME:
            std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            break;
        default:
            std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}
