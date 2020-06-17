# 요구 CMake 최소 버전
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )
 
# 프로젝트 이름 및 버전
PROJECT ( "g-sdk-linux-ws-sample-app" )
SET ( PROJECT_VERSION_MAJOR 0 )
SET ( PROJECT_VERSION_MINOR 1 )
 
# 빌드 형상(Configuration) 및 주절주절 Makefile 생성 여부
SET ( CMAKE_BUILD_TYPE Debug)
SET ( CMAKE_VERBOSE_MAKEFILE true )
 
# 빌드 대상 바이너리 파일명 및 소스 파일 목록
SET ( OUTPUT_ELF
        testapp
        )

SET ( SRC_FILES
        GinsideHandler.cpp
        VLCPlayer.cpp
        sample_main.cpp
        )
 
# 공통 컴파일러
SET ( CMAKE_C_COMPILER "gcc" )
 
# 공통 헤더 파일 Include 디렉토리 (-I)
INCLUDE_DIRECTORIES (/usr/local/ginsidews/include)
 

# 공통 컴파일 옵션, 링크 옵션
ADD_COMPILE_OPTIONS ( -std=c++11 -Wall -DLOG_TO_STDOUT=1 -pthread)
SET ( CMAKE_EXE_LINKER_FLAGS "-std=c++11 -Wall -DLOG_TO_STDOUT=1 -pthread" )
 
# 공통 링크 라이브러리 (-l)
LINK_LIBRARIES(ginsidews boost_system crypto ssl cpprest asound vlc KwsRnet)
 
# 공통 링크 라이브러리 디렉토리 (-L)
LINK_DIRECTORIES (/usr/local/ginsidews/lib )
 
# "Debug" 형상 한정 컴파일 옵션, 링크 옵션
SET ( CMAKE_C_FLAGS_DEBUG "-g" )
SET ( CMAKE_EXE_LINKER_FLAGS_DEBUG " " )
 
# "Release" 형상 한정 컴파일 옵션, 링크 옵션
SET ( CMAKE_C_FLAGS_RELEASE "" )
SET ( CMAKE_EXE_LINKER_FLAGS_RELEASE "" )

# 출력 디렉토리
SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE} )
SET ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/lib )
SET ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/lib )
 
# 빌드 대상 바이너리 추가
ADD_EXECUTABLE( ${OUTPUT_ELF} ${SRC_FILES} )
