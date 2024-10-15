#pragma once

#include <SPIFFS.h>
#include <lvgl.h>

// 文件打开回调
static void *__lv_fs_spiffs_open_cb(lv_fs_drv_t *drv, const char *path,
                                    lv_fs_mode_t mode) {
  const char *flags = "";
  if (mode == LV_FS_MODE_WR)
    flags = "w";
  else if (mode == LV_FS_MODE_RD)
    flags = "r";
  else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
    flags = "r+";

  File file = SPIFFS.open(path, flags);
  if (!file) return nullptr;

  File *fp = new File(file);
  return fp;
}

// 文件关闭回调
static lv_fs_res_t __lv_fs_spiffs_close_cb(lv_fs_drv_t *drv, void *file_p) {
  File *fp = static_cast<File *>(file_p);
  fp->close();
  delete fp;
  return LV_FS_RES_OK;
}

// 文件读取回调
static lv_fs_res_t __lv_fs_spiffs_read_cb(lv_fs_drv_t *drv, void *file_p,
                                          void *buf, uint32_t btr,
                                          uint32_t *br) {
  File *fp = static_cast<File *>(file_p);
  *br = fp->read(static_cast<uint8_t *>(buf), btr);
  return LV_FS_RES_OK;
}

// 文件写入回调
static lv_fs_res_t __lv_fs_spiffs_write_cb(lv_fs_drv_t *drv, void *file_p,
                                           const void *buf, uint32_t btw,
                                           uint32_t *bw) {
  File *fp = static_cast<File *>(file_p);
  *bw = fp->write(static_cast<const uint8_t *>(buf), btw);
  return LV_FS_RES_OK;
}

// 文件定位回调
static lv_fs_res_t __lv_fs_spiffs_seek_cb(lv_fs_drv_t *drv, void *file_p,
                                          uint32_t pos, lv_fs_whence_t whence) {
  File *fp = static_cast<File *>(file_p);
  if (whence == LV_FS_SEEK_SET)
    fp->seek(pos, SeekSet);
  else if (whence == LV_FS_SEEK_CUR)
    fp->seek(pos, SeekCur);
  else if (whence == LV_FS_SEEK_END)
    fp->seek(pos, SeekEnd);
  return LV_FS_RES_OK;
}

// 文件位置回调
static lv_fs_res_t __lv_fs_spiffs_tell_cb(lv_fs_drv_t *drv, void *file_p,
                                          uint32_t *pos_p) {
  File *fp = static_cast<File *>(file_p);
  *pos_p = fp->position();
  return LV_FS_RES_OK;
}

// 目录打开回调
static void *__lv_fs_spiffs_dir_open_cb(lv_fs_drv_t *drv, const char *path) {
  File dir = SPIFFS.open(path);
  if (!dir || !dir.isDirectory()) return NULL;

  File *dp = new File(dir);
  return dp;
}

// 目录读取回调
static lv_fs_res_t __lv_fs_spiffs_dir_read_cb(lv_fs_drv_t *drv, void *rddir_p,
                                              char *fn) {
  File *dp = static_cast<File *>(rddir_p);
  File entry = dp->openNextFile();
  if (!entry) return LV_FS_RES_FS_ERR;

  strcpy(fn, entry.name());
  return LV_FS_RES_OK;
}

// 目录关闭回调
static lv_fs_res_t __lv_fs_spiffs_dir_close_cb(lv_fs_drv_t *drv,
                                               void *rddir_p) {
  File *dp = static_cast<File *>(rddir_p);
  dp->close();
  delete dp;
  return LV_FS_RES_OK;
}
