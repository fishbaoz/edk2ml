#;*****************************************************************************
#;
#; Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
#;
#; AMD is granting you permission to use this software (the Materials)
#; pursuant to the terms and conditions of your Software License Agreement
#; with AMD.  This header does *NOT* give you permission to use the Materials
#; or any rights under AMD's intellectual property.  Your use of any portion
#; of these Materials shall constitute your acceptance of those terms and
#; conditions.  If you do not agree to the terms and conditions of the Software
#; License Agreement, please do not use any portion of these Materials.
#;
#; CONFIDENTIALITY:  The Materials and all other information, identified as
#; confidential and provided to you by AMD shall be kept confidential in
#; accordance with the terms and conditions of the Software License Agreement.
#;
#; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
#; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
#; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
#; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
#; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
#; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
#; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
#; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
#; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
#; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
#; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
#; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
#; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
#;
#; AMD does not assume any responsibility for any errors which may appear in
#; the Materials or any other related information provided to you by AMD, or
#; result from use of the Materials or any related information.
#;
#; You agree that you will not reverse engineer or decompile the Materials.
#;
#; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
#; further information, software, technical information, know-how, or show-how
#; available to you.  Additionally, AMD retains the right to modify the
#; Materials at any time, without notice, and is not obligated to provide such
#; modified Materials to you.
#;
#; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
#; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
#; subject to the restrictions as set forth in FAR 52.227-14 and
#; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
#; Government constitutes acknowledgement of AMD's proprietary rights in them.
#;
#; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
#; direct product thereof will be exported directly or indirectly, into any
#; country prohibited by the United States Export Administration Act and the
#; regulations thereunder, without the required authorization from the U.S.
#; government nor will be used for any purpose prohibited by the same.
#;*****************************************************************************

[=============================================================================]
#
# Instructions for building the FchD3Cold ACPI table storage file
#
[=============================================================================]
[Build.Ia32.FCHD3COLD_ACPI_TABLES,Build.x64.FCHD3COLD_ACPI_TABLES]
#
# Check if we have any source to work with.
#
!IFNDEF SECTIONS
!IFNDEF ASL_FILES
!ERROR No ASL source files to build were defined in the INF file
!ENDIF
!ENDIF

#
# Define some macros to simplify changes
#
TARGET_FFS_FILE   = $(BIN_DIR)\$(FILE_GUID)-$(BASE_NAME).ffs

#
# Build FchD3Cold SSDT section
#
$(DEST_DIR)\$(FILE).sec : $(ASL_SOURCE_FILES) $(ASL_FILES)
    $(ASL) $(ASL_FLAGS) -p $(BIN_DIR)\$(FILE).aml $(DEST_DIR)\$(FILE).asl 
    -copy $(BIN_DIR)\$(FILE).aml $(DEST_DIR)\$(FILE).acpi
#    $(ASL) $(ASL_FLAGS) $(DEST_DIR)\$(FILE).asl 
#    -copy $(DEST_DIR)\$(FILE).aml $(DEST_DIR)\$(FILE).acpi
    $(GENSECTION) -I $(DEST_DIR)\$(FILE).acpi -O $(DEST_DIR)\$(FILE).sec -S EFI_SECTION_RAW

#
# Build FFS file
#
#$(TARGET_FFS_FILE) : $(SECTIONS) $(DEST_DIR)\$(FILE).sec
#  $(GENFFSFILE) -B $(DEST_DIR) -P1 $(DEST_DIR)\$(BASE_NAME).pkg -V

#
# Build FFS file
#
$(TARGET_FFS_FILE) : $(SECTIONS) $(DEST_DIR)\$(FILE).sec
  $(GENFFSFILE) -B $(DEST_DIR) -V -P1 <<$(DEST_DIR)\$(BASE_NAME).pkg
PACKAGE.INF
\[.]
BASE_NAME                   = $(BASE_NAME)
FFS_FILEGUID                = $(FILE_GUID)
FFS_FILETYPE                = EFI_FV_FILETYPE_FREEFORM
FFS_ATTRIB_CHECKSUM         = TRUE
IMAGE_SCRIPT =
{
  Compress (Dummy) {
    $(ACPI_SECTIONS)
  }
}
<<KEEP

all : $(TARGET_FFS_FILE)

[=============================================================================]
[Package.FCHD3COLD_ACPI_TABLES.Default]
PACKAGE.INF
\[.]
BASE_NAME                   = $(BASE_NAME)
FFS_FILEGUID                = $(FILE_GUID)
FFS_FILETYPE                = EFI_FV_FILETYPE_FREEFORM
FFS_ATTRIB_CHECKSUM         = TRUE

IMAGE_SCRIPT =
{
  Compress (Dummy) {
    $(ACPI_SECTIONS)
  }
}
