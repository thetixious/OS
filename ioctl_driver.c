#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arkadiy");

#define WR_MODULES_INFO _IOW('a', 5, struct module_struct*)
#define WR_GROUP_TIMER _IOW('a', 6, struct timer_struct*)
#define MYMAJOR 64
#define MAX_COUNT 100

int count_of_modules(void);

static int __init

custom_init(void);

static void __exit

custom_exit(void);

void fill_module_struct(void);

void fill_timer_struct(void);

static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg);

int etx_open(struct inode *inode, struct file *file);

int etx_release(struct inode *inode, struct file *file);

ssize_t etx_read(struct file *filp, char __user

*buf,
size_t len, loff_t
*off);

ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);


struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = etx_read,
        .write = etx_write,
        .open = etx_open,
        .unlocked_ioctl = my_ioctl,
        .release = etx_release,
};

/*int count_of_modules(){
	int count = 0;
	struct module *mod;
	list_for_each_entry_reverse(mod, THIS_MODULE->list.prev,list){
	count++;
	}
	return count;
}*/



struct module_info {
    char module_name[MAX_COUNT][64];
    //enum module_state module_state[MAX_COUNT];
    int module_ref[MAX_COUNT];
    int count_of_modules;
};


struct timer_struct {
    bool exist;
    u64 utime;
    u64 stime;
    u64 sum_exec_runtime;
};
struct module_info_msg1 {
    struct module_info *mf;
    pid_t pid;
};
struct timer_struct_msg2 {
    struct timer_struct *ts;
    pid_t pid;
};
struct module_info_msg1 msg1;
struct timer_struct_msg2 msg2;


static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg) {
    switch (cmd) {
        case WR_MODULES_INFO:
            if (copy_from_user(&msg1, (struct module_info_msg1 *) arg, sizeof(msg1))) {
                pr_err("ERROR: cannot write the data\n");

            }
            pr_info("Data have written succesfully!");

            fill_module_struct();
            break;

        case WR_GROUP_TIMER:
            if (copy_from_user(&msg2, (struct timer_struct_msg2 *) arg, sizeof(msg2))) {
                pr_err("ERROR: cannot write the data\n");

            }
            pr_info("Data for timer have written succesfully!");
            fill_timer_struct();
            break;
        default:
            pr_info("nothing happend");
            break;

    }
    return 1;

}

static int __init

custom_init(void) {
    printk(KERN_INFO
    "Hello, driver loaded.\n");
    int shot;
    shot = register_chrdev(MYMAJOR, "my_ioctl_driver", &fops);
    if (shot == 0) {
        pr_info("ioctl_driver - registered. Major: %d; Minor: %d \n", MYMAJOR, 0);

    } else if (shot > 0) {

        pr_info("ioctl_driver - registered. Major: %d; Minor: %d \n", shot >> 20, shot & 0xfffff);
    } else {
        pr_info("Cannot register device\n");
        return -1;
    }

    return 0;
}

void fill_module_struct(void) {
    struct module *mod;
    int index = 0;

    list_for_each_entry(mod, THIS_MODULE->list.next, list)
    {
        if (index >= MAX_COUNT) {

            break;
        }

        strncpy(msg1.mf->module_name[index], mod->name, MODULE_NAME_LEN - 1);
        msg1.mf->module_name[index][MODULE_NAME_LEN -
                                    1] = '\0';  // \u0417\u0430\u0432\u0435\u0440\u0448\u0435\u043d\u0438\u0435 \u0441\u0442\u0440\u043e\u043a\u0438 \u043d\u0443\u043b\u0435\u0432\u044b\u043c \u0441\u0438\u043c\u0432\u043e\u043b\u043e\u043c

        //  msg1.mf->module_state[index] = mod->state;
        msg1.mf->module_ref[index] = module_refcount(mod);
        index++;
    }
    msg1.mf->count_of_modules = index;
    pr_info("count of modules: %d", index);
}

void fill_timer_struct(void) {
    pr_info("Your PID: %d", msg2.pid);
    struct task_struct *task = get_pid_task(find_get_pid(msg2.pid), PIDTYPE_PID);
    if (task == NULL) {
        msg2.ts->exist = false;
    } else {
        msg2.ts->exist = true;
        struct thread_group_cputimer *cputimer = &current->signal->cputimer;
        msg2.ts->utime = atomic64_read(&cputimer->cputime_atomic.utime);
        msg2.ts->stime = atomic64_read(&cputimer->cputime_atomic.stime);
        msg2.ts->sum_exec_runtime = atomic64_read(&cputimer->cputime_atomic.sum_exec_runtime);
    }
}

void __exit

custom_exit(void) {
    unregister_chrdev(MYMAJOR, "my_ioctl_driver");
    pr_info("By, Kernel!\n");


}

int etx_open(struct inode *inode, struct file *file) {
    pr_info("Device File Opened...\n");
    return 0;
}


int etx_release(struct inode *inode, struct file *file) {
    pr_info("Device File Closed...\n");
    return 0;
}

ssize_t etx_read(struct file *filp, char __user

* buf,
size_t len, loff_t
* off) {
pr_info("Read Function\n");
return 0;
}


ssize_t etx_write(struct file *filp, const char __user

* buf,
size_t len, loff_t
* off) {
pr_info("Write function\n");
return
len;
}
module_init(custom_init);
module_exit(custom_exit);