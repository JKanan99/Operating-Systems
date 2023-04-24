/**
 * simple.c
 *
 * A simple kernel module.
 *
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

typedef struct list_head list_head;
typedef struct date date;
struct date {
  int day;
  int month;
  int year;
  const char *name;
};
struct date birthdays[] = {
    // TODO:  fill in these dates and names:
    {25, 8, 1930, "Sean Conery"},
    {27, 1, 1756, "Wolfgnag Amadues Mozat"},
    {23, 4, 1564, "William Shakespeare"},
    {6, 3, 1475, "Michaelangelo"},
    {28, 9, -551, "Confucius"},
};

typedef struct birthday birthday;
struct birthday {
  int day;
  int month;
  int year; // avoid putting name in birthday struct
  struct list_head list;
};
// list_head birthday_list;
static LIST_HEAD(birthday_list);

birthday *birthday_create(date aDay) {
  // TODO: fill in this function
  birthday *person;
  person = kmalloc(sizeof(*person), GFP_KERNEL);
  person->day = aDay.day;
  person->month = aDay.month;
  person->year = aDay.year;
  INIT_LIST_HEAD(&person->list);
  list_add_tail(&person->list, &birthday_list);
  return person;
}
void birthday_print(birthday *p) {
  // TODO: fill in this function
  printk(KERN_INFO "Birthday is: %d/%d/%d\n", p->day, p->month, p->year);
}
void birthday_list_create(void) {
  // TODO: fill in this function
  birthday *person;
  int i = 0;
  int const length = sizeof(birthdays) / sizeof(birthdays[0]);

  for (i = 0; i < length; i++) {
    person = birthday_create(birthdays[i]);
    printk(KERN_INFO "Name is: %s\n", birthdays[i].name);
  }
}
void birthday_list_print(void) {
  // TODO: fill in this function
  birthday *p;
  list_for_each_entry(p, &birthday_list, list) { birthday_print(p); }
}
void birthday_list_delete(void) {
  // TODO: fill in this function
  birthday *p, *next;
  list_for_each_entry_safe(p, next, &birthday_list, list) {
    list_del(&p->list);
    kfree(p);
  }
}

/* This function is called when the module is loaded. */
int simple_init(void) {
  printk(KERN_INFO "Loading Module\n\n");
  printk(KERN_INFO "I can program simple kernel code\n");
  printk(KERN_INFO "Creating birthday list...\n");
  birthday_list_create();
  printk(KERN_INFO "birthday list created!\n\n");
  birthday_list_print();
  printk(KERN_INFO "\n");
  return 0;
}
/* This function is called when the module is removed. */
void simple_exit(void) {
  printk(KERN_INFO "Deleting birthday list...\n");
  birthday_list_delete();
  printk(KERN_INFO "birthday list deleted!\n");
  printk(KERN_INFO "\nRemoving Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

//===================================================================
// OUTPUT
//===================================================================

// osc@ubuntu:~/final-src-osc10e/ch2$ make
// make -C /lib/modules/4.4.0-87-generic/build M=/home/osc/final-src-osc10e/ch2 modules
// make[1]: Entering directory '/usr/src/linux-headers-4.4.0-87-generic'
//   Building modules, stage 2.
//   MODPOST 1 modules
// make[1]: Leaving directory '/usr/src/linux-headers-4.4.0-87-generic'
// osc@ubuntu:~/final-src-osc10e/ch2$ sudo insmod simple.ko
// osc@ubuntu:~/final-src-osc10e/ch2$ dmesg
// [  341.318446] Loading Module
//
// [  341.318449] I can program simple kernel code
// [  341.318449] Creating birthday list...
// [  341.318450] Name is: Sean Conery
// [  341.318450] Name is: Wolfgnag Amadues Mozat
// [  341.318450] Name is: William Shakespeare
// [  341.318451] Name is: Michaelangelo
// [  341.318451] Name is: Confucius
// [  341.318452] birthday list created!
//
// [  341.318452] Birthday is: 25/8/1930
// [  341.318453] Birthday is: 27/1/1756
// [  341.318453] Birthday is: 23/4/1564
// [  341.318454] Birthday is: 6/3/1475
// [  341.318454] Birthday is: 28/9/-551
//===================================================================
