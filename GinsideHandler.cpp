#include "GinsideHandler.h"
#include "LogMessage.h"
#include <cpprest/json.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <alsa/asoundlib.h>


typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;

//! _data malloc allocation ,so use free
bool _readBinFile(std::string filePath, unsigned char **_data, int *datalen) 
{ 
    std::ifstream is(filePath, std::ifstream::binary); 
    if (is) { 
        // get length of file  
        is.seekg(0, is.end);
        int length = (int)is.tellg(); 

        is.seekg(0, is.beg); 
        unsigned char * buffer = (unsigned char*)malloc(length);
        // read data as a block: 
        is.read((char*)buffer, length);
        is.close(); 
        *_data = buffer; 
        *datalen = length; 
        return true;
    } 
    return false; 
}

//! ap_play_buffer malloc allocation, so use free
int _playSound(unsigned char *ap_play_buffer,int bufsize)
{
    unsigned int freq = 16000; //sampling rate
    snd_pcm_t *p_playback_handle;
    snd_pcm_hw_params_t *hw_params;
    
    if(snd_pcm_open(&p_playback_handle, "default",SND_PCM_STREAM_PLAYBACK,0)<0){
        perror("Cound not open output audio dev");
        return 0;
    }
    // 열린 재생 장치에 재생시 사용할 기준 값을 설정하기 위하여 메모리를 할당한다.
    snd_pcm_hw_params_malloc(&hw_params);
    // 설정 값을 저장할 메모리에 기본(default) 값들을 설정한다.
    snd_pcm_hw_params_any(p_playback_handle,hw_params);
    // 이 재생 장치를 사용하여 send_pcm_readi 또는
    // send_pcm_writei 함수를 사용할 수 있도록 설정한다.
    snd_pcm_hw_params_set_access(p_playback_handle, hw_params,
                                                            SND_PCM_ACCESS_RW_INTERLEAVED);
    // 샘플링 format 부호를 고려하는 2바이트(16비트)로 설정한다.
    // 바이트 정렬 방식은 little endian 으로 한다.
    snd_pcm_hw_params_set_format(p_playback_handle, hw_params,
                                                (snd_pcm_format_t)SND_PCM_FORMAT_S16_LE);
    // 샘플링 주기는 16KHz로 설정한다.
    snd_pcm_hw_params_set_rate_near(p_playback_handle, hw_params, &freq,0);
    // 모노 채널을 사용한다. 1 -> mono
    snd_pcm_hw_params_set_channels(p_playback_handle, hw_params, 1);
    // 재생시 사용할 기준 값들을 p_hw_params에 모두 저장되었기 때문에
    // p_hw_params를 사용하여 재생 장치에 정보를 설정한다.
    snd_pcm_hw_params(p_playback_handle, hw_params);
    // 재생 장치에 기준 값을 설정하기 위해 할당한 메모리를 제거한다.
    snd_pcm_hw_params_free (hw_params);

    printf("play sound\n");
    snd_pcm_prepare(p_playback_handle);

    
    int duration=bufsize/(freq*2);/*byte per sec*/
    int dur_remain=bufsize%(freq*2);
    if(dur_remain>0) duration++; 

    LOG(DEBUG)<<"bufsize:"<<bufsize<<" frequency:"<<freq<<" duration:"<<duration;

    // 재생 시작
    // ap_play_buffer가 가리키는 메모리에 몇개의 샘플데이터를 재생할지 지정 (메모리 사이즈가 아님)
    snd_pcm_writei(p_playback_handle, ap_play_buffer, bufsize/2 /*frame 갯수*/);

    // send_pcm_readi와 달리 send_pcm_writei는  재생을 끝내고 빠져나오는 것이 아니라 즉시 return
    // 재생이 완료될 때까지 프로그램이 종료하지 않도록 대기한다.
    //usleep(5000000);
    // wait... 5 sec
    
    sleep(duration);

    LOG(DEBUG)<<"play complete\n";
    snd_pcm_drop(p_playback_handle); // 재생 중지
    snd_pcm_close(p_playback_handle); // 재생 장치 닫기

    return duration;
}

// constexpr unsigned int _hashCode(const char* str)
// {
//     return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * _hashCode(str + 1) : 8603;
// }

constexpr unsigned int _hashCode(const char* str)
{
    return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * _hashCode(str + 1) : 8603;
}

