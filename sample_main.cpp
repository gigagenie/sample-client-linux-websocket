#include "WsConGinside.h"
#include "GinsideHandler.h"
#include <iostream>
#include "LogMessage.h"
#include <cpprest/json.h>
typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;



void printmenu()
{
      std::cout << "*****************************************"<< std::endl;
      std::cout << " 1 - text command"<<std::endl;
      std::cout << " 2 - voice command"<<std::endl;
      std::cout << " 0 - try to connect & start session again"<<std::endl;
      std::cout << " 98 - get current dev status"<<std::endl;
      std::cout << " 99 - stop listening & stop session"<<std::endl;
      std::cout << " 100 - Exit"<<std::endl;
      std::cout << " Enter your choice and press return: "<<std::endl;
      std::cout << "*****************************************"<< std::endl;
}


int main()
{
    const std::string id="ginside_dev2";
    const std::string key="Y2xpZW50X2tleToxNTgyMjY0MDExOTU5 ";  //! key have a space at the last
    const std::string secret="Y2xpZW50X3NlY3JldDoxNTgyMjY0MDExOTU5";
    const std::string clienttype="GINSIDE";
    const std::string desturi="wss://inside-dev.gigagenie.ai:30109/channel";
    const std::string authouri="https://inside-dev.gigagenie.ai:30109/v2/authorize";

    GinsideHandler ggi_api;
    
    std::string uuid;
    std::ifstream _file("_uuid");
    std::getline(_file,uuid);

    if(uuid.empty()) //if you've never got a uuid before, register your deivce and get it. 
    {
        std::string res=ggi_api.agent_register(id,key,secret);

        auto res_reg_val=JsonValue::parse(res);
        
        int rc;

        if(!res_reg_val.is_null() && res_reg_val.is_object())
        {
            auto rc_val=res_reg_val["rc"];

            if( !rc_val.is_null() && rc_val.is_integer())
            {
                rc=rc_val.as_integer();
                LOG(DEBUG)<<"rc:"<<rc;
            }

            auto uuid_val=res_reg_val["uuid"];

            if( !uuid_val.is_null() && uuid_val.is_string())
            {
                uuid=uuid_val.as_string();
                LOG(DEBUG)<<"uuid:"<<uuid;
            }

            if(rc==200 && !uuid.empty()) //I get uuid sucessfuly, so save it for next connection
            {
                std::ofstream uuidfile;
                uuidfile.open("_uuid");
                uuidfile<<uuid;
                uuidfile.close();
            }
        }
    }
  
    ggi_api.agent_init(id,key,secret,uuid);
  
    int choice=-1;
    bool is_continue = true;
    while (is_continue != false)
    {
      printmenu();

      std::string strin;
      getline(std::cin, strin);

      try{
        choice=stoi(strin);

      }
      catch(std::exception& e)
      {
        continue; //it looks like not an integer...  try again
      }

      switch (choice)
      {
            case 1: //text command
            {
                
                std::cout <<std::endl<< " Enter your message and press return: "<<std::endl;
                std::string strin;
                getline(std::cin, strin);
                ggi_api.agent_sendTxt(strin);
                LOG(DEBUG)<<"==== sent Req_TXCM command ====";
            }
         
            break;
            case 2: // start voice command
            {
                ggi_api.agent_startVoice();
            }
            break;

            case 0: // init
            {
                ggi_api.agent_init(id,key,secret,uuid);
       
            }
            break;

            case 98: //show device status
            {
                for(auto i:ggi_api.getDevSts()) 
                { 
                    LOG(DEBUG)<<i;
                }
            }
            break;

            case 99: //reset 
            {
                ggi_api.agent_reset();
         
            }
            break;

            case 100: //reset & exit
            {
                ggi_api.agent_reset();
           
                is_continue = false;
            }
            break;

            default:
              printmenu();
            break;
      }
  }

    return 0;
}