#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define XOR_KEY 26118

static int major_number;
static char encrypted_data[1024] = {0};
static short bytes_encrypted;


static ssize_t device_read(struct file *file, char __user *buffer, size_t len, loff_t *offset){
    int bytes_read = 0;

while (bytes_encrypted > 0 && bytes_read < len) {
    if (put_user(encrypted_data[bytes_read], buffer+ bytes_read)) {
        break;
    }
    bytes_encrypted--;
    bytes_read++;
}
return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset){
    int bytes_written = 0;
    while (bytes_written < len && bytes_written < 1024) {
        char c;
        if (get_user(c, buffer + bytes_written)) {
            break;
        }
        encrypted_data[bytes_written] = c ^ XOR_KEY;
        printk(KERN_INFO "XOR: '%c' -> '%c'\n", c, encrypted_data[bytes_written]);
        bytes_encrypted++;
        bytes_written++;
    }
    return bytes_written;
}


static struct file_operations fops = {
        .open = NULL,
        .release = NULL,
        .read = device_read,
        .write = device_write,
};

static int __init xpo_xor_module_init(void) {
    major_number = register_chrdev(0, "xpo_gonilnik_kodiranje", &fops);
    if (major_number < 0) {
        printk(KERN_ALERT
        "Failed to register a major number for xpo_xor_module\n");
        return major_number;
    }

    printk(KERN_INFO
    "xpo_gonilnik_kodiranje: Registered with major number %d\n", major_number);
    return 0;

}

static void __exit xpo_xor_module_exit(void) {
    unregister_chrdev(major_number, "xpo_gonilnik_kodiranje");
    printk(KERN_INFO
    "Unregistered xpo_xor_module\n");
}

module_init(xpo_xor_module_init);
module_exit(xpo_xor_module_exit);

MODULE_LICENSE("GPL");