void GinsideHandler::onVoiceRec()
{
    char *buffer;
    int err=0;
    int buffer_frames = 160; //frame size , 1/100 sec 
    unsigned int rate = 16000;//16K;
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE; //16bit Little endian

    if ((err = snd_pcm_open (&capture_handle,"default", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf (stderr, "cannot open audio device %s (%s)\n",
                "default",
                snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "audio interface opened\n");

    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
                snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params allocated\n");

    if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
                snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params initialized\n");

    if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf (stderr, "cannot set access type (%s)\n",
                snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params access setted\n");

    if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, format)) < 0) {
        fprintf (stderr, "cannot set sample format (%s)\n",
                snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params format setted\n");

    if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
        fprintf (stderr, "cannot set sample rate (%s)\n",
                snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params rate setted\n");

    if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 1)) < 0) {
        fprintf (stderr, "cannot set channel count (%s)\n",
                snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params channels setted\n");

    if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n",
                snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "hw_params setted\n");

    snd_pcm_hw_params_free (hw_params);

    fprintf(stdout, "hw_params freed\n");

    if ((err = snd_pcm_prepare (capture_handle)) < 0) {
        fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
                snd_strerror (err));
        exit (1);
    }

    fprintf(stdout, "audio interface prepared\n");

    int chunk_size=buffer_frames*snd_pcm_format_width(format)/8;
    buffer = (char*)malloc(chunk_size); //320byte

    fprintf(stdout, "buffer allocated\n");

    fprintf(stdout, "ready to capture voice\n");

    std::ofstream output("voice.pcm", std::ios::out | std::ios::binary);

    //record 320 byte audio per one loop// 16000*2 byte per sec // total 30sec
    for (int i = 0; i < 3000; ++i) {

        if(voc_rec_stop || playmd) break; //when get stopvoice msg or playing media status then stop recording
        
        if ((err = snd_pcm_readi (capture_handle, buffer, buffer_frames)) != buffer_frames) {
        fprintf (stderr, "read from audio interface failed (%s)\n",
                err, snd_strerror (err));
        exit (1);
        }
        fprintf(stdout, "read %d done\n", i);
        fprintf(stdout, "file write (%d) done size:%d\n", i,buffer_frames*snd_pcm_format_width(format)/8);

        agent_sendVoice((const char*)buffer,chunk_size);

        output.write(buffer,chunk_size);
    }

    free(buffer);
    output.close();

    fprintf(stdout, "buffer freed\n");

    snd_pcm_close (capture_handle);
    fprintf(stdout, "audio interface closed\n");

}

void GinsideHandler::onCommand(std::string& actionType,std::string& cmd)
{
    auto payload_val=JsonValue::parse(cmd);
    LOG(DEBUG)<<"payload parsing complete";
    int rc=-9999;
    auto rc_val=payload_val["rc"];
    std::string cmd_opt_json;
    int channel=-9999;
    if(!rc_val.is_null() && rc_val.is_integer()) rc=rc_val.as_integer();
    
    auto cmd_opt_val=payload_val["cmdOpt"];
    if(!cmd_opt_val.is_null() && cmd_opt_val.is_object())
    {
        cmd_opt_json=cmd_opt_val.to_string();

        auto channel_val=cmd_opt_val["channel"];

        if( !channel_val.is_null() && channel_val.is_integer())
        {
            channel=channel_val.as_integer();
        }
    }



    switch(_hashCode(actionType.c_str()))
    {
            case _hashCode(ACTION_TYPE_STOP_VOICE): 
            {
                voc_rec_stop=true; 
            }
            break;

             case _hashCode(ACTION_TYPE_CONTROL_MEDIA): 
            {
                cur_channel=channel;
                std::string url; 
                std::string action;

                auto cmd_opt_val=JsonValue::parse(cmd_opt_json);

                if(!cmd_opt_val.is_null() && cmd_opt_val.is_object())
                {
                    auto act_val=cmd_opt_val["act"];

                    if( !act_val.is_null() && act_val.is_string())
                    {
                        action=act_val.as_string();
                        LOG(DEBUG)<<"action:"<<action;
                        
                        if(action.compare("pause")==0)
                        {
                            if(player.pause()==0)
                            {
                                // if pause success? 
                                agent_updateMediaStatus(cur_channel,"paused",0);
                                //agetnsendCmdUpdateMediaState(std::to_string(cur_channel),"paused","0");
                            }

                        } 
                        if(action.compare("resume")==0) 
                        {
                            if(player.resume()==0)
                            {
                                // if resume success? 
                                agent_updateMediaStatus(cur_channel,"started",0);
                                //sendCmdUpdateMediaState(std::to_string(cur_channel),"started","0");
                            }
                        }
                        
                        if(action.compare("stop")==0)
                        {

                            if(player.stop()==0)
                            {
                                 // if stop success? 
                                long play_time=player.getDuration();
                                
                                agent_updateMediaStatus(cur_channel,"stopped",play_time);
                               // sendCmdUpdateMediaState(std::to_string(cur_channel),"stopped",std::to_string(play_time));
                            }
                        } 
                    }
                }
            }
            break;

            case _hashCode(ACTION_TYPE_PLAY_MEDIA): 
            {
                voc_rec_stop=true; 
                //save chanel info
                LOG(DEBUG)<<"channel:"<<channel;
                cur_channel=channel;
                std::string url; 
                std::string action;

                auto cmd_opt_val=JsonValue::parse(cmd_opt_json);

                if(!cmd_opt_val.is_null() && cmd_opt_val.is_object())
                {
                    auto url_val=cmd_opt_val["url"];

                    if( !url_val.is_null() && url_val.is_string())
                    {
                        url=url_val.as_string();
                        LOG(DEBUG)<<"url:"<<url;
                    }

                    auto act_val=cmd_opt_val["actOnOther"];

                     if(!act_val.is_null() && act_val.is_string())
                    {
                        action=act_val.as_string();
                        LOG(DEBUG)<<"action:"<<action;
                        
                        if(action.compare("pause")==0)
                        {
                            if(player.pause()==0)
                            {
                                // if pause success? 
                                agent_updateMediaStatus(cur_channel,"paused",0);
                                //sendCmdUpdateMediaState(std::to_string(cur_channel),"paused","0");
                            }

                        } 
                        if(action.compare("resume")==0) 
                        {
                            if(player.resume()==0)
                            {
                                // if resume success? 
                                agent_updateMediaStatus(cur_channel,"started",0);
                                //sendCmdUpdateMediaState(std::to_string(cur_channel),"started","0");
                            }
                        }
                        
                        if(action.compare("stop")==0)
                        {
                            if(player.stop()==0)
                            {
                                 // if stop success? 
                                int play_time=player.getDuration();
                                agent_updateMediaStatus(cur_channel,"stopped",play_time/1000);
                                //sendCmdUpdateMediaState(std::to_string(cur_channel),"stopped",std::to_string(play_time/1000));
                            }
                        } 

                    }

                }
                if(!url.empty()) //hls audio streaming
                {
                    agent_updateMediaStatus(cur_channel,"started",0);
                    //sendCmdUpdateMediaState(std::to_string(cur_channel),"started","0");
                    player.play(url);
                }

            }
            break;

            case _hashCode(ACTION_TYPE_START_VOICE): 
            {
                LOG(DEBUG)<<"== handling Req_STRV ===";
                
                //is server ready?
                // VLOG(3)<<"server command -> msgPayload>>rc :"<<rc;

                // if(rc==200) //if server ready to get voice data
                {
                    voc_rec_stop=false; //set enable voice recoder
                    rec_thread=std::thread(&GinsideHandler::onVoiceRec,this);
                    rec_thread.detach();
                }
            }
            break;

        default:
        break;
 }

}

/* handler for audio data from server */
void GinsideHandler::onMediaData(const char* buf,int size)
{
    playmd=true;
    
    LOG(DEBUG)<<"onMediaStream called :"<<buf[0]<<","<<buf[1]<<","<<buf[2]<<","<<buf[3]<<","
                                    <<buf[4]<<","<<buf[5]<<","<<buf[6]<<","<<buf[7]<<","
                                    <<buf[8]<<","<<buf[9]<<","<<buf[10]<<","<<buf[11]<<"...";

    LOG(DEBUG)<< "got some voice stream & save it to file";
    std::ofstream output("response.wav", std::ios::out | std::ios::binary);
    output.write(buf,size);
    output.close();

    agent_updateMediaStatus(cur_channel,"started",0);
    //sendCmdUpdateMediaState(std::to_string(cur_channel),"started","0");

    unsigned char* pBuf;
    int buf_size=0;
    _readBinFile("response.wav",&pBuf,&buf_size);
    int duration_sec=_playSound(&pBuf[44],buf_size-44); //skip the wav header, assume that wav file is 16000hz 16bit mono format

    LOG(DEBUG)<< "buf size:"<<buf_size<<",play time:"<<duration_sec;

    agent_updateMediaStatus(cur_channel,"complete",duration_sec);
    //sendCmdUpdateMediaState(std::to_string(cur_channel),"complete",std::to_string(duration_sec));

    playmd=false;
    free(pBuf);
}

 void GinsideHandler::onEvent(int evt,std::string opt)
 {
     LOG(INFO)<<"evt:"<<evt<<", opt:"<<opt;

    switch(evt){
        case EVT_GOTO_STANDBY:
            LOG(INFO)<<"evt:"<<"goto standby";
        break;
        case EVT_DISCONNECTED:
            LOG(INFO)<<"evt:"<<"disconnected";
        break;
        case EVT_SEVER_ERROR:
            LOG(INFO)<<"evt:"<<"server error";
        break;
        case EVT_INIT_FAIL:
            LOG(INFO)<<"evt:"<<"sdk init failed";
        break;
    }

 }

//  void GinsideHandler::onSessionClosed(int status,const std::string& reason,const std::error_code& errcode){
//     LOG(INFO) << "connection closed, reason: " << reason 
//              << " close status: " << status << " error code " << errcode << " user stopped:"<<is_user_stopped;
//  }


// void GinsideHandler::onErr(std::exception e,int errtype=-1){
//     LOG(DEBUG)<< "onErr called";
// }
