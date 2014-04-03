#include "EfiLdr.h"
#include "Debug.h"
#include "corebootmem.h"
#include "SeaBIOS/memmap.h"
#include "SeaBIOS/types.h"
#include "SeaBIOS/util.h"

// globals used by SeaBIOS code
struct e820entry *e820_list;
int e820_count;
int CONFIG_MAX_E820;
u32 RamSize;
u64 RamSizeOver4G;

//---------------------------------------------------------------------------

void buildBiosMemoryMap (BIOS_MEMORY_MAP *BiosMemoryMap)
    {
    e820_list = (void *) &BiosMemoryMap->MemoryMapEntry [0];
    CONFIG_MAX_E820 = BiosMemoryMap->MemoryMapSize / sizeof BiosMemoryMap->MemoryMapEntry [0];
    coreboot_setup (); // build E820 array from coreboot table
    memmap_finalize ();
    }

//---------------------------------------------------------------------------

void warn_noalloc (void)
    {
    PrintString ("warn_noalloc: %a line %d\n", __FILE__, __LINE__);
    {volatile int a=1;while(a);}
    }

//---------------------------------------------------------------------------

void cbfs_setup (void) {};
void vgahook_setup (const char *vendor, const char *part) {}

//---------------------------------------------------------------------------

void EFIAPI dprintf (int level, char *format, ...)
    {
    VA_LIST marker;
    static char buffer [200];
    char *position;

    // UEFI uses %a for ascii and %s for unicode
    for (position = format; position [0] != '\0' && position [1] != '\0'; position++)
        if (position [0] == '%' && position [1] == 's') position [1] = 'a';
    VA_START (marker, format);
    AsciiVSPrint (buffer, sizeof buffer, format, marker);
    VA_END (marker);
    PrintString (buffer);
    }

//---------------------------------------------------------------------------
/* memmove.c -- copy memory.
   Copy LENGTH bytes from SOURCE to DEST.  Does not null-terminate.
   In the public domain.
   By David MacKenzie <djm@gnu.ai.mit.edu>.  */

void *memmove (void *dest0, void const *source0, size_t length)
{
  char *dest = dest0;
  char const *source = source0;
  if (source < dest)
    /* Moving from low mem to hi mem; start at end.  */
    for (source += length, dest += length; length; --length)
      *--dest = *--source;
  else if (source != dest)
    {
      /* Moving from hi mem to low mem; start at beginning.  */
      for (; length; --length)
	*dest++ = *source++;
    }
  return dest0;
}

//---------------------------------------------------------------------------
