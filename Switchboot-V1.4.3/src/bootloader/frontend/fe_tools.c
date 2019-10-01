/*
 * Copyright (c) 2018 naehrwert
 * Copyright (c) 2018-2019 CTCaer
 * Copyright (c) 2018 Reisyukaku
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>

#include "fe_tools.h"
#include "../config/config.h"
#include "../gfx/gfx.h"
#include "../gfx/tui.h"
#include "../hos/hos.h"
#include "../hos/pkg1.h"
#include "../hos/pkg2.h"
#include "../hos/sept.h"
#include "../libs/fatfs/ff.h"
#include "../mem/heap.h"
#include "../power/max7762x.h"
#include "../power/bq24193.h"
#include "../power/max17050.h"
#include "../sec/se.h"
#include "../storage/nx_emmc.h"
#include "../storage/sdmmc.h"
#include "../soc/fuse.h"
#include "../utils/btn.h"
#include "../utils/util.h"
#include "../utils/browser.h"

extern boot_cfg_t b_cfg;
extern hekate_config h_cfg;

extern bool sd_mount();
extern void sd_unmount();
extern int  sd_save_to_file(void *buf, u32 size, const char *filename);
extern void emmcsn_path_impl(char *path, char *sub_dir, char *filename, sdmmc_storage_t *storage);
u32 screenshot_count;

#pragma GCC push_options
#pragma GCC optimize ("Os")

void dump_packages12()
{
	if (!sd_mount())
		return;

	char path[64];

	u8 *pkg1 = (u8 *)calloc(1, 0x40000);
	u8 *warmboot = (u8 *)calloc(1, 0x40000);
	u8 *secmon = (u8 *)calloc(1, 0x40000);
	u8 *loader = (u8 *)calloc(1, 0x40000);
	u8 *pkg2 = NULL;
	u8 kb = 0;

	tsec_ctxt_t tsec_ctxt;

	gfx_clear_partial_grey(BG_COL, 0, 1256);
	gfx_con_setpos(0, 0);

	sdmmc_storage_t storage;
	sdmmc_t sdmmc;
	if (!sdmmc_storage_init_mmc(&storage, &sdmmc, SDMMC_4, SDMMC_BUS_WIDTH_8, 4))
	{
		EPRINTF("Failed to init eMMC.");
		goto out_free;
	}
	sdmmc_storage_set_mmc_partition(&storage, 1);

	// Read package1.
	sdmmc_storage_read(&storage, 0x100000 / NX_EMMC_BLOCKSIZE, 0x40000 / NX_EMMC_BLOCKSIZE, pkg1);
	const pkg1_id_t *pkg1_id = pkg1_identify(pkg1);
	if (!pkg1_id)
	{
		EPRINTF("Unknown pkg1 version for reading\nTSEC firmware.");
		// Dump package1.
		emmcsn_path_impl(path, "/pkg1", "pkg1_enc.bin", &storage);
		if (sd_save_to_file(pkg1, 0x40000, path))
			goto out_free;
		gfx_puts("\nEnc pkg1 dumped to pkg1_enc.bin\n");

		goto out_free;
	}
	const pk11_hdr_t *hdr = (pk11_hdr_t *)(pkg1 + pkg1_id->pkg11_off + 0x20);

	kb = pkg1_id->kb;

	if (!h_cfg.se_keygen_done)
	{
		tsec_ctxt.fw = (void *)pkg1 + pkg1_id->tsec_off;
		tsec_ctxt.pkg1 = (void *)pkg1;
		tsec_ctxt.pkg11_off = pkg1_id->pkg11_off;
		tsec_ctxt.secmon_base = pkg1_id->secmon_base;

		if (kb >= KB_FIRMWARE_VERSION_700 && !h_cfg.sept_run)
		{
			b_cfg.autoboot = 0;
			b_cfg.autoboot_list = 0;

			gfx_printf("sept will run to get the keys.\nThen rerun this option.");
			btn_wait();

			if (!reboot_to_sept((u8 *)tsec_ctxt.fw, kb))
			{
				gfx_printf("Failed to run sept\n");
				goto out_free;
			}
		}

		// Read keyblob.
		u8 *keyblob = (u8 *)calloc(NX_EMMC_BLOCKSIZE, 1);
		sdmmc_storage_read(&storage, 0x180000 / NX_EMMC_BLOCKSIZE + kb, 1, keyblob);

		// Decrypt.
		keygen(keyblob, kb, &tsec_ctxt, NULL);
		if (kb <= KB_FIRMWARE_VERSION_600)
			h_cfg.se_keygen_done = 1;
		free(keyblob);
	}

	if (kb <= KB_FIRMWARE_VERSION_600)
		pkg1_decrypt(pkg1_id, pkg1);

	if (kb <= KB_FIRMWARE_VERSION_620)
	{
		pkg1_unpack(warmboot, secmon, loader, pkg1_id, pkg1);
	
		// Display info.
		gfx_printf("%kNX Bootloader size:  %k0x%05X\n\n", WARN_TEXT_COL, MAIN_TEXT_COL, hdr->ldr_size);
	
		gfx_printf("%kSecure monitor addr: %k0x%05X\n", WARN_TEXT_COL, MAIN_TEXT_COL, pkg1_id->secmon_base);
		gfx_printf("%kSecure monitor size: %k0x%05X\n\n", WARN_TEXT_COL, MAIN_TEXT_COL, hdr->sm_size);
	
		gfx_printf("%kWarmboot addr:       %k0x%05X\n", WARN_TEXT_COL, MAIN_TEXT_COL, pkg1_id->warmboot_base);
		gfx_printf("%kWarmboot size:       %k0x%05X\n\n", WARN_TEXT_COL, MAIN_TEXT_COL, hdr->wb_size);

		// Dump package1.1.
		emmcsn_path_impl(path, "/pkg1", "pkg1_decr.bin", &storage);
		if (sd_save_to_file(pkg1, 0x40000, path))
			goto out_free;
		gfx_puts("\npkg1 dumped to pkg1_decr.bin\n");
	
		// Dump nxbootloader.
		emmcsn_path_impl(path, "/pkg1", "nxloader.bin", &storage);
		if (sd_save_to_file(loader, hdr->ldr_size, path))
			goto out_free;
		gfx_puts("NX Bootloader dumped to nxloader.bin\n");
	
		// Dump secmon.
		emmcsn_path_impl(path, "/pkg1", "secmon.bin", &storage);
		if (sd_save_to_file(secmon, hdr->sm_size, path))
			goto out_free;
		gfx_puts("Secure Monitor dumped to secmon.bin\n");
	
		// Dump warmboot.
		emmcsn_path_impl(path, "/pkg1", "warmboot.bin", &storage);
		if (sd_save_to_file(warmboot, hdr->wb_size, path))
			goto out_free;
		gfx_puts("Warmboot dumped to warmboot.bin\n\n\n");
	}

	// Dump package2.1.
	sdmmc_storage_set_mmc_partition(&storage, 0);
	// Parse eMMC GPT.
	LIST_INIT(gpt);
	nx_emmc_gpt_parse(&gpt, &storage);
	// Find package2 partition.
	emmc_part_t *pkg2_part = nx_emmc_part_find(&gpt, "BCPKG2-1-Normal-Main");
	if (!pkg2_part)
		goto out;

	// Read in package2 header and get package2 real size.
	u8 *tmp = (u8 *)malloc(NX_EMMC_BLOCKSIZE);
	nx_emmc_part_read(&storage, pkg2_part, 0x4000 / NX_EMMC_BLOCKSIZE, 1, tmp);
	u32 *hdr_pkg2_raw = (u32 *)(tmp + 0x100);
	u32 pkg2_size = hdr_pkg2_raw[0] ^ hdr_pkg2_raw[2] ^ hdr_pkg2_raw[3];
	free(tmp);
	// Read in package2.
	u32 pkg2_size_aligned = ALIGN(pkg2_size, NX_EMMC_BLOCKSIZE);
	pkg2 = malloc(pkg2_size_aligned);
	nx_emmc_part_read(&storage, pkg2_part, 0x4000 / NX_EMMC_BLOCKSIZE, 
		pkg2_size_aligned / NX_EMMC_BLOCKSIZE, pkg2);
	// Decrypt package2 and parse KIP1 blobs in INI1 section.
	pkg2_hdr_t *pkg2_hdr = pkg2_decrypt(pkg2);
	if (!pkg2_hdr)
	{
		gfx_printf("Pkg2 decryption failed!\n");
		goto out;
	}

	// Display info.
	gfx_printf("%kKernel size:   %k0x%05X\n\n", WARN_TEXT_COL, MAIN_TEXT_COL, pkg2_hdr->sec_size[PKG2_SEC_KERNEL]);
	gfx_printf("%kINI1 size:     %k0x%05X\n\n", WARN_TEXT_COL, MAIN_TEXT_COL, pkg2_hdr->sec_size[PKG2_SEC_INI1]);

	// Dump pkg2.1.
	emmcsn_path_impl(path, "/pkg2", "pkg2_decr.bin", &storage);
	if (sd_save_to_file(pkg2, pkg2_hdr->sec_size[PKG2_SEC_KERNEL] + pkg2_hdr->sec_size[PKG2_SEC_INI1], path))
		goto out;
	gfx_puts("\npkg2 dumped to pkg2_decr.bin\n");

	// Dump kernel.
	emmcsn_path_impl(path, "/pkg2", "kernel.bin", &storage);
	if (sd_save_to_file(pkg2_hdr->data, pkg2_hdr->sec_size[PKG2_SEC_KERNEL], path))
		goto out;
	gfx_puts("Kernel dumped to kernel.bin\n");

	// Dump INI1.
	emmcsn_path_impl(path, "/pkg2", "ini1.bin", &storage);
	u32 ini1_off = pkg2_hdr->sec_size[PKG2_SEC_KERNEL];
	u32 ini1_size = pkg2_hdr->sec_size[PKG2_SEC_INI1];
	if (!ini1_size)
	{
		ini1_off = *(u32 *)(pkg2_hdr->data + PKG2_NEWKERN_INI1_START);
		ini1_size = *(u32 *)(pkg2_hdr->data + PKG2_NEWKERN_INI1_END) - *(u32 *)(pkg2_hdr->data + PKG2_NEWKERN_INI1_START);
	}
	if (sd_save_to_file(pkg2_hdr->data + ini1_off, ini1_size, path))
		goto out;
	gfx_puts("INI1 dumped to ini1.bin\n");

	gfx_puts("\nDone. Press any key...\n");

out:
	nx_emmc_gpt_free(&gpt);
out_free:
	free(pkg1);
	free(secmon);
	free(warmboot);
	free(loader);
	free(pkg2);
	sdmmc_storage_end(&storage);
	sd_unmount();

	if (kb >= KB_FIRMWARE_VERSION_620)
		se_aes_key_clear(8);

	btn_wait();
}

void _toggle_autorcm(bool enable)
{
	sdmmc_storage_t storage;
	sdmmc_t sdmmc;

	u8 randomXor = 0;

	gfx_clear_partial_grey(BG_COL, 0, 1256);
	gfx_con_setpos(0, 0);

	if (!sdmmc_storage_init_mmc(&storage, &sdmmc, SDMMC_4, SDMMC_BUS_WIDTH_8, 4))
	{
		EPRINTF("Failed to init eMMC.");
		goto out;
	}

	u8 *tempbuf = (u8 *)malloc(0x200);
	sdmmc_storage_set_mmc_partition(&storage, 1);

	int i, sect = 0;
	u8 corr_mod_byte0;
	if ((fuse_read_odm(4) & 3) != 3)
		corr_mod_byte0 = 0xF7;
	else
		corr_mod_byte0 = 0x37;

	for (i = 0; i < 4; i++)
	{
		sect = (0x200 + (0x4000 * i)) / NX_EMMC_BLOCKSIZE;
		sdmmc_storage_read(&storage, sect, 1, tempbuf);

		if (enable)
		{
			do
			{
				randomXor = get_tmr_us() & 0xFF; // Bricmii style of bricking.
			} while (!randomXor); // Avoid the lottery.

			tempbuf[0x10] ^= randomXor;
		}
		else
			tempbuf[0x10] = corr_mod_byte0;
		sdmmc_storage_write(&storage, sect, 1, tempbuf);
	}

	free(tempbuf);
	sdmmc_storage_end(&storage);

	if (enable)
		gfx_printf("%kAutoRCM mode enabled!%k", WARN_TEXT_COL, MAIN_TEXT_COL);
	else
		gfx_printf("%kAutoRCM mode disabled!%k", INFO_TEXT_COL, MAIN_TEXT_COL);
	gfx_printf("\n\nPress any key...\n");

out:
	btn_wait();
}

void _enable_autorcm()  { _toggle_autorcm(true); }
void _disable_autorcm() { _toggle_autorcm(false); }

void menu_autorcm()
{
	gfx_clear_grey(BG_COL);
	gfx_con_setpos(0, 0);

	if (h_cfg.rcm_patched)
	{
		gfx_printf("%kThis device is RCM patched and\nAutoRCM function is disabled.\n\n"
			"In case %kAutoRCM%k is enabled\nthis will %kBRICK%k your device PERMANENTLY!!%k",
			WARN_TEXT_COL, DANGER_ERROR_TEXT_COL, WARN_TEXT_COL, DANGER_ERROR_TEXT_COL, WARN_TEXT_COL, MAIN_TEXT_COL);
		btn_wait();

		return;
	}

	// Do a simple check on the main BCT.
	sdmmc_storage_t storage;
	sdmmc_t sdmmc;
	bool disabled = true;

	if (!sdmmc_storage_init_mmc(&storage, &sdmmc, SDMMC_4, SDMMC_BUS_WIDTH_8, 4))
	{
		EPRINTF("Failed to init eMMC.");
		btn_wait();

		return;
	}

	u8 *tempbuf = (u8 *)malloc(0x200);
	sdmmc_storage_set_mmc_partition(&storage, 1);
	sdmmc_storage_read(&storage, 0x200 / NX_EMMC_BLOCKSIZE, 1, tempbuf);

	if ((fuse_read_odm(4) & 3) != 3)
	{
		if (tempbuf[0x10] != 0xF7)
			disabled = false;
	}
	else
	{
		if (tempbuf[0x10] != 0x37)
			disabled = false;
	}

	free(tempbuf);
	sdmmc_storage_end(&storage);

	// Create AutoRCM menu.
	ment_t *ments = (ment_t *)malloc(sizeof(ment_t) * 6);

	ments[0].type = MENT_BACK;
	ments[0].caption = "Back";

	ments[1].type = MENT_CHGLINE;

	ments[2].type = MENT_CAPTION;
	ments[3].type = MENT_CHGLINE;
	if (disabled)
	{
		ments[2].caption = "Status: Disabled!";
		ments[2].color = INFO_TEXT_COL;
		ments[4].caption = "Enable AutoRCM";
		ments[4].handler = _enable_autorcm;
	}
	else
	{
		ments[2].caption = "Status: Enabled!";
		ments[2].color = WARN_TEXT_COL;
		ments[4].caption = "Disable AutoRCM";
		ments[4].handler = _disable_autorcm;
	}
	ments[4].type = MENT_HDLR_RE;
	ments[4].data = NULL;

	memset(&ments[5], 0, sizeof(ment_t));
	menu_t menu = {ments, "This corrupts your BOOT0!", 0, 0};

	tui_do_menu(&menu);
}

int _fix_attributes(char *path, u32 *total, bool set_bit)
{
	FRESULT res;
	DIR dir;
	u32 dirLength = 0;
	static FILINFO fno;

		// Read file attributes.
		res = f_stat(path, &fno);
		if (res != FR_OK)
			return res;

		// Check if archive bit is set.
		if (!set_bit)
		{
			*(u32 *)total = *total + 1;
			f_chmod(path, 0, AM_ARC);
		} else
		{
			*(u32 *)total = *total + 1;
			f_chmod(path, AM_ARC, AM_ARC);
		}

	// Open directory.
	res = f_opendir(&dir, path);
	if (res != FR_OK)
		return res;

	dirLength = strlen(path);
	for (;;)
	{
		// Clear file or folder path.
		path[dirLength] = 0;

		// Read a directory item.
		res = f_readdir(&dir, &fno);

		// Break on error or end of dir.
		if (res != FR_OK || fno.fname[0] == 0)
			break;

		// Set new directory or file.
		memcpy(&path[dirLength], "/", 1);
		memcpy(&path[dirLength + 1], fno.fname, strlen(fno.fname) + 1);

		if (!set_bit)
		{
			*total = *total + 1;
			f_chmod(path, 0, AM_ARC);
		} else
		{
			*total = *total + 1;
			f_chmod(path, AM_ARC, AM_ARC);
		}
		// Is it a directory?
		if (fno.fattrib & AM_DIR)
		{
			if (!set_bit)
			{
				*(u32 *)total = *total + 1;
				f_chmod(path, 0, AM_ARC);
			} else
			{
				*(u32 *)total = *total + 1;
				f_chmod(path, AM_ARC, AM_ARC);
			}

			// Update status bar.
			tui_sbar(false);

			// Enter the directory.
			res = _fix_attributes(path, total, set_bit);
			if (res != FR_OK)
				break;
		}
	}

	f_closedir(&dir);

	return res;
}

void _fix_sd_attr(u32 type)
{
	gfx_clear_partial_grey(BG_COL, 0, 1256);
	gfx_con_setpos(0, 0);

	char *path = malloc(256);

	u32 total = 0;
	if (sd_mount())
	{
		path = file_browser(NULL, NULL, "Choose a directory", true, true);
		if(!path) return;

		gfx_printf("This may take some time...\n\n");
		_fix_attributes(path, &total, type);
		gfx_printf("%kTotal archive bits changed: %d!%k\n\nPress any key...", INFO_TEXT_COL, total, MAIN_TEXT_COL);
		sd_unmount();
	}
	btn_wait();
}

void set_sd_attr() { _fix_sd_attr(0); }
void unset_sd_attr() { _fix_sd_attr(1); }

void flipVertically(unsigned char* pixels_buffer, const unsigned int width, const unsigned int height, const int bytes_per_pixel)
{
    const unsigned int rows = height / 2; // Iterate only half the buffer to get a full flip
    const unsigned int row_stride = width * bytes_per_pixel;
    unsigned char* temp_row = (unsigned char*)malloc(row_stride);

    int source_offset, target_offset;

    for (int rowIndex = 0; rowIndex < rows; rowIndex++)
    {
        source_offset = rowIndex * row_stride;
        target_offset = (height - rowIndex - 1) * row_stride;

        memcpy(temp_row, pixels_buffer + source_offset, row_stride);
        memcpy(pixels_buffer + source_offset, pixels_buffer + target_offset, row_stride);
        memcpy(pixels_buffer + target_offset, temp_row, row_stride);
    }

    free(temp_row);
    temp_row = NULL;
}

void screenshot()
{
	
	char *screenshotname = malloc(32);
	char *i_chr = malloc(3);
	

	if (sd_mount()){
		f_mkdir ("Screenshots");
		memcpy (screenshotname+0, "Screenshots/screenshot", 23);
		u8 sc_pathln = strlen(screenshotname);
		u8 i = 1;
		while (i < 99){
			itoa (i, i_chr, 10);
			memcpy (screenshotname+sc_pathln, i_chr, strlen(i_chr)+1);
			memcpy (screenshotname+strlen(screenshotname), ".bmp", 5);
			if(!f_stat(screenshotname, NULL)) ++i;
			else break;
			
		}
	screenshot_count = i+1;
	
			
	free(gfx_ctxt.fb);
    s32 width = 720;
    s32 height = 1280;
    u16 bitcount = 32;
	int width_in_bytes = (width * 4);

    u32 imagesize = width_in_bytes * height;
    const u32 bi_size = 40;
    const u32 buf_offset_bits = 54; 

    u32 filesize = 54 + imagesize;

    const u16 bi_planes = 1;
    unsigned char header[54] = { 0 };
    memcpy(header, "BM", 2);
    memcpy(header + 2 , &filesize, 4);
    memcpy(header + 10, &buf_offset_bits, 4);
    memcpy(header + 14, &bi_size, 4);
    memcpy(header + 18, &width, 4);
    memcpy(header + 22, &height, 4);
    memcpy(header + 26, &bi_planes, 2);
    memcpy(header + 28, &bitcount, 2);
    memcpy(header + 34, &imagesize, 4);

    u8* buff = (u8*)malloc(imagesize + 54);
    memcpy(buff, header, 54);
	flipVertically ((unsigned char*)gfx_ctxt.fb, width, height , 4);
    memcpy(buff + 54, gfx_ctxt.fb, imagesize);

if (screenshot_count > 99) screenshot_count = 0;
		if (!sd_save_to_file(buff, imagesize + 54, screenshotname)){
		flipVertically ((unsigned char*)gfx_ctxt.fb, width, height , 4);
		free(buff);
		free(gfx_ctxt.fb);
		gfx_clear_partial_grey(BG_COL, 0, 16);
		gfx_con_setpos(0, 0);
		gfx_printf("Saved as %s", screenshotname);
		msleep(1500);
		gfx_clear_partial_grey(BG_COL, 0, 16);
		++screenshot_count;
	}
	
	}
    return;
}

void fix_fuel_gauge_configuration()
{
	gfx_clear_partial_grey(BG_COL, 0, 1256);
	gfx_con_setpos(0, 0);

	int battVoltage, avgCurrent;

	max17050_get_property(MAX17050_VCELL, &battVoltage);
	max17050_get_property(MAX17050_AvgCurrent, &avgCurrent);

	// Check if still charging. If not, check if battery is >= 95% (4.1V).
	if (avgCurrent < 0 && battVoltage > 4100)
	{
		if ((avgCurrent / 1000) < -10)
			EPRINTF("You need to be connected to a wall adapter,\nto apply this fix!");
		else
		{
			gfx_printf("%kAre you really sure?\nThis will reset your fuel gauge completely!\n", WARN_TEXT_COL);
			gfx_printf("Additionally this will power off your console.\n%k", MAIN_TEXT_COL);

			gfx_puts("\nPress POWER to Continue.\nPress VOL to go to the menu.\n\n\n");

			u32 btn = btn_wait();
			if (btn & BTN_POWER)
			{
				max17050_fix_configuration();
				msleep(1000);
				gfx_con_getpos(&gfx_con.savedx,  &gfx_con.savedy);
				u16 value = 0;
				gfx_printf("%kThe console will power off in 45 seconds.\n%k", WARN_TEXT_COL, MAIN_TEXT_COL);
				while (value < 46)
				{
					gfx_con_setpos(gfx_con.savedx, gfx_con.savedy);
					gfx_printf("%2ds elapsed", value);
					msleep(1000);
					value++;
				}
				msleep(2000);

				power_off();
			}
			return;
		}
	}
	else
		EPRINTF("You need a fully charged battery\nand connected to a wall adapter,\nto apply this fix!");

	msleep(500);
	btn_wait();
}

void reset_pmic_fuel_gauge_charger_config()
{
	int avgCurrent;

	gfx_clear_partial_grey(BG_COL, 0, 1256);
	gfx_con_setpos(0, 0);

	gfx_printf("%k\nThis will wipe your battery stats completely!\n"
		"%kAnd it may not power on without physically\nremoving and re-inserting the battery.\n%k"
		"\nAre you really sure?%k\n", WARN_TEXT_COL, DANGER_ERROR_TEXT_COL, WARN_TEXT_COL, MAIN_TEXT_COL);

	gfx_puts("\nPress POWER to Continue.\nPress VOL to go to the menu.\n\n\n");
	u32 btn = btn_wait();
	if (btn & BTN_POWER)
	{
		gfx_clear_partial_grey(BG_COL, 0, 1256);
		gfx_con_setpos(0, 0);
		gfx_printf("%kKeep the USB cable connected!%k\n\n", WARN_TEXT_COL, MAIN_TEXT_COL);
		gfx_con_getpos(&gfx_con.savedx,  &gfx_con.savedy);

		u8 value = 30;
		while (value > 0)
		{
			gfx_con_setpos(gfx_con.savedx, gfx_con.savedy);
			gfx_printf("%kWait... (%ds)   %k", INFO_TEXT_COL, value, MAIN_TEXT_COL);
			msleep(1000);
			value--;
		}
		gfx_con_setpos(gfx_con.savedx, gfx_con.savedy);

		//Check if still connected.
		max17050_get_property(MAX17050_AvgCurrent, &avgCurrent);
		if ((avgCurrent / 1000) < -10)
			EPRINTF("You need to be connected to a wall adapter\nor PC to apply this fix!");
		else
		{
			// Apply fix.
			bq24193_fake_battery_removal();
			gfx_printf("Done!               \n"
				"%k1. Remove the USB cable\n"
				"2. Press POWER for 15s.\n"
				"3. Reconnect the USB to power-on!%k\n", WARN_TEXT_COL, MAIN_TEXT_COL);
		}
		msleep(500);
		btn_wait();
	}
}

#pragma GCC pop_options