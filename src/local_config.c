
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <yaml.h>

#include "shadow_cli.h"

char config_values[6][200];

int loadConfig(char* filename) {
  FILE *fp = fopen(filename, "r");
  int serverCnt = 0;
  
  if (fp == NULL) {
    fprintf(stderr, "无法打开配置文件\n");
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
  int scalar_cnt = 0;
  char last_prop[100];
  char *current_val;
  
  // 进入事件循环
  while (yaml_parser_parse(&parser, &event) && in_parse) {
    switch (event.type) {
    case YAML_MAPPING_START_EVENT:
      depth++;
      scalar_cnt = 0;
      break;
    case YAML_MAPPING_END_EVENT:
      depth--;
      scalar_cnt = 0;
      break;
    case YAML_SEQUENCE_START_EVENT:
      break;
    case YAML_SEQUENCE_END_EVENT:
      break;
    case YAML_SCALAR_EVENT:
      current_val = event.data.scalar.value;
      if (depth == 1) {
	scalar_cnt++;
	if ((scalar_cnt % 2) == 1) {
	  memset(last_prop, 0, sizeof(last_prop));
	  strcpy(last_prop, current_val);
	} else {
	  if (strcmp(last_prop, "url") == 0) {
	    strcpy(config_values[CFG_URL], current_val);
	  } else if (strcmp(last_prop, "update") == 0) {
	    strcpy(config_values[CFG_UPDATE], current_val);
	  } else if (strcmp(last_prop, "port") == 0) {
	    strcpy(config_values[CFG_PORT], current_val);
	  } else if (strcmp(last_prop, "trojan-bin") == 0) {
	    strcpy(config_values[CFG_TROJAN_BIN], current_val);
	  } else if (strcmp(last_prop, "ss-bin") == 0) {
	    strcpy(config_values[CFG_SS_BIN], current_val);
	  }
	}
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
  }
  
  // done:
  yaml_parser_delete(&parser);
  fclose(fp);
  return EXIT_SUCCESS;
}

char* get_config(int idx) {
  return config_values[idx];
}
