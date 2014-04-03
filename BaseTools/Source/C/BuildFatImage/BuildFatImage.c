// enable C99 print format specifiers for mingw port of gcc
#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <stdarg.h>

//----------------------------------------------------------------------------

#pragma pack (push, 1)
typedef struct
    {
    uint8_t     bootIndicator;
    uint8_t     startHead;
    uint8_t     startSector;
    uint8_t     startCylinder;
    uint8_t     mediaType;
    uint8_t     endHead;
    uint8_t     endSector;
    uint8_t     endCylinder;
    uint8_t     partitionStartLba [4];
    uint8_t     totalSectors [4];
    }
PART;

typedef struct
    {
    uint8_t     code [0x1BE];
    PART        part [4];     
    }
MBR;

typedef struct
    {
    uint8_t     jmp [3];
    uint8_t     oemId [8];
    uint8_t     bytesPerSector [2];
    uint8_t     sectorsPerCluster;
    uint8_t     reservedSectors [2];
    uint8_t     numberOfFats;
    uint8_t     maxFiles [2];
    uint8_t     totalSectorsShort [2];
    uint8_t     mediaType;
    uint8_t     sectorsPerFat [2];
    uint8_t     sectorsPerTrack [2];
    uint8_t     headsPerCylinder [2];
    uint8_t     hiddenSectors [4];
    uint8_t     totalSectorsLong [4];
    uint8_t     physicalDrive;
    uint8_t     currentHead;
    uint8_t     signature;
    uint8_t     id [4];
    uint8_t     fatLabel [11];
    uint8_t     systemId [8];
    }
FAT16;

typedef struct
    {
    uint8_t     name [8];
    uint8_t     extn [3];
    uint8_t     attributes;
    uint8_t     doNotUse;
    uint8_t     createTimeFine;
    uint8_t     createTime [2];
    uint8_t     createDate [2];
    uint8_t     accessDate [2];
    uint8_t     accessRights [2];
    uint8_t     modifyTime [2];
    uint8_t     modifyDate [2];
    uint8_t     startingCluster [2];
    uint8_t     fileSize [4];
    }
DIR_ENTRY;
#pragma pack (pop)

//----------------------------------------------------------------------------
//
// formatMessage - return formatted message in static buffer
//
static char *formatMessage (char *format, ...)
    {
    va_list marker;
    static char buffer [4][200];
    static int cycle;
    char *position = buffer [cycle];
    cycle++;
    if (cycle == 4) cycle = 0;

    va_start (marker, format);
    vsprintf (position, format, marker);
    va_end (marker);
    return position;
    }

//----------------------------------------------------------------------------
//
// command line help goes here
//
static char *helpScreen (void)
    {
    printf("\nbuildfat 1.0, copyright 2013 Scott Duplichan\n");
    printf("use buildfat mbr=mbr_file bs=bs16_file ldr=efildr16_file out=image_file\n");
    return NULL;
    }

//----------------------------------------------------------------------------

static char *readfile (FILE *stream, size_t *filelength, char **retbuffer)
    {
    char *buffer = NULL;
    size_t readcount = 0;
    for (;;)
        {
        int ch = fgetc (stream);
        if (ch == EOF) break;
        buffer = realloc (buffer, readcount + 1);
        if (buffer == NULL) return "out of memory";
        buffer [readcount++] = ch;
        }
    *filelength = readcount;
    *retbuffer = buffer;
    return NULL;
    }

//----------------------------------------------------------------------------

static char *writeFile (void *buffer, size_t bufferSize, FILE *stream)
    {
    size_t count = fwrite (buffer, 1, bufferSize, stream);
    if (count != bufferSize) return formatMessage ("fwrite failed, %zd of %zd", count, bufferSize);
    return NULL;
    }

//----------------------------------------------------------------------------

static int writeLittleEndian16 (void *buffer, int data)
    {
    uint8_t *start = buffer, *position = buffer;
    unsigned int udata = data;
    *position++ = udata & 0xFF;
    *position++ = udata >> 8;
    return position - start;
    }

