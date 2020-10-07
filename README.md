# hello Leap Motion

Simple demo application using Leap-SDK's LeapC-API and opengl (freeglut, glew) to render the hand and finger positions.

This is an adapted copy of the "GLUTSample" application from the Leap-SDK.

# Building on Windows

## Install Prerequisites

Supposing the ```scoop``` package manager and the ```Visual Studio``` is already installed on the host machine.

```
scoop install cmake conan
```

## Build

1. Prepare

    ```
    mkdir build
    cd build
    ```

1. Install dependencies

    1. 

    ```
    conan remote add inria https://api.bintray.com/conan/inria/conan
    conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
    conan install -g cmake -s build_type=Debug --build=missing ..
    ```

    1. Installing Leap-SDK

    Download [SDK](https://www2.leapmotion.com/v4.1-lmc-windows-sdk). Unzip the archive file and set the ```LeapSDK_DIR``` environment variable e.g. like this.

    ```
    set LeapSDK_DIR=<ARCHIVE_ROOT>/LeapSDK
    ```

1. Configure

    ```
    cmake .. 
    ```

1. Build

    ```
    cmake --build .
    ```

# Run