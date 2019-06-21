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
# @Created Time : 2019-06-22 01:07:39
# @Description  : 
******************************************************************************************/


#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(int argc, char **argv)
{
    if(argc != 3) {
        printf("usage: %s etcd-server-addr key\n", argv[0]);
        return 1;
    }
    char *addr = argv[1];
    char *key = argv[2];

    char host[1024] = {0};
    sprintf(host, "Host: %s", addr);

    char url[1024] = {0};
    sprintf(url, "http://%s/v2/keys/%s", addr, key);

    curl_global_init(CURL_GLOBAL_ALL);
    CURL * c = curl_easy_init();
    assert(c != NULL);
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, host);
    //headers = curl_slist_append(headers, "Content-Length: 10");
    //headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(c, CURLOPT_URL, url);

    CURLcode ret = curl_easy_perform(c);
    if (ret != CURLE_OK) {
        curl_easy_strerror(ret);
    }

    curl_easy_cleanup(c);
    curl_global_cleanup();
}

