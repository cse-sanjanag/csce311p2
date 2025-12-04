/* fs.c — Tiny RAM filesystem parser
   ----------------------------------
   The .initramfs section contains a sequence of:
       [u32 name_len][u32 file_size][name][file_bytes]
   This parser walks that region and exposes file entries.
*/

#include <stdint.h>
#include <stddef.h>
#include <string.h>

extern char _initramfs_start[];
extern char _initramfs_end[];

struct file_entry {
    const char *name;
    const uint8_t *data;
    uint32_t size;
};

static struct file_entry files[16];
static int file_count = 0;

/* Parse initramfs into file_entry table */
void fs_init(void) {
    uint8_t *p = (uint8_t*)_initramfs_start;
    uint8_t *end = (uint8_t*)_initramfs_end;

    file_count = 0;

    while (p + 8 <= end && file_count < 16) {

        uint32_t name_len = *(uint32_t*)p; p += 4;
        uint32_t size = *(uint32_t*)p; p += 4;

        if (p + name_len + size > end) break;

        files[file_count].name = (const char*)p;
        p += name_len;

        files[file_count].data = p;
        files[file_count].size = size;

        p += size;

        file_count++;
    }
}

int fs_count(void) { return file_count; }
const struct file_entry* fs_get_by_index(int i) {
    if (i < 0 || i >= file_count) return NULL;
    return &files[i];
}

/* strcmp-based lookup */
const struct file_entry* fs_find(const char *name) {
    for (int i=0;i<file_count;i++) {
        if (strcmp(files[i].name, name) == 0)
            return &files[i];
    }
    return NULL;
}
