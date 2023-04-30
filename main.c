#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

// Unique number for our driver.
int driver_num;

// Buffer for storing encrypted data.
char encrypted_data[1024] = {0};

// Number of bytes in the buffer.
int bytes_encrypted;

// Read from the device.

ssize_t read(struct file *file, char __user *buffer, size_t len, loff_t *offset)
{
    // Keep track of already read bytes.
    int bytes_read = 0;

    while (bytes_read < len)
    {

        // Copy from kernel space (encrypted_data) to user space.(buffer -> starting address of user space)
        if (put_user(encrypted_data[bytes_read], buffer + bytes_read) != 0)
            break;

        // Stores amount of bytes ready to be read and encrypted, if 0, we are done.
        if (bytes_encrypted == 0)
            break;

        // Increment to move to next byte.
        bytes_read++;

        // Decrement amount of bytes we need to read.
        bytes_encrypted--;
    }
    return bytes_read;
}

ssize_t write(struct file *file, const char __user *buffer, size_t len, loff_t *offset)
{

    // Key for XOR encryption.
    const int KEY = 26118;

    int bytes_written = 0;

    // Loop until we have read all bytes.
    while (bytes_written < len)
    {
        // Check if we have enough space in the buffer.
        if (bytes_written < 1024)
        {
            char c;
            // Copy from user space to kernel space.
            if (get_user(c, buffer + bytes_written))
            {
                break;
            }
            // XOR encryption.
            encrypted_data[bytes_written] = c ^ KEY;
            printk(KERN_INFO "DEBUG XOR: '%c' CONVERT TO: '%c'\n", c, encrypted_data[bytes_written]);

            // To keep track of how many bytes we have encrypted.
            bytes_encrypted++;
            // Move to next byte.
            bytes_written++;
        }
    }
    return bytes_written;
}

// Unused methods have to be set to NULL.
struct file_operations operations = {
    .read = read,
    .write = write,
    .open = NULL,
    .release = NULL};

// Register the driver.
int __init xpo_xor_module_init(void)
{
    driver_num = register_chrdev(0, "xpo_gonilnik_kodiranje", &operations);
    printk(KERN_INFO "driver num: %d\n", driver_num); // sudo dmesg | tail
    if (driver_num < 0)
    {
        printk(KERN_ALERT "Failed to register.\n");
        return driver_num;
    }
    return 0;
}

// Unregister the driver.
void __exit xpo_xor_module_exit(void)
{
    unregister_chrdev(driver_num, "xpo_gonilnik_kodiranje");
    printk(KERN_INFO
           "Unregistered xpo_xor_module\n");
}

module_init(xpo_xor_module_init);
module_exit(xpo_xor_module_exit);

MODULE_LICENSE("GPL");
