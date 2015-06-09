#include <signal.h>

#include "test.h"
#include "test_rmi.h"
#include "find_device.h"
#include "find_rmi.h"
#include "socket.h"

#define MAX_NUM 2
#define CONNECT_NUM	10

#define WAIT_TIME	(10*1000)

/*struct aaa gs_para1[MAX_NUM];*/
/*struct bbb gs_para2[MAX_NUM];*/

struct rmi gs_rmi[CONNECT_NUM];

char * server_ip;
unsigned short server_port;

int test_cnt = 0;
int connect_cnt = 0;

int set_dev_info(struct rmi * rmi, DEV_INFO_S * pdev) {
	printf("dev_ip: %s\n", net_to_ip(pdev->dev_ip));

	return 0;
}

void * test_proc() {
	struct rmi client_rmi, * rmi;
	int i;
	int test_times = 1;
	int n;
	
	rmi = &client_rmi;
	RMI_INIT_CLIENT(rmi, test);	
	rmi_set_socket_type(rmi, RMI_SOCKET_UDP);
	rmi_set_broadcast(rmi);
	rmi_set_timeout(rmi, 500);
	if (0 != rmi_client_start(rmi, server_ip, server_port)) {
		trace("rmi_client_start failed\n");
		return NULL;
	}
/*	printf("connect is done\n");*/
	//connect_cnt++;
/*	return NULL;*/
	for (n = 0; n < test_times; n++) {
#if 0
		gs_para1[0].a = -1;
		gs_para1[0].b = -2;
		gs_para1[0].c = 1.2;
		gs_para1[0].d = 98.12345;
		gs_para1[0].e = 1000.3456778;
		strcpy(gs_para1[0].f, "1234567890");
		gs_para2[0].stAaa = gs_para1[0];
		for (i = 0; i < 10; i++) {
			gs_para2[0].a[i] = i;
		}
		for (i = 0; i < 20; i++) {
			gs_para2[0].a_array[i] = gs_para2[0].stAaa;
		}
		
		gs_para1[1].a = -2;
		gs_para1[1].b = 3;
		gs_para2[1].stAaa = gs_para1[1];
		for (i = 0; i < 10; i++) {
			gs_para2[1].a[i] = 0 - i*2;
		}
		for (i = 0; i < 20; i++) {
			gs_para2[1].a_array[i] = gs_para2[1].stAaa;
		}

		for (i = 0; i < MAX_NUM; i++) {
			if (0 != set_para(rmi, i, &gs_para1[i])) {
				trace("set_para failed\n");
				return NULL;
			}
			usleep(WAIT_TIME);
		}

		for (i = 0; i < MAX_NUM; i++) {
			if (0 != set_para2(rmi, i, &gs_para1[i], &gs_para2[i])) {
				trace("set_para failed\n");
				return NULL;
			}
			usleep(WAIT_TIME);
		}
		for (i = 0; i < MAX_NUM; i++) {
			struct aaa aaa_buf;
			memset(&aaa_buf, 0, sizeof(aaa_buf));
			if (0 != get_para(rmi, i, &aaa_buf)) {
				trace("get_para failed\n");
				return NULL;
			}
			if (0 != memcmp(&aaa_buf, &gs_para1[i], sizeof(aaa_buf))) {
				trace("para1 read error\n");
				return NULL;
			}
			usleep(WAIT_TIME);
		}
		for (i = 0; i < MAX_NUM; i++) {
			struct aaa aaa_buf;
			struct bbb bbb_buf;
			memset(&aaa_buf, 0, sizeof(aaa_buf));
			memset(&bbb_buf, 0, sizeof(bbb_buf));
			if (0 != get_para2(rmi, &aaa_buf, &bbb_buf, i)) {
				trace("get_para failed\n");
				return NULL;
			}
			if (0 != memcmp(&aaa_buf, &gs_para1[i], sizeof(aaa_buf))) {
				trace("para1 read error\n");
				return NULL;
			}
			if (0 != memcmp(&bbb_buf, &gs_para2[i], sizeof(bbb_buf))) {
				trace("para2 read error\n");
				return NULL;
			}
			usleep(WAIT_TIME);
		}
	#endif
/*		SWITCH_PORT_INFO_S stPortInfo;*/
/*		switch_get_port_status(rmi,0,&stPortInfo);*/
/*		printf("enable: %d\n", stPortInfo.enable);*/
/*		printf("link: %08x\n", stPortInfo.link);*/
/*		printf("speed: %08x\n", stPortInfo.speed);*/
/*		printf("duplex: %08x\n", stPortInfo.duplex);*/
/*		printf("fc: %08x\n", stPortInfo.fc);*/
/*		TEST3_S stTest3;*/
/*		switch_test_get2(rmi, &stTest3);*/
		find_device(rmi);
	}

	if (0 != rmi_client_close(rmi)) {
		trace("rmi_client_close failed\n");
		return NULL;
	}
	printf("test pass!!!\n");
	test_cnt++;

	return NULL;
}

int main(int argc, char * argv[]) {
	unsigned short port;
	char * host;
	struct rmi * rmi;
	int i;
	int cnt;
	pthread_t pid;
	struct sigaction act;

	struct rmi find_rmi;
	
	if (argc != 3) {
		printf("usage: %s host port\n", argv[0]);
		return -1;
	}

	server_ip = argv[1];
	server_port = atoi(argv[2]);
	
	act.sa_sigaction = SIG_IGN;
    act.sa_flags = SA_NOMASK;
    sigaction(SIGPIPE, &act, NULL);

	RMI_INIT_SERVER(&find_rmi, find);
	rmi_set_socket_type(&find_rmi, RMI_SOCKET_UDP);
	rmi_set_broadcast(&find_rmi);
	rmi_set_keepalive_time(&find_rmi, 0xffffff);
	rmi_server_start(&find_rmi, 8888);

	for(i = 0; i < CONNECT_NUM; i++) {
		int ret = pthread_create(&pid, NULL, test_proc, NULL);
/*		printf("pthread_create return : %d\n", ret);*/
	}

	getchar();
	printf("successs cnt: %d\n", test_cnt);
	
	rmi_server_close(&find_rmi);

	return 0;
}

