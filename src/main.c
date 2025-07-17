
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <yaml.h>

#include <sys/stat.h>

#include "shadow_cli.h"

int main(int argc, char** argv) {
  char* configFileName = "config.yml";
  char* serverListFileName = "tmp/server_list.yml";
  server_info_t* selectedServer;

  server_info_t serverList[200];
  int server_cnt = 200;

  int ret = 0;

  int configRet = loadConfig(configFileName);
  if (configRet != 0) {
    return -1;
  }
  
  // int ret = mkdir("tmp", S_IRWXU);
  mkdir("tmp", S_IRWXU);

  if (strcmp(get_config(CFG_UPDATE), "yes") == 0) {
    printf("开始同步\n");
    fetchFromUrl(serverListFileName);
  } else {
    printf("设置为不同步，直接载入缓存\n");
  }
  
  memset(serverList, 0, sizeof(serverList));
  
  ret = extractServerList(serverListFileName, serverList);
  if (ret != 0) {
    printf("无法从列表中获取服务器信息\n");
    return EXIT_FAILURE;
  }
  // printf("count: %d\n", sizeof(serverList));
  /* for(int i=0; i<server_cnt; i++) { */
  /*   printf("num:%d, name:%s, type:%s, server:%s\n", serverList[i].num, serverList[i].name, serverList[i].type, serverList[i].server); */
  /*   //printf("%s", serverList[i]); */
  /*   //printf("\n"); */
  /* } */

  //server_info_t* selectServerFromList(server_info_t* serverList, int serverCnt);
  while (selectedServer == NULL) {
    selectedServer = selectServerFromList(serverList, server_cnt);
  }
  
  makeConnectCmd(selectedServer, NULL);
  
  return 0;
}
