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

#ifndef __LOG_MESSAGE__

#define __LOG_MESSAGE__

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

namespace da{

class LogMessage{
    std::string level;
    std::ostream &ofs;
    static bool enable;
    public:
        LogMessage(const std::string &l);
        LogMessage(std::ostream &o);
        inline std::ostream &stream(){return ofs;}
        ~LogMessage();
        static void Enable(bool _enable);
};

}

#define FLAGS_v 3

#define LOG(type)        da::LogMessage(#type).stream()
#define DLOG(type)       da::LogMessage(#type).stream()
#define VLOG(level)      if ((level) <= FLAGS_v) LOG(VERVOCE)

#define LOG_LINE_PRINT LOG(DEBUG)<<"File:"<<__FILE__<<" Line:"<<__LINE__<<std::endl
#define CHECK(x)         if (x) {} else LOG(ERROR) << #x
#define DCHECK(x)        CHECK(x)

#define CHECK_EQ(x, y)   CHECK((x) == (y))
#define CHECK_LT(x, y)   CHECK((x) < (y))
#define CHECK_GT(x, y)   CHECK((x) > (y))
#define CHECK_LE(x, y)   CHECK((x) <= (y))
#define CHECK_GE(x, y)   CHECK((x) >= (y))
#define CHECK_NE(x, y)   CHECK((x) != (y))
#define CHECK_NOTNULL(x) \
    ({ \
        CHECK_NE(x, NULL); \
        (x); \
    })

#define DCHECK_EQ(x, y)  DCHECK((x) == (y))
#define DCHECK_LT(x, y)  DCHECK((x) < (y))
#define DCHECK_GT(x, y)  DCHECK((x) > (y))
#define DCHECK_LE(x, y)  DCHECK((x) <= (y))
#define DCHECK_GE(x, y)  DCHECK((x) >= (y))
#define DCHECK_NE(x, y)  DCHECK((x) != (y))


#endif 
