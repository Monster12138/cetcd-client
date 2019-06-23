/******************************************************************************************
┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
│Esc│   │ F1│ F2│ F3│ F4│ │ F5│ F6│ F7│ F8│ │ F9│F10│F11│F12│ │P/S│S L│P/B│  ┌┐    ┌┐    ┌┐
└───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘  └┘    └┘    └┘
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
│~ `│! 1│@ 2│# 3│$ 4│% 5│^ 6│& 7│* 8│( 9│ ) 0│_ -│+ =│ BacSp │ │Ins│Hom│PUp│ │N L│ / │// │ - │
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
# @Created Time : 2019-06-22 13:52:03
# @Description  : 
******************************************************************************************/
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "cJSON/include/cJSON.h"

#define HOST_BUF_SIZE     1024
#define URL_BUF_SIZE      2048
#define DEBUG             0

int quit = 0;
CURL *c; 

void sighandler(int sig)
{
    curl_easy_cleanup(c);
    curl_global_cleanup();

    exit(0);
}

void response_put(cJSON *json)
{
    cJSON *item;
    if(json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }

    printf("PUT\n");

    item = cJSON_GetObjectItemCaseSensitive(json, "errorCode");
    if(cJSON_IsNumber(item) && item->valueint > 0) {
        item = cJSON_GetObjectItemCaseSensitive(json, "message");
        if(cJSON_IsString(item) && item->valuestring != NULL) {
            printf("%s\n", item->valuestring);
        }

        return;
    }

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
        printf("%s: %s\n", "value", item->valuestring);
    }
}

void response_delete(cJSON *json)
{
    cJSON *item;
    if(json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }

    printf("DELETE\n");

    item = cJSON_GetObjectItemCaseSensitive(json, "errorCode");
    if(cJSON_IsNumber(item) && item->valueint > 0) {
        item = cJSON_GetObjectItemCaseSensitive(json, "message");
        if(cJSON_IsString(item) && item->valuestring != NULL) {
            printf("%s\n", item->valuestring);
        }

        cJSON_Delete(json);
        return;
    }

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
            printf("%s: %s\n", "old_value", item->valuestring);
        }
    }
}

size_t process_response(void *buffer, size_t size, size_t nmemb, void *arg)
{
    cJSON *json, *item;
    json = cJSON_Parse((const char *)buffer);
    if(json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return CURLE_WRITE_ERROR;
    }

#if DEBUG
    char *out = cJSON_Print(json);
    printf("%s\n", out);
#endif

    item = cJSON_GetObjectItem(json, "action");
    if(!(cJSON_IsString(item) && item->valuestring != NULL)) {
        cJSON_Delete(json);
        return CURLE_WRITE_ERROR;
    }

    if(strcmp(item->valuestring, "set") == 0) {
        response_put(json);
    }
    else if(strcmp(item->valuestring, "delete") == 0) {
        response_delete(json);
    }
    cJSON_Delete(json);

    return size * nmemb;
}

int main(int argc, char **argv)
{
    if(argc != 3) {
        printf("usage: %s etcd-server-addr key\n", argv[0]);
        return 1;
    }
    char *addr = argv[1];
    char *key = argv[2];

    char host[HOST_BUF_SIZE] = {0};
    sprintf(host, "Host: %s", addr);

    char url[URL_BUF_SIZE] = {0};
    sprintf(url, "http://%s/v2/keys/%s?wait=true", addr, key);

    curl_global_init(CURL_GLOBAL_ALL);

    c = curl_easy_init();
    assert(c != NULL);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, host);

    curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(c, CURLOPT_URL, url);
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, process_response);
    signal(SIGINT, sighandler);

    while(1)
    {
        CURLcode ret = curl_easy_perform(c);
        if (ret != CURLE_OK) {
            curl_easy_strerror(ret);
            curl_easy_cleanup(c);
            curl_global_cleanup();
            return 1;
        }
    }

    /* nerver run here */
    
    return 0;
}
