#include <jni.h>
#include <string>
#include <curl/curl.h>
#include <android/log.h>

#define LOG_TAG "NativeAPI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_dev.ron_NativeApi_postVerifyData(JNIEnv *env, jobject /* this */) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (!curl) {
        LOGI("Curl init failed");
        return env->NewStringUTF("CURL_INIT_FAILED");
    }

    const char* url = "https://servertest-c309.onrender.com/verify";
    const char* jsonData = R"({"key":"fake_key_123","device_id":"fake_device_456"})";

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        LOGI("curl_easy_perform() failed: %s", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return env->NewStringUTF("REQUEST_FAILED");
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return env->NewStringUTF(readBuffer.c_str());
}