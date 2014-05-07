/* 
 * File:   MediaPlayer.cpp
 * Author: Lukas
 * 
 * Created on 07. Mai 2014, 22:35
 */

#include "MediaPlayer.h"

MediaPlayer::MediaPlayer()
{ 
    m_volume_ = 75;
}

MediaPlayer::~MediaPlayer(){ }

void MediaPlayer::playURL(std::string url)
{
    if(m_child_pid_ != 0)
        stop();
    
    m_child_pid_ = fork();
    if (m_child_pid_ == 0)
    {
        struct rlimit fds_limit;
        int max_fd = 1024;
        if (getrlimit(RLIMIT_NOFILE, &fds_limit) == 0) max_fd = fds_limit.rlim_cur;
        for (int i = 0; i <= max_fd; ++i)
        {
            if (i != STDERR_FILENO && i != STDOUT_FILENO && i != STDIN_FILENO) close(i);
        }

        std::string playerstr("/usr/bin/mplayer");
        
        char** args = new char*[3];
        char* player = new char[17];
        memset(player, 0, 17);
        std::copy(playerstr.begin(), playerstr.end(), player);
        args[0] = player;
        char* arg = new char[url.size() + 1];
        memset(arg, 0, url.size() + 1);
        std::copy(url.begin(), url.end(), arg);
        args[1]= arg;
        args[2] = NULL;

        if (execv((char*)"/usr/bin/mplayer", args) == -1)
        {
            _exit(-1);
        }

        _exit(0);
    }
}

void MediaPlayer::stop()
{
    kill(m_child_pid_, SIGTERM);
    int status = 0;
    pid_t retval = waitpid(m_child_pid_, &status, 0);
    m_child_pid_ = 0;
}

void MediaPlayer::increaseVolume()
{
    m_volume_ += 5;
    std::stringstream ss;
    ss << m_volume_ << "%";
    amixerCall(ss.str());
}

void MediaPlayer::decreaseVolume()
{
    m_volume_ -= 5;
    std::stringstream ss;
    ss << m_volume_ << "%";
    amixerCall(ss.str());
}

void MediaPlayer::mute()
{
    amixerCall("mute");
}

void MediaPlayer::unmute()
{
    amixerCall("unmute");
    std::stringstream ss;
    ss << m_volume_ << "%";
    amixerCall(ss.str());
}

void MediaPlayer::amixerCall(std::string arg)
{
    std::vector<std::string> v_args;
    v_args.push_back("/usr/bin/amixer");
    v_args.push_back("set");
    v_args.push_back("PCM");
    v_args.push_back(arg);
    
    pid_t child_pid = fork();
    if (child_pid == 0)
    {
        struct rlimit fds_limit;
        int max_fd = 1024;
        if (getrlimit(RLIMIT_NOFILE, &fds_limit) == 0) max_fd = fds_limit.rlim_cur;
        for (int i = 0; i <= max_fd; ++i)
        {
            if (i != STDERR_FILENO && i != STDOUT_FILENO && i != STDIN_FILENO) close(i);
        }

        char** args = new char*[v_args.size() + 1];
        args[v_args.size()] = NULL;
        int idx = 0;
        for (std::vector<std::string>::iterator iter = v_args.begin(); iter != v_args.end(); ++iter)
        {
            char* arg = new char[iter->size() + 1];
            memset(arg, 0, iter->size() + 1);
            std::copy(iter->begin(), iter->end(), arg);
            args[idx++] = arg;
        }

        if (execv((char*)"/usr/bin/amixer", args) == -1)
        {
            _exit(-1);
        }

        _exit(0);
    }

    int status = 0;
    pid_t retval = waitpid(child_pid, &status, 0);
}