//----------------------------------------------------------------------------

static int writeLittleEndian32 (void *buffer, int data)
    {
    uint8_t *start = buffer, *position = buffer;
    uint32_t udata = data;
    *position++ = udata & 0xFF;
    *position++ = udata >> 8;
    *position++ = udata >> 16;
    *position++ = udata >> 24;
    return position - start;
    }

//----------------------------------------------------------------------------

static int readLittleEndian16 (void *buffer)
    {
    uint8_t *position = buffer;
    return position [0] + position [1] * 256;
    }

//----------------------------------------------------------------------------

static int writeDirEntry (void *buffer, char *name, char *extn, int attributes, int startingCluster, int fileSize)
    {
    DIR_ENTRY *dirEntry = buffer;

    memset (dirEntry, 0, sizeof *dirEntry);
    memcpy (dirEntry->name, name, sizeof dirEntry->name);
    memcpy (dirEntry->extn, extn, sizeof dirEntry->extn);
    dirEntry->attributes = attributes;
    writeLittleEndian16 (dirEntry->startingCluster, startingCluster);
    writeLittleEndian32 (dirEntry->fileSize, fileSize);
    return sizeof *dirEntry;
    }

//----------------------------------------------------------------------------
//
// lba  - block address                 0 - x
// heads - number of heads per cylinder 1 - 255
// sectors - number of sectors per head 1 - 64
// return c                             0 - x
// return h                             0 - heads-1
// return s                             0 - sectors-1
// lba = c * heads + h * sectors + s
//
static void lbaToChs (int lba, int heads, int sectors, int *c, int *h, int *s)
    {
    int sectorsPerCylinder, sectorsPerHead;
    
    sectorsPerCylinder = sectors * heads;
    sectorsPerHead = sectors;
    *c = lba / sectorsPerCylinder;
    lba -= *c *sectorsPerCylinder;
    *h = lba / sectorsPerHead; 
    lba -= *h *sectorsPerHead;
    *s = lba;
    }

//----------------------------------------------------------------------------
//
// roundUp - round an integer value up to a multiple of n
//
int roundUp (int value, int n)
   {
   value += n - 1;
   value -= value % n;
   return value;
   }

//----------------------------------------------------------------------------

static void *recalloc (void *buffer, size_t oldSize, size_t newSize)
    {
    uint8_t *newBuffer = realloc (buffer, newSize);
    if (newSize > oldSize)
        memset (newBuffer + oldSize, 0, newSize - oldSize);
    return newBuffer;
    }

//----------------------------------------------------------------------------

