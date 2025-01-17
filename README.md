# sample-client-linux-websocket
Linux Sample Client (using g-sdk-linux-ws lib)

## Prerequisites

### 인사이드 디바이스 키 발급
    1. [API Link](https://apilink.kt.co.kr) 에서 회원가입 
    2. 사업 제휴 신청 및 디바이스 등록 (Console > GiGA Genie > 인사이드 디바이스 등록)
    3. 디바이스 등록 완료 후 My Device에서 등록한 디바이스 정보 및 개발키 발급 확인 (Console > GiGA Genie > My Device)

### 개발 환경
*   OS: Linux Ubuntu (16.04 > 권장)
*   Build: cmake,gcc (c++ standard 11)


## Quick Start

### 1. 필요한 라이브러리 설치
* [g-inside sdk websocket library](https://github.com/gigagenie/ginside-sdk/tree/master/g-sdk-linux-ws)
  
    라이브러리와 헤더파일 다운로드후 아래폴더로 복사
    
    
    ```shell
    $ sudo mkdir /usr/local/ginsidews
  $ sudo cp -r inc/ /usr/local/ginsidews/include
  $ sudo cp -r lib/ubuntu-x86_64/ /usr/local/ginsidews/lib
  $ sudo ldconfig
  ```
  
  >위에서 라이브러리와 헤더파일의 복사 위치를 다른폴더로 변경할경우는 CMakeLists.txt의INCLUDE_DIRECTORIES,LINK_DIRECTORIES 도 함께 변경이 필요함)
  
* [cpprest library](https://github.com/microsoft/cpprestsdk)

    ```shell
    $ apt-get install libcpprest-dev
    ```
    
* [ALSA library](https://www.alsa-project.org/wiki/Main_Page)

    ```shell
    $ sudo apt-get install libasound2-dev
    ```
    
* [VLC Media Player](https://www.videolan.org/index.ko.html)

    ```shell
    $ sudo apt install vlc
    ```
    
* [VLC library](https://www.videolan.org/vlc/libvlc.html)

    ```shell
    $ sudo apt-get install libvlc-dev
    ```

* 인사이드 디바이스 키 설정 (발급은 [인사이드 디바이스 키 발급](https://gitlab.gigagenie.ai/gigagenie-inside/g-sdk-linux-ws#%EC%9D%B8%EC%82%AC%EC%9D%B4%EB%93%9C-%EB%94%94%EB%B0%94%EC%9D%B4%EC%8A%A4-%ED%82%A4-%EB%B0%9C%EA%B8%89))

### 2. sample_main.cpp 파일에서 아래 내용 실제 발급받은 키값으로수정

`ggi_api.agent_register(id,key,secret);`

### 3. Build & Run

* cmake 명령으로 우선 make 파일을 생성 (재빌드시는 cmake 재수행 없이 make 명령만 수행)

    ```shell
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    ```
    
* 실행

    ```shell
    $cd Debug
    $./testapp
    ```
    
* Release/Debug 빌드설정 

    CMakeList.txt의 SET ( CMAKE_BUILD_TYPE *Debug*) 항목을 Release 혹은 Debug로 수정



## License

sample-client-linux-websocket is licensed under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)