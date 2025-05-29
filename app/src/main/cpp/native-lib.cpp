#include <jni.h>
#include <string>
#include <curl/curl.h>
#include <android/log.h>

#define TAG "NativeCurl"

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void showToastSafe(JNIEnv* env, jobject context, const std::string& message) {
    jclass contextClass = env->GetObjectClass(context);
    jmethodID getAppCtx = env->GetMethodID(contextClass, "getApplicationContext", "()Landroid/content/Context;");
    jobject appCtx = env->CallObjectMethod(context, getAppCtx);

    jclass toastClass = env->FindClass("android/widget/Toast");
    jmethodID makeText = env->GetStaticMethodID(toastClass, "makeText",
        "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
    jmethodID show = env->GetMethodID(toastClass, "show", "()V");

    jstring jmsg = env->NewStringUTF(message.c_str());
    jobject toast = env->CallStaticObjectMethod(toastClass, makeText, appCtx, jmsg, 1);
    env->CallVoidMethod(toast, show);

    env->DeleteLocalRef(jmsg);
    env->DeleteLocalRef(toast);
}

extern "C" JNIEXPORT void JNICALL
Java_com_dev_ron_MainActivity_curlTest(JNIEnv* env, jobject thiz) {
    JavaVM* javaVM;
    env->GetJavaVM(&javaVM);

    JNIEnv* newEnv = nullptr;
    javaVM->AttachCurrentThread(&newEnv, nullptr);

    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://servertest-c309.onrender.com/verify");

        // POST method
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // JSON body
        const char* jsonData = "{\"key\":\"ron\",\"device_id\":\"TestingJust\"}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);

        // Set headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Callback to capture response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // SSL options
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            response = "❌ CURL error: " + std::string(curl_easy_strerror(res));
        } else {
            response = "✅ Curl POST Success:\n" + response;
        }

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        response = "❌ Failed to init libcurl!";
    }

    curl_global_cleanup();

    showToastSafe(newEnv, thiz, response);

    javaVM->DetachCurrentThread();
}