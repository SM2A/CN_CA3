#ifndef CONFIG

#define CONFIG

#define SERVER_PORT 8888
#define ROUTER_PORT 8889
#define MSG_SIZE 10
#define PACKET_SIZE (MSG_SIZE + 11 + 4 + 2 + 1 + 1)
#define TIMEOUT 100000

#endif