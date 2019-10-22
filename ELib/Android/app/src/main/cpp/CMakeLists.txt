# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

#该变量为真时会创建完整版本的Makefile
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D SUPPORT_OPENGL_ES")


#定义变量ocvlibs使后面的命令可以使用定位具体的库文件
set(opencvlibs "D:/DevelopProj/OpenCV/opencv-4.1.0-android-sdk/OpenCV-android-sdk/sdk/native/libs")
set(ProjectDir "D:/DevelopProj/Education/EProject/ELib/")

#调用头文件的具体路径
include_directories(D:/DevelopProj/OpenCV/opencv-4.1.0-android-sdk/OpenCV-android-sdk/sdk/native/jni/include
        ${ProjectDir}Elib
        ${ProjectDir}Elib/Utility
        ${ProjectDir}Elib/Tangram
        ${ProjectDir}Elib/NativeGraphics
        ${ProjectDir}UnityNativeLoader
        ${ProjectDir}UnityNativeLoader/Unity

        )

#增加我们的动态库
add_library(libopencv_java4 SHARED IMPORTED)

#建立链接
set_target_properties(libopencv_java4 PROPERTIES IMPORTED_LOCATION
        "${opencvlibs}/${ANDROID_ABI}/libopencv_java4.so")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(# Sets the name of the library.
            native-lib

            # Sets the library as a shared library.
            SHARED

            # Provides a relative path to your source file(s).
            native-lib.cpp

            # Native Renderer
            ${ProjectDir}Elib/NativeGraphics/NativeGraphics.cpp
            ${ProjectDir}Elib/NativeGraphics/RenderAPI.cpp
            ${ProjectDir}Elib/NativeGraphics/RenderAPI_OpenGLCoreES.cpp

            ${ProjectDir}UnityNativeLoader/UnityNativeLoader.cpp

            #Tangram lib
            ${ProjectDir}Elib/Tangram/TangramData.cpp
            ${ProjectDir}Elib/Tangram/TangramDetection.cpp
            ${ProjectDir}Elib/Tangram/TangramStates.cpp
            ${ProjectDir}Elib/Tangram/TangramUtility.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                        native-lib
                        libopencv_java4
                        # library for opengl unified
                        GLESv2

                        # Links the target library to the log library
                        # included in the NDK.
                        ${log-lib} )