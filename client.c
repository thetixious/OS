
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include <linux/ptrace.h>
#include <inttypes.h>
#include <linux/types.h>

#define WR_MODULES_INFO _IOW('a', 5, struct module_struct*)
#define WR_GROUP_TIMER _IOW('a', 6, struct timer_struct*)
#define MODULE_NAME_LEN 64
#define MAX_COUNT 100


struct module_info {
    char module_name[MAX_COUNT][MODULE_NAME_LEN];
    //enum module_state module_state[MAX_COUNT];
    int module_ref[MAX_COUNT];
    uint64_t count_of_modules;
};


struct timer_struct {
    bool exist;
    uint64_t utime;
    uint64_t stime;
    uint64_t sum_exec_runtime;
};
struct module_info_msg1 {
    struct module_info *mf;
    pid_t pid;
};
struct timer_struct_msg2 {
    struct timer_struct *ts;
    pid_t pid;
};

int main(int argc, char *argv[]) {
    int fd;
    printf("\n----------------------------\n");
    printf("\nStart working....\n");
    fd = open("/dev/ioctl_driver", O_RDWR);
    if (fd < 0) {
        printf("cannot work with this driver\n");
        return 0;
    }
    printf("\n processing your data...\n");
    if (argc < 2) {
        printf("enter pid, lol\n");
        return 0;
    }
    printf("\n parody on lsmod:\n");
    struct module_info *mf = malloc(sizeof(struct module_info));
    if (mf == NULL) {
        printf("Error allocating memory\n");
        close(fd);
        return 0;
    }
    struct module_info_msg1 msg;

    msg.pid = atoi(argv[1]);
    msg.mf = mf;
    if (ioctl(fd, WR_MODULES_INFO,(struct module_info_msg1 *) &msg)<0){
        printf("Error in ioctl");
        close(fd);
        return 0;
    }
    //printf("check\n");
    //printf("int : %ld\n",msg.mf->count_of_modules);
    //printf("name: %s\n",msg.mf->module_name[3]);


    for (int i = 0; i < msg.mf->count_of_modules; i++) {
        printf("Module: %s,  Refcount: %d\n",
               msg.mf->module_name[i], msg.mf->module_ref[i]);
    }

    printf("\nsome timer's stuff\n");

    struct timer_struct *ts = malloc(sizeof(struct timer_struct));
    if (mf == NULL) {
        printf("Error allocating memory\n");
        close(fd);
        return 0;
    }
    struct timer_struct_msg2 msg1;
    msg1.pid = atoi(argv[1]);
    msg1.ts = ts;

    if (ioctl(fd, WR_GROUP_TIMER,(struct timer_struct_msg2 *) &msg1)<0){
        printf("Error in ioctl");
        close(fd);
        return 0;
    }
    if (msg1.ts->exist == false) {
        printf("\nYour PID doesnt exist\n");

    } else {
        printf("Spended time in user mode: %ld\n", msg1.ts->utime);
        printf("Spended time in system mode: %ld\n", msg1.ts->stime);
        printf("Spended time in summary; %ld\n", msg1.ts->sum_exec_runtime);

    }
    free(ts);
    free(mf);
    close(fd);
}




























