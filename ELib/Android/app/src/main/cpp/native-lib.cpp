#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <cstdlib>

extern "C" JNIEXPORT jstring JNICALL
Java_com_bhgame_elib_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    cv::Mat mat = cv::Mat::zeros(cv::Size(100,100),CV_8UC1);
    char buffer [33];
    sprintf (buffer , "-- %d %d",mat.size().height,mat.channels());

    std::string h(buffer);
    hello += h;
    return env->NewStringUTF(hello.c_str());
}
