
#ifndef _SHADOW_CLI_H
#define _SHADOW_CLI_H 1

struct server_info {
  int    num;
  char   type[10];
  char   name[100];
  char   server[200];
  char   port[10];
  char   cipher[100];
  char   password[100];
  char   udp[5];
  char   sni[100];
  char   skip_cert_verify[10];
};

typedef struct server_info server_info_t;

char CFG_URL[200];
char CFG_UPDATE[10];
char CFG_PORT[10];
char CFG_TROJAN_BIN[200];
char CFG_SS_BIN[200];

/**
 */
int loadConfig(char* filename);
/**
 *
 */
int extractServerList(char* filename, server_info_t* serverList);
/**
 */
server_info_t* selectServerFromList(server_info_t* serverList, int serverCnt);
/**
 */
int fetchFromUrl(char* serverListFileName);

/**
 */
int makeConnectCmd(server_info_t* server_info, char* to_str);

#endif
