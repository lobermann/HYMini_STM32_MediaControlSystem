/* 
 * File:   MediaPlayer.h
 * Author: Lukas
 *
 * Created on 07. Mai 2014, 22:35
 */

#ifndef MEDIAPLAYER_H
#define	MEDIAPLAYER_H

#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

class MediaPlayer 
{
public:
    MediaPlayer();
    virtual ~MediaPlayer();
    
    void playURL(std::string url);
    void stop();
    void increaseVolume();
    void decreaseVolume();
    void mute();
    void unmute();
private:
    
    void amixerCall(std::string arg);
    
    pid_t m_child_pid_;
    int m_volume_;
};

#endif	/* MEDIAPLAYER_H */

