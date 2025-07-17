
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <yaml.h>

#include "shadow_cli.h"

char URL_CONTENT[10 * 1024 * 1024];

// 回调函数，用于处理接收到的数据
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t total_size = size * nmemb;
  // 将接收到的数据写入标准输出
  // fwrite(contents, size, nmemb, stdout);
  memcpy((URL_CONTENT + strlen(URL_CONTENT)), contents, nmemb);
  return total_size;
}

int fetchFromUrl(char* serverListFileName) {
  CURL *curl;
  CURLcode res;
  int retValue = 0;
  char* fileName = serverListFileName;
  FILE* serverListFile;

  memset(URL_CONTENT, 0, sizeof(URL_CONTENT));

  // 初始化CURL库
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if(curl) {
    // 设置目标URL
    curl_easy_setopt(curl, CURLOPT_URL, get_config(CFG_URL));
    
    // 设置自定义的User-Agent头
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: clash");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 设置回调函数以处理响应数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    // 执行请求
    res = curl_easy_perform(curl);

    // 检查请求是否成功
    if(res == CURLE_OK) {
      // 清理头部列表
      curl_slist_free_all(headers);
      // 释放CURL句柄
      curl_easy_cleanup(curl);
      //
      // printf(URL_CONTENT);

      serverListFile = fopen(fileName, "w+");
      if (serverListFile == NULL) {
	printf("无法打开文件 %s\n", fileName);
      } else {
	fputs(URL_CONTENT, serverListFile);
	fclose(serverListFile);
	printf("文件已转存至: %s\n", fileName);
      }
    } else {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      retValue = -1;
    }
  } else {
    fprintf(stderr, "无法初始化CURL会话。");
    retValue = -1;
  }

  // 清理CURL全局环境
  curl_global_cleanup();
  return retValue;
}
