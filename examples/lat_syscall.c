/*
 * hello-5.c - Demonstrates command line argument passing to a module.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/ktime.h>

MODULE_LICENSE("GPL");

static short int myshort = 1;
static int myint = 420;
static long int mylong = 9999;
static char *mystring = "blah";
static int myintarray[2] = { 420, 420 };
static int arr_argc = 0;

/* module_param(foo, int, 0000)
 * The first param is the parameters name.
 * The second param is its data type.
 * The final argument is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */
module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

/* module_param_array(name, type, num, perm);
 * The first param is the parameter's (in this case the array's) name.
 * The second param is the data type of the elements of the array.
 * The third argument is a pointer to the variable that will store the number
 * of elements of the array initialized by the user at module loading time.
 * The fourth argument is the permission bits.
 */
module_param_array(myintarray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintarray, "An array of integers");

static ktime_t k_now(void)
{
    return ktime_get();
}
static int k_start(ktime_t * const starttime) {*starttime = k_now(); return 0;}
static int k_end(ktime_t * const rettime, ktime_t const starttime) {
    ktime_t ret = k_now();
    *rettime = ktime_sub(ret, starttime);
    return 0;
}

static int __init lat_syscall_init(void)
{
    int i;
    ktime_t start = 0, during = 0;

    k_start(&start);
    pr_info("start in ktime: %lld\n", start);
    pr_info("during in ms: %lld ms", ktime_to_ms(start));
    pr_info("during in ns: %lld ns", ktime_to_ns(start));

    pr_info("Hello, world 5\n=============\n");
    pr_info("myshort is a short integer: %hd\n", myshort);
    pr_info("myint is an integer: %d\n", myint);
    pr_info("mylong is a long integer: %ld\n", mylong);
    pr_info("mystring is a string: %s\n", mystring);

    for (i = 0; i < ARRAY_SIZE(myintarray); i++)
        pr_info("myintarray[%d] = %d\n", i, myintarray[i]);

    pr_info("got %d arguments for myintarray.\n", arr_argc);

    k_end(&during, start);
    pr_info("during in ktime: %lld\n", during);
    pr_info("during in ms: %lld ms", ktime_to_ms(during));
    pr_info("during in ns: %lld ns", ktime_to_ns(during));

    return 0;
}

static void __exit lat_syscall_exit(void)
{
    pr_info("Goodbye, world 5\n");
}

module_init(lat_syscall_init);
module_exit(lat_syscall_exit);
