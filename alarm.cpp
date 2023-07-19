#include "alarm.h"
#include "fileio.h"
#include <QMediaPlayer>
#include <QString>
#include <QDir>

//Constructor
Alarm::Alarm(QObject *parent) : QObject(parent)
{
    media = new QMediaPlayer(this);

    this->_DefaultPath = "alarm.mp3";
    this->_isPlaying = false;
    this->_Pause = new QTimer(this);
    this->canResume = true;

    connect(this->_Pause, SIGNAL(timeout()), this, SLOT(Resume()));
}

// Starts the alarm playback when alarm triggered
void Alarm::Start(bool useCustom)
{
    if (useCustom)
    {
        media->setMedia(QUrl::fromLocalFile(this->_CustPath));
        this->UsingCustomPath = true;
    }
    else
    {
        media->setMedia(QUrl::fromLocalFile(this->_DefaultPath));
        this->UsingCustomPath = false;
    }
    media->play();
    connect(media, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(RepeatAllTheThings(QMediaPlayer::MediaStatus)));
    this->_isPlaying = true;
}

// Stops the alarm audio playback
void Alarm::Stop()
{
    media->stop();
    this->_Pause->start(60000);
    this->_isPlaying = false;
}

// Repeats audio playback at the end of audio
void Alarm::RepeatAllTheThings(QMediaPlayer::MediaStatus state)
{
    if (state == QMediaPlayer::EndOfMedia)
    {
        if (this->UsingCustomPath)
        {
            media->setMedia(QUrl::fromLocalFile(this->_CustPath));
        }
        else
        {
            media->setMedia(QUrl::fromLocalFile(this->_DefaultPath));
        }
        media->play();
    }
}


// Returns if the audio is being played
bool Alarm::isPlaying()
{
    return this->_isPlaying;
}

// Sets custom audio file path
void Alarm::SetCustomPath(QString CustPath)
{
    this->_CustPath = CustPath;
}

// Resumes audio playback at the trigger of snooze
void Alarm::Resume()
{
    this->canResume = true;
    this->_Pause->stop();
}

// Sets audio playback volume
void Alarm::SetVolume(int Volume)
{
    media->setVolume(Volume);
}
