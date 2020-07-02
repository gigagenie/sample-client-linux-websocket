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

#include "VLCPlayer.h"


void VLCPlayer::init()
{
    /* Load the VLC engine */
     inst = libvlc_new (0, NULL);
}

void VLCPlayer::finalize()
{
    if(mp!=nullptr)
    {
        stop();
    }

    libvlc_release (inst);
}


VLCPlayer::VLCPlayer()
{
    init();
}

VLCPlayer::~VLCPlayer()
{
    finalize();
}


int VLCPlayer::stop()
{

    if(mp==nullptr) return -1;
    /* Stop playing */
     libvlc_media_player_stop (mp);
     /* Free the media_player */
     libvlc_media_player_release (mp);
     mp=nullptr;

     /* No need to keep the media now */
    libvlc_media_release (m);
    m=nullptr;

    play_end_time=std::chrono::steady_clock::now();

     return 0;
}

int VLCPlayer::play(const std::string& play_url)
{
    /* Create a new item */
    m = libvlc_media_new_location (inst,play_url.c_str());
    /* Create a media player playing environement */
    mp = libvlc_media_player_new_from_media (m);
  
    /* play the media_player */
     libvlc_media_player_play (mp);

     play_start_time=std::chrono::steady_clock::now();

     return 0;
}
int VLCPlayer::pause()
{
    if(mp==nullptr) return -1;
    libvlc_media_player_set_pause(mp,1);

    return 0;
}
int VLCPlayer::resume()
{
    if(mp==nullptr) return -1;
    libvlc_media_player_set_pause(mp,0);

    return 0;
}

long VLCPlayer::getDuration()
{
    //if(m==nullptr) return -1;
    //return libvlc_media_get_duration (m);
    return std::chrono::duration_cast<std::chrono::seconds>(play_end_time-play_start_time).count();
}
