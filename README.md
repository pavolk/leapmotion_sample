# Leap Motion Sample Application

A sample application using Leap-SDK's [LeapC-API](https://developer.leapmotion.com/documentation/v4/index.html) and OpenGL (freeglut, glew, glm) to render the hand and finger positions.

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

    1. Setting up the conan repositories and installing the dependencies 

        ```
        conan remote add inria https://api.bintray.com/conan/inria/conan
        conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
        conan install -g cmake -s build_type=Debug --build=missing ..
        ```

    1. Installing Leap-SDK

        Download [SDK](https://developer.leapmotion.com/sdk-leap-motion-controller/). Unzip the archive file and set the ```LeapSDK_DIR``` environment variable e.g. like this.

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
