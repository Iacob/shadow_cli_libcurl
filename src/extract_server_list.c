
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <yaml.h>

#include "shadow_cli.h"

int extractServerList(char* filename, server_info_t* serverList) {
  FILE *fp = fopen(filename, "r");
  int serverCnt = 0;
  
  if (fp == NULL) {
    fprintf(stderr, "无法打开服务器列表文件\n");
    return EXIT_FAILURE;
  }
  
  // 初始化YAML解析器
  yaml_parser_t parser;
  if (!yaml_parser_initialize(&parser)) {
    fprintf(stderr, "无法初始化YAML解析器\n");
    fclose(fp);
    return EXIT_FAILURE;
  }

  yaml_event_t event;
  
  yaml_parser_set_input_file(&parser, fp);

  int in_parse = 1;
  int depth = 0;
  int in_proxies_list = 0;
  int in_mapping_scalar_cnt = 0;
  int proxy_idx = 0;
  char last_prop[100];
  char *current_val;
  int server_num = 1;
  
  // 进入事件循环
  while (yaml_parser_parse(&parser, &event) && in_parse) {
    // yaml_event_t event = yaml_parser_pop_event(&parser);
    switch (event.type) {
    case YAML_MAPPING_START_EVENT:
      depth++;
      in_mapping_scalar_cnt = 0;
      break;
    case YAML_MAPPING_END_EVENT:
      depth--;
      in_mapping_scalar_cnt = 0;
      if (in_proxies_list) {
	serverList[proxy_idx].num = server_num;
	server_num++;
	proxy_idx++;
	// printf("-------------\n");
      }
      break;
    case YAML_SEQUENCE_START_EVENT:
      break;
    case YAML_SEQUENCE_END_EVENT:
      in_proxies_list = 0;
      break;
    case YAML_SCALAR_EVENT:
      //printf("--%s-- depth: %d in_mapping: %d\n", event.data.scalar.value, depth, in_mapping);
      current_val = event.data.scalar.value;
      if ((depth == 1) && (strcmp(current_val, "proxies") == 0)) {
	in_proxies_list = 1;
      } else if (in_proxies_list) {
	// printf(" %s ", event.data.scalar.value);
	if ((in_mapping_scalar_cnt % 2) == 0) {
	  memset(last_prop, 0, sizeof(last_prop));
	  strcpy(last_prop, current_val);
	  // printf(" : ");
	} else {
	  if (strcmp(last_prop, "name") == 0) {
	    strcpy(serverList[proxy_idx].name, current_val);
	  } else if (strcmp(last_prop, "type") == 0) {
	    strcpy(serverList[proxy_idx].type, current_val);
	  } else if (strcmp(last_prop, "server") == 0) {
	    strcpy(serverList[proxy_idx].server, current_val);
	  } else if (strcmp(last_prop, "port") == 0) {
	    strcpy(serverList[proxy_idx].port, current_val);
	  } else if (strcmp(last_prop, "cipher") == 0) {
	    strcpy(serverList[proxy_idx].cipher, current_val);
	  } else if (strcmp(last_prop, "password") == 0) {
	    strcpy(serverList[proxy_idx].password, current_val);
	  } else if (strcmp(last_prop, "udp") == 0) {
	    strcpy(serverList[proxy_idx].udp, current_val);
	  } else if (strcmp(last_prop, "sni") == 0) {
	    strcpy(serverList[proxy_idx].sni, current_val);
	  } else if (strcmp(last_prop, "skip-cert-verify") == 0) {
	    strcpy(serverList[proxy_idx].skip_cert_verify, current_val);
	  }
	  
	  // printf("\n");
	}
	in_mapping_scalar_cnt++;
      }
      break;
    case YAML_DOCUMENT_END_EVENT:
      // 文档结束，退出循环
      // goto done;
      in_parse = 0;
      break;
    default:
      break;
    }
    /* // 如果事件有数据，释放内存 */
    /* if (event.type == YAML_SCALAR_EVENT) { */
    /*   // xmlFree(event.data.scalar.value); */
    /* } */
  }
  
  // done:
  yaml_parser_delete(&parser);
  fclose(fp);
  return EXIT_SUCCESS;
}

server_info_t* selectServerFromList(server_info_t* serverList, int serverCnt) {
  char input_line[100];
  int serverNum = 0;
  server_info_t* selectedServer;
  server_info_t* serverPtr = serverList;
  for (int i=0; i<serverCnt; i++) {
    if (serverList[i].num > 0) {
      printf("%d: [%s]%s\n", serverList[i].num, serverList[i].type, serverList[i].name);
    }
  }
  printf("请输入服务器序号: ");
  fgets(input_line, 6, stdin);
  serverNum = atoi(input_line);
  printf("用户选择服务器: %d\n", atoi(input_line));

  for (int i=0; i<serverCnt; i++) {
    if ((serverPtr->num > 0) && (serverNum == serverPtr->num)) {
      selectedServer = serverPtr;
    }
    serverPtr++;
  }
  printf("%d: [%s]%s %s:%s\n", selectedServer->num, selectedServer->type, selectedServer->name, selectedServer->server, selectedServer->port);
  
  return selectedServer;
}
