/*
 * Copyright 2020 KT AI Lab.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

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
