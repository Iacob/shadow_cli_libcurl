
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shadow_cli.h"

int makeConnectCmd(server_info_t* server_info, char* to_str) {

  char* trojanConfigFileName = "tmp/tj_config.json";
  char cmd[200];

  char* listen_port = CFG_PORT;
  if (strlen(listen_port) < 1) {
    listen_port = "8118";
  }

  printf("服务器类型为: %s\n", server_info->type);
  if (strcmp(server_info->type, "trojan") == 0) {
    FILE* configFile = fopen(trojanConfigFileName, "w");
    if (configFile == NULL) {
      printf("无法写入配置文件\n");
      return -1;
    } else {
      fprintf(configFile, "{\n");
      fprintf(configFile, "  \"run_type\": \"client\",\n");
      fprintf(configFile, "  \"local_addr\": \"127.0.0.1\",\n");
      fprintf(configFile, "  \"local_port\": %s,\n", listen_port);
      fprintf(configFile, "  \"remote_addr\": \"%s\",\n", server_info->server);
      fprintf(configFile, "  \"remote_port\": %s,\n", server_info->port);
      fprintf(configFile, "  \"password\": [\n");
      fprintf(configFile, "    \"%s\"\n", server_info->password);
      fprintf(configFile, "  ],\n");
      fprintf(configFile, "  \"log_level\": 1,\n");
      fprintf(configFile, "  \"ssl\": {\n");
      fprintf(configFile, "    \"verify\": false,\n");
      fprintf(configFile, "    \"verify_hostname\": false,\n");
      fprintf(configFile, "    \"cert\": \"\",\n");
      fprintf(configFile, "    \"sni\": \"%s\",\n", server_info->sni);
      fprintf(configFile, "    \"alpn\": [\n");
      fprintf(configFile, "      \"h2\",\n");
      fprintf(configFile, "      \"http/1.1\"\n");
      fprintf(configFile, "    ],\n");
      fprintf(configFile, "    \"reuse_session\": true,\n");
      fprintf(configFile, "    \"session_ticket\": false,\n");
      fprintf(configFile, "    \"curves\": \"\"\n");
      fprintf(configFile, "  },\n");
      fprintf(configFile, "  \"tcp\": {\n");
      fprintf(configFile, "    \"no_delay\": true,\n");
      fprintf(configFile, "    \"keep_alive\": true,\n");
      fprintf(configFile, "    \"reuse_port\": false,\n");
      fprintf(configFile, "    \"fast_open\": false,\n");
      fprintf(configFile, "    \"fast_open_qlen\": 20\n");
      fprintf(configFile, "  }\n");
      fprintf(configFile, "}\n");
      fclose(configFile);
      printf("配置文件保存至: %s\n", trojanConfigFileName);

      sprintf(cmd, "%s -c %s",
	      (strlen(CFG_TROJAN_BIN) < 1) ? "trojan" : CFG_TROJAN_BIN,
	      trojanConfigFileName);
      printf("运行命令: %s\n", cmd);
      system(cmd);
      
      return 0;
    }
  }
  
  return 0;
}
