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

#include "WsConGinside.h"
#include "VLCPlayer.h"

class GinsideHandler: public WsConGinside{
    private:
        volatile bool  playmd=false;
        volatile bool  voc_rec_stop=false;
        volatile int  cur_channel=-1;
        VLCPlayer player;

        std::thread rec_thread;
    public:
        GinsideHandler():WsConGinside(){};
        GinsideHandler(const std::string& clientid,const std::string& clientkey,const std::string& clientsecret,const std::string& desturi
                       ,const std::string& authouri,const std::string& clienttype="GINSIDE")
                    :WsConGinside(clientid,clientkey,clientsecret,desturi,authouri,clienttype){};
        
         
        void onCommand(std::string& actionType,std::string& cmd);
       
        void onEvent(int evt,std::string opt="");
       
        void onMediaData(const char* buf,int size);

        // void onCmd(const std::string& srv_cmd_type,int rc,int channel,const std::string& cmd_opt_json,const std::string& payload_json);
        // void onMediaStream(const char* buf,int size);
        // void onErr(std::exception e,int errtype);

        void onVoiceRec();
        /*
        status
        normal = 1000, going_away = 1001, protocol_error = 1002, unsupported = 1003,
        abnormal_close = 1006, inconsistent_datatype = 1007, policy_violation = 1008, too_large = 1009,
        negotiate_error = 1010, server_terminate = 1011
        */
       // void onSessionClosed(int status,const std::string& reason,const std::error_code& errcode);
};
