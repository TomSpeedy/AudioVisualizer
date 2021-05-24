# AudioVisualizer

## Specification:

The project AudioVisualizer is an application which is capable of:

-playing selected files in non compressed .wav format

-is extandable to other formats by using the inheritance

-skipping the playing to the offset of the song using the slider or skipping to the next song

-adjusting the volume of the currently played song by the slider

-play songs from the queue

-visualizing the frequencies of the currently played played song


## Implementation:

### Main Objects

**MainWindow**

Main window has its own instance of MusicPlayer and serves mostly for event hadling.

**MusicPlayer**

MusicPlayer keeps track of the playing and calls the Open AL library. Tracks the queue and also the current Track.

**Track**

Root class for anything that will be played by Music player. Currently WAVTrack:Track is implemented (ReadHeader() and ReadData() is required)

**TrackFactory**

Class that creates Track instances based on the input string parameter 

**AudioVisualizer**

This class mostly calculates fast fourier transform on the input and also draws its result to the output

### Threading
The applicaiton uses two active theads to make the UI responsive - application thread and playing thread.

-application thread handles the form events as well as drawing of the visualization

-playing thread is fully occupied by playing the given songs, for each song a new thread is created


#### Playing Synchronization
The reading of the header is done bytewise and when we read the header, we open openAL library, pass it the data buffer and start playing. When trying to get current position in the currently played data of the openAL, the result is rounded down by the size of a buffer. This means, if the visualization used the position in the data from the player, it would visualize the same buffer for a longer time which would cause it to look inaccurate and laggy. Thats why we decided to approximate the playing speed and do the visualization with this approximated playing speed (there is a timer in application thread which causes to update the visualization frequently and this timer is synchronized with the playing thread progress once per 3 seconds, just to make sure playing and visualization is in sync and fits the song). When the song is over, the playing thread is joined.

#### Changing state Synchronization
State of the player is shared between the threads using shared variables (bool stopped, double progress)

The prorgess bar is updated based on the progress of the song and when the user changes the position of the progress bar - the validation counter is changed and the music player stops playing and sets its position to the new value


## Build the project from console:

In order to build the project from console:

1.Create the build output directory

2.CD to the build output directory

3.Call qmake referencing the .pro file

4.Call make 

Real Example:

cd MyBuildDirectory

qmake ../MyProject/myproject.pro

nmake

## Build the project by loading it to QT creator

1.Load the QT project into qt creator (.pro) file

2.Build the project from the UI of QT creator
