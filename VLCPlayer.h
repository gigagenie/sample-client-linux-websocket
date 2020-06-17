#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vlc/vlc.h>
#include <chrono>

class VLCPlayer{

    private:
        libvlc_instance_t * inst=nullptr;
        libvlc_media_player_t *mp=nullptr;
        libvlc_media_t *m=nullptr;

        std::chrono::time_point<std::chrono::steady_clock> play_start_time;
        std::chrono::time_point<std::chrono::steady_clock> play_end_time;

        void init();
        void finalize();

    public:
        VLCPlayer();
        ~VLCPlayer();
        
        int stop();
        int play(const std::string& play_url);
        int pause();
        int resume();
        long getDuration();
};