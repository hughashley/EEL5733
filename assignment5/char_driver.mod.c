#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x6bd0e573, "down_interruptible" },
	{ 0xcf2a6966, "up" },
	{ 0x92997ed8, "_printk" },
	{ 0xc6cbbc89, "capable" },
	{ 0x37a0cba, "kfree" },
	{ 0xb8b9f817, "kmalloc_order_trace" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0x75ac04da, "class_destroy" },
	{ 0xb74f8447, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x4e6125e3, "device_destroy" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xdcb764ad, "memset" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0xcf0b1a1, "cpu_hwcap_keys" },
	{ 0xb788fb30, "gic_pmr_sync" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xe2172e02, "cpu_hwcaps" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x8cd2b679, "__class_create" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xb9557d59, "cdev_init" },
	{ 0x2c2ae348, "cdev_add" },
	{ 0xfbca8ea1, "device_create" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x9a7e8f56, "param_ops_int" },
	{ 0x1b435db3, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F6B915CCFCE46DCD90E25BA");