static char *runmain (int argc, char *argv [])
    {
    char    *error;
    int     argCount, bytesPerSector, bytesPerCluster, fatCopy, dirSectors, maxFiles, reservedSectors, fatSectorCount;
    int     partitionSectorCount, partitionStartLba, clusterCount;
    size_t  mbrcount, ldrcount, ldrCountPadded, bscount, sectorsPerFat;
    char    *outfile = NULL, *mbrfile = NULL, *bsfile = NULL, *ldrfile = NULL;
    char    *mbrbuffer, *ldrbuffer, *bsbuffer, *fatBuffer, *fatStart, *position;
    FILE    *outstream = NULL, *mbrstream = NULL, *bsstream = NULL, *ldrstream = NULL;
    FAT16   *bs;
    MBR     *mbr;

    if (argc == 1) return helpScreen ();

    argCount = argc;
    while (--argCount)
        {
        char *position = argv [argCount];

        if (memcmp (position, "out=", 4) == 0)
            outfile = position + 4;
        else if (memcmp (position, "mbr=", 4) == 0)
            mbrfile = position + 4;
        else if (memcmp (position, "bs=", 3) == 0)
            bsfile = position + 3;
        else if (memcmp (position, "ldr=", 4) == 0)
            ldrfile = position + 4;
        else return formatMessage ("unrecognized option: \"%s\"", position);
        }

    if (outfile == NULL) return "out= option is required";
    if (mbrfile == NULL) return "mbr= option is required";
    if (ldrfile == NULL) return "ldr= option is required";
    if (bsfile  == NULL) return "bs= option is required";

    outstream = fopen (outfile, "wb");
    mbrstream = fopen (mbrfile, "rb");
    bsstream  = fopen (bsfile , "rb");
    ldrstream = fopen (ldrfile, "rb");

    if (outstream == NULL) return formatMessage ("failed to create outfile (%s)", outfile);
    if (mbrstream == NULL) return formatMessage ("failed to open mbrfile (%s)", mbrfile);
    if (bsstream  == NULL) return formatMessage ("failed to open bsfile (%s)", bsfile);
    if (ldrstream == NULL) return formatMessage ("failed to open ldrfile (%s)", ldrfile);

    error = readfile (mbrstream, &mbrcount, &mbrbuffer);
    if (error) return error;
    if (mbrcount != 512) return formatMessage ("mbr size %zd, expected 512", mbrcount);
    error = readfile (bsstream, &bscount, &bsbuffer);
    if (error) return error;
    if (bscount != 512) return formatMessage ("bs size %zd, expected 512", bscount);
    error = readfile (ldrstream, &ldrcount, &ldrbuffer);
    if (error) return error;


    bs = (void *) bsbuffer;
    mbr = (void *) mbrbuffer;

    // use standard sector size
    bytesPerSector = 512;
    writeLittleEndian16 (bs->bytesPerSector, bytesPerSector);

    // sectors per cluster: use a large number to reduce FAT size
    // too big waste space when padding the file to a cluster multiple
    bs->sectorsPerCluster = 16;

    // partition needs to be 16MB minimum so that it is interpreted as FAT16
    // and not FAT12 by disk utilities. The unused space at the end will be
    // trimmed so that extra partition size does not add to the image size.
    partitionSectorCount = 32 * 0x100000 / bytesPerSector;

    // start the partition (almost) immediately after the MBR sector
    // (use 2 instead of 1 for compatibility with Award BIOS on AMD Solo)
    partitionStartLba = 2;

    // one reserved sector is needed for the partition sector
    reservedSectors = 1;
    writeLittleEndian16 (bs->reservedSectors, reservedSectors);
    // deduct one because we write partition sector separately
    reservedSectors--;

    // no need for multiple FATs
    bs->numberOfFats = 1;
    
    // use a minimum number of root directory entries
    dirSectors = 1;
    maxFiles = dirSectors * bytesPerSector / sizeof (DIR_ENTRY);
    writeLittleEndian16 (bs->maxFiles, maxFiles);

    // other misc values
    bs->mediaType = 0xF8;
    bs->physicalDrive = 0x80;
    bs->signature = 0x29;
    writeLittleEndian16 (bs->headsPerCylinder, 255);
    writeLittleEndian16 (bs->sectorsPerTrack, 63);
    memcpy (bs->oemId, "INTEL   ", 8);
    memcpy (bs->systemId, "FAT16   ", 8);


    // pad the file to a whole number of clusters
    bytesPerCluster = bytesPerSector * bs->sectorsPerCluster;
    ldrCountPadded = roundUp (ldrcount, bytesPerCluster);
    ldrbuffer = recalloc (ldrbuffer, ldrcount, ldrCountPadded);
    if (!mbrbuffer) return "out of memory";

    sectorsPerFat = partitionSectorCount / bs->sectorsPerCluster / bytesPerSector * sizeof (short);
    writeLittleEndian16 (bs->sectorsPerFat, sectorsPerFat);
    writeLittleEndian32 (bs->hiddenSectors, partitionStartLba);
    fatSectorCount = reservedSectors + dirSectors + bs->numberOfFats * sectorsPerFat;

    // fill in partition ending chs in mbr
    {
    int clyinder, head, sector;
    int heads, sectors;

    heads = readLittleEndian16 (bs->headsPerCylinder);
    sectors = readLittleEndian16 (bs->sectorsPerTrack);

    lbaToChs (partitionStartLba, heads, sectors, &clyinder, &head, &sector);
    mbr->part [0].mediaType = 4; // FAT16
    mbr->part [0].bootIndicator = 0x80; // bootable
    mbr->part [0].startCylinder = clyinder;
    mbr->part [0].startHead = head;
    mbr->part [0].startSector = sector + 1;
    lbaToChs (partitionStartLba + partitionSectorCount - 1, heads, sectors, &clyinder, &head, &sector);
    mbr->part [0].endCylinder = clyinder;
    mbr->part [0].endHead = head;
    mbr->part [0].endSector = sector + 1;
    }

    writeLittleEndian32 (mbr->part [0].partitionStartLba, partitionStartLba);
    writeLittleEndian32 (mbr->part [0].totalSectors, partitionSectorCount);
    if (partitionSectorCount <= 0xFFFF)
        writeLittleEndian16 (bs->totalSectorsShort, partitionSectorCount);
    else
        writeLittleEndian32 (bs->totalSectorsLong, partitionSectorCount);

    // add padding between MBR and first partition
    mbrbuffer = recalloc (mbrbuffer, mbrcount, partitionStartLba * bytesPerSector);
    if (!mbrbuffer) return "out of memory";

    // write the MBR (and padding that follows)
    error = writeFile (mbrbuffer, partitionStartLba * bytesPerSector, outstream);
    if (error) return error;

    error = writeFile (bsbuffer, bscount, outstream);
    if (error) return error;

    // now build the FAT16 part
    fatBuffer = calloc (1, fatSectorCount * bytesPerSector);
    if (!fatBuffer) return "out of memory";


    // FAT (multiple copies possible)
    fatStart = fatBuffer + reservedSectors * bytesPerSector;
    clusterCount = ldrCountPadded / bytesPerCluster + 2;
    for (fatCopy = 0; fatCopy < bs->numberOfFats; fatCopy++)
        {
        int cluster;
        position = fatStart + fatCopy * sectorsPerFat * bytesPerSector;

        // first two clusters are reserved
        position += writeLittleEndian16 (position, 0xFFF8);
        position += writeLittleEndian16 (position, 0xFFFF);
        for (cluster = 3; cluster < clusterCount; cluster++)
            position += writeLittleEndian16 (position, cluster);
        position += writeLittleEndian16 (position, 0xFFFF);
        }

    // skip past FATs to directory entries
    position = fatStart + bs->numberOfFats * sectorsPerFat * bytesPerSector;

    // write volume label
    position += writeDirEntry (position, "UEFI    ", "   ", 8, 0, 0);

    // write file entry
    position += writeDirEntry (position, "EFILDR16", "   ", 0, 2, ldrcount);

    // write the FAT data
    error = writeFile (fatBuffer, fatSectorCount * bytesPerSector, outstream);
    if (error) return error;

    // add padding after fill to fill out the partition
    //paddedLdrSectors = partitionSectorCount;
    //ldrbuffer = realloc (ldrbuffer, paddedLdrSectors * bytesPerSector);
    //if (!ldrbuffer) return "out of memory";
    //memset (ldrbuffer + ldrcount, 0, paddedLdrSectors * bytesPerSector - ldrcount);

    // write the file data (and padding that follows)
    error = writeFile (ldrbuffer, ldrCountPadded, outstream);
    if (error) return error;

    free (fatBuffer);
    fclose (outstream);
    fclose (mbrstream);
    fclose (bsstream);
    fclose (ldrstream);
    return NULL;
    }

//----------------------------------------------------------------------------

int main (int argc, char *argv [])
    {
    char *error;
    
    error = runmain (argc, argv);
    if (error) return printf ("%s\n", error), 1;
    return 0;
    }

//----------------------------------------------------------------------------
