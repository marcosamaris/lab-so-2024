#include<linux/module.h>     // included for all kernel modules
#include<linux/init.h>       // included for __init and __exit macros
#include<linux/kthread.h>    // included for threading related functions
#include<linux/sched.h>      // included to create task_struct
#include<linux/delay.h>      // included for the sleep/delay function in the thread

// array for task_struct to hold task info
static struct task_struct kth_arr[4];

// long running function to be executed inside a thread, this will run for 30 secs. 
int thread_function(void * idx) {
  unsigned int i = 0;
  int t_id = * (int * ) idx;

  // kthread_should_stop call is important.
  while (!kthread_should_stop()) {
    printk(KERN_INFO "Thread %d Still running...! %d secs\n", t_id, i);
    i++;
    if (i == 30)
      break;
    msleep(1000);
  }
  printk(KERN_INFO "thread %d stopped\n", t_id);
  return 0;
}

// initialize one thread at a time.
int initialize_thread(struct task_struct * kth, int idx) {
  char th_name[20];
  sprintf(th_name, "kthread_%d", idx);
  kth = kthread_create(thread_function, & idx, (const char * ) th_name);
  if (kth != NULL) {
    wake_up_process(kth);
    printk(KERN_INFO "%s is running\n", th_name);
  } else {
    printk(KERN_INFO "kthread %s could not be created\n", th_name);
    return -1;
  }
  return 0;
}

// module init function
static int __init mod_init(void) {
  int i = 0;
  printk(KERN_INFO "Initializing thread module\n");
  for (i = 0; i < 4; i++) {
	// initialize one thread at a time.
    if (initialize_thread( & kth_arr[i], i) == -1) {
      return -1;
    }
  }
  printk(KERN_INFO "all of the threads are running\n");
  return 0;
}

// module exit function.
static void __exit mod_exit(void) {
  int i = 0;
  int ret = 0;
  printk(KERN_INFO "exiting thread module\n");
  for (i = 0; i < 4; i++) {
	// stop all of the threads before removing the module.
    ret = kthread_stop( & kth_arr[i]);
    if (!ret) {
      printk("can't stop thread %d", i);
    }
  }
  printk(KERN_INFO "stopped all of the threads\n");
}

MODULE_LICENSE("GPL");


module_init(mod_init);
module_exit(mod_exit);
