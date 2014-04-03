void warn_noalloc (void);
void EFIAPI dprintf (int level, char *fmt, ...);
void *memmove(void *d, const void *s, size_t len);
void vgahook_setup(const char *vendor, const char *part);
void coreboot_setup(void);
void cbfs_setup (void);

extern u32 RamSize;
extern u64 RamSizeOver4G;
