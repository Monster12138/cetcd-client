/******************************************************************************************
┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
│Esc│   │ F1│ F2│ F3│ F4│ │ F5│ F6│ F7│ F8│ │ F9│F10│F11│F12│ │P/S│S L│P/B│  ┌┐    ┌┐    ┌┐
└───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘  └┘    └┘    └┘
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
│~ `│! 1│@ 2│# 3│$ 4│% 5│^ 6│& 7│* 8│( 9│) 0│_ -│+ =│ BacSp │ │Ins│Hom│PUp│ │N L│ / │// │ - │
├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
│ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{ [│} ]│  |  │ │Del│End│PDn│ │ 7 │ 8 │ 9 │   │
├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤ + │
│ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│ Enter  │               │ 4 │ 5 │ 6 │   │
├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤
│ Shift  │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│  Shift   │     │ ↑ │     │ 1 │ 2 │ 3 │   │
├─────┬──┴─┬─┴──┬┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤ E││
│ Ctrl│    │Alt │         Space         │ Alt│    │    │Ctrl│ │ ← │ ↓ │ → │ │   0   │ . │←─┘│
└─────┴────┴────┴───────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
# @Author       : Guo Lei
# @Created Time : 2019-06-21 19:00:55
# @Description  : 
******************************************************************************************/

#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "cJSON/include/cJSON.h"

#define HOST_BUF_SIZE       1024
#define URL_BUF_SIZE        2048
#define BODY_BUF_SIZE       1024
#define DEBUG               1

size_t process_response(void *buffer, size_t size, size_t nmemb, void *arg)
{
    cJSON *json, *item;
    json = cJSON_Parse((const char *)buffer);
    if(json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(0);
    }

#if DEBUG
    char *out = cJSON_Print(json);
    printf("%s\n", out);
#endif

    item = cJSON_GetObjectItemCaseSensitive(json, "errorCode");
    if(cJSON_IsNumber(item) && item->valueint > 0) {
        item = cJSON_GetObjectItemCaseSensitive(json, "message");
        if(cJSON_IsString(item) && item->valuestring != NULL) {
            printf("%s\n", item->valuestring);
        }

        cJSON_Delete(json);
        exit(0);
    }
    printf("OK\n");

    cJSON *object = cJSON_GetObjectItemCaseSensitive(json, "node");
    item = cJSON_GetObjectItem(object, "key");
    if(cJSON_IsString(item) && item->valuestring != NULL) {
        printf("%s: %s\n", "key", item->valuestring);
    }

    cJSON *prev_object = NULL;
    prev_object = cJSON_GetObjectItemCaseSensitive(json, "prevNode");
    if(!cJSON_IsNull(prev_object)) {
        item = cJSON_GetObjectItem(prev_object, "value");
        if(cJSON_IsString(item) && item->valuestring != NULL) {
            printf("%s: %s -> ", "old_value", item->valuestring);
        }
    }

    item = cJSON_GetObjectItem(object, "value");
    if(cJSON_IsString(item) && item->valuestring != NULL) {
        printf("%6s: %s\n", "value", item->valuestring);
    }

    cJSON_Delete(json);

    return 0;
}

int main(int argc, char **argv)
{
    if(argc != 4) {
        printf("usage: %s etcd-server-addr key value\n", argv[0]);
        return 1;
    }
    char *addr = argv[1];
    char *key = argv[2];
    char *value = argv[3];

    char host[HOST_BUF_SIZE] = {0};
    sprintf(host, "Host: %s", addr);

    char url[URL_BUF_SIZE] = {0};
    sprintf(url, "http://%s/v2/keys/%s", addr, key);

    char body[BODY_BUF_SIZE] = {0};
    sprintf(body, "value=%s", value);

    curl_global_init(CURL_GLOBAL_ALL);
    CURL * c = curl_easy_init();
    assert(c != NULL);
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, host);
    //headers = curl_slist_append(headers, "Content-Length: 10");
    //headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    curl_easy_setopt(c, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(c, CURLOPT_URL, url);
    curl_easy_setopt(c, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(c, CURLOPT_POSTFIELDSIZE, strlen(body));
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, process_response);

    CURLcode ret = curl_easy_perform(c);

    curl_easy_cleanup(c);
    curl_global_cleanup();

    if (ret != CURLE_OK) {
        curl_easy_strerror(ret);
        return 1;
    }

    return 0;

}

