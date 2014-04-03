/** @file

Copyright (c) 2006 - 2011, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:
  HobGeneration.c

Abstract:

Revision History:

**/
#include "DxeIpl.h"
#include "HobGeneration.h"
#include "PpisNeededByDxeCore.h"
#include "FlashLayout.h"
#include "Debug.h"

#define EFI_CPUID_EXTENDED_FUNCTION  0x80000000
#define CPUID_EXTENDED_ADD_SIZE      0x80000008

HOB_TEMPLATE  gHobTemplate = {
  { // Phit
    {  // Header
      EFI_HOB_TYPE_HANDOFF,                 // HobType
      sizeof (EFI_HOB_HANDOFF_INFO_TABLE),  // HobLength
      0                                     // Reserved
    },
    EFI_HOB_HANDOFF_TABLE_VERSION,          // Version
    BOOT_WITH_FULL_CONFIGURATION,           // BootMode
    0,                                      // EfiMemoryTop
    0,                                      // EfiMemoryBottom
    0,                                      // EfiFreeMemoryTop
    0,                                      // EfiFreeMemoryBottom
    0                                       // EfiEndOfHobList
  }, 
  { // Bfv
    {
      EFI_HOB_TYPE_FV,                      // HobType
      sizeof (EFI_HOB_FIRMWARE_VOLUME),     // HobLength
      0                                     // Reserved
    },
    0,                                      // BaseAddress
    0                                       // Length
  },
  { // BfvResource
    {
      EFI_HOB_TYPE_RESOURCE_DESCRIPTOR,     // HobType
      sizeof (EFI_HOB_RESOURCE_DESCRIPTOR), // HobLength
      0                                     // Reserved
    },
    {
      0                                     // Owner Guid
    },
    EFI_RESOURCE_FIRMWARE_DEVICE,           // ResourceType
    (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
     EFI_RESOURCE_ATTRIBUTE_TESTED |
     EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE),  // ResourceAttribute
    0,                                              // PhysicalStart
    0                                               // ResourceLength
  },
  { // Cpu
    { // Header
      EFI_HOB_TYPE_CPU,                     // HobType
      sizeof (EFI_HOB_CPU),                 // HobLength
      0                                     // Reserved
    },
    52,                                     // SizeOfMemorySpace - Architecture Max
    16,                                     // SizeOfIoSpace,
    {
      0, 0, 0, 0, 0, 0                      // Reserved[6]
    }
  },
  {   // Stack HOB
    {   // header
      EFI_HOB_TYPE_MEMORY_ALLOCATION,               // Hob type
      sizeof (EFI_HOB_MEMORY_ALLOCATION_STACK),     // Hob size
      0                                             // reserved
    },
    {
      EFI_HOB_MEMORY_ALLOC_STACK_GUID,
      0x0,                                          // EFI_PHYSICAL_ADDRESS  MemoryBaseAddress;
      0x0,                                          // UINT64                MemoryLength;
      EfiBootServicesData,                          // EFI_MEMORY_TYPE       MemoryType;  
      {0, 0, 0, 0}                                  // Reserved              Reserved[4]; 
    }
  },
  { // MemoryAllocation for HOB's & Images
    {
      EFI_HOB_TYPE_MEMORY_ALLOCATION,               // HobType
      sizeof (EFI_HOB_MEMORY_ALLOCATION),           // HobLength
      0                                             // Reserved
    },
    {
      {
        0, //EFI_HOB_MEMORY_ALLOC_MODULE_GUID       // Name
      },
      0x0,                                          // EFI_PHYSICAL_ADDRESS  MemoryBaseAddress;
      0x0,                                          // UINT64                MemoryLength;
      EfiBootServicesData,                          // EFI_MEMORY_TYPE       MemoryType;  
      {
        0, 0, 0, 0                                  // Reserved              Reserved[4]; 
      }
    }
   },
  { // MemoryFreeUnder1MB for unused memory that DXE core will claim
    {
      EFI_HOB_TYPE_RESOURCE_DESCRIPTOR,             // HobType
      sizeof (EFI_HOB_RESOURCE_DESCRIPTOR),         // HobLength
      0                                             // Reserved
    },
    {
      0                                             // Owner Guid
    },
    EFI_RESOURCE_SYSTEM_MEMORY,                     // ResourceType
    (EFI_RESOURCE_ATTRIBUTE_PRESENT                 |
     EFI_RESOURCE_ATTRIBUTE_TESTED                  |
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED             |
     EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE             | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE       | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE),     
    0x0,                                            // PhysicalStart
    0                                               // ResourceLength
  },
  { // MemoryFreeAbove1MB for unused memory that DXE core will claim
    {
      EFI_HOB_TYPE_RESOURCE_DESCRIPTOR,             // HobType
      sizeof (EFI_HOB_RESOURCE_DESCRIPTOR),         // HobLength
      0                                             // Reserved
    },
    {
      0                                             // Owner Guid
    },
    EFI_RESOURCE_SYSTEM_MEMORY,                     // ResourceType
    (EFI_RESOURCE_ATTRIBUTE_PRESENT                 |
     EFI_RESOURCE_ATTRIBUTE_TESTED                  |
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED             |
     EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE             | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE       | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE),     
    0x0,                                            // PhysicalStart
    0                                               // ResourceLength
  },
  { // MemoryFreeAbove4GB for unused memory that DXE core will claim
    {
      EFI_HOB_TYPE_RESOURCE_DESCRIPTOR,             // HobType
      sizeof (EFI_HOB_RESOURCE_DESCRIPTOR),         // HobLength
      0                                             // Reserved
    },
    {
      0                                             // Owner Guid
    },
    EFI_RESOURCE_SYSTEM_MEMORY,                     // ResourceType
    (EFI_RESOURCE_ATTRIBUTE_PRESENT                 |
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED             |
     EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE             | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE       | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE),     
    0x0,                                            // PhysicalStart
    0                                               // ResourceLength
  },
  {   // Memory Allocation Module for DxeCore
    {   // header
      EFI_HOB_TYPE_MEMORY_ALLOCATION,               // Hob type
      sizeof (EFI_HOB_MEMORY_ALLOCATION_MODULE),    // Hob size
      0                                             // reserved
    },
    {
      EFI_HOB_MEMORY_ALLOC_MODULE_GUID,
      0x0,                                          // EFI_PHYSICAL_ADDRESS  MemoryBaseAddress;
      0x0,                                          // UINT64                MemoryLength;
      EfiBootServicesCode,                          // EFI_MEMORY_TYPE       MemoryType;  
      {
        0, 0, 0, 0                                  // UINT8                 Reserved[4]; 
      },
    },
    DXE_CORE_FILE_NAME_GUID,
    0x0                                             //  EFI_PHYSICAL_ADDRESS of EntryPoint;
  },
  { // MemoryDxeCore
    {
      EFI_HOB_TYPE_RESOURCE_DESCRIPTOR,             // HobType
      sizeof (EFI_HOB_RESOURCE_DESCRIPTOR),         // HobLength
      0                                             // Reserved
    },
    {
      0                                             // Owner Guid
    },
    EFI_RESOURCE_SYSTEM_MEMORY,                     // ResourceType
    (EFI_RESOURCE_ATTRIBUTE_PRESENT                 |
//     EFI_RESOURCE_ATTRIBUTE_TESTED                  | // Do not mark as TESTED, or DxeCore will find it and use it before check Allocation
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED             |
     EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE             | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE       | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE | 
     EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE),     
    0x0,                                            // PhysicalStart
    0                                               // ResourceLength
  },
  { // Memory Map Hints to reduce fragmentation in the memory map
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,                    // Hob type
        sizeof (MEMORY_TYPE_INFORMATION_HOB),           // Hob size
        0,                                              // reserved
      },
      EFI_MEMORY_TYPE_INFORMATION_GUID
    },
    {
      {
        EfiACPIReclaimMemory,
        0x80
      },  // 0x80 pages = 512k for ASL
      {
        EfiACPIMemoryNVS,
        0x100
      },  // 0x100 pages = 1024k for S3, SMM, etc
      {
        EfiReservedMemoryType,
        0x04
      },  // 16k for BIOS Reserved
      {
        EfiRuntimeServicesData,
        0x100
      },
      {
        EfiRuntimeServicesCode,
        0x100
      },
      {
        EfiBootServicesCode,
        0x200
      },
      {
        EfiBootServicesData,
        0x200
      },
      {
        EfiLoaderCode,
        0x100
      },
      {
        EfiLoaderData,
        0x100
      },
      {
        EfiMaxMemoryType,
        0
      }
    }
  },
  { // Pointer to ACPI Table
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (TABLE_HOB),                // Hob size
        0                                  // reserved
      },
      EFI_ACPI_TABLE_GUID
    },
    0
  },
  { // Pointer to ACPI20 Table
    {
      {  
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (TABLE_HOB),                // Hob size
        0                                  // reserved
      },
      EFI_ACPI_20_TABLE_GUID
    },
    0
  },
  { // Pointer to SMBIOS Table
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (TABLE_HOB),                // Hob size
        0                                  // reserved
      },
      SMBIOS_TABLE_GUID
    },
    0
  },
  { // Pointer to MPS Table
    {
      {
         EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (TABLE_HOB),                // Hob size
        0,                                 // reserved
      },
      EFI_MPS_TABLE_GUID
    },
    0
  },
  /**
  { // Pointer to FlushInstructionCache
    EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
    sizeof (PROTOCOL_HOB),             // Hob size
    0,                                 // reserved
    EFI_PEI_FLUSH_INSTRUCTION_CACHE_GUID,
    NULL
  },
  { // Pointer to TransferControl
    EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
    sizeof (PROTOCOL_HOB),             // Hob size
    0,                                 // reserved
    EFI_PEI_TRANSFER_CONTROL_GUID,
    NULL
  },
  { // Pointer to PeCoffLoader
    EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
    sizeof (PROTOCOL_HOB),             // Hob size
    0,                                 // reserved
    EFI_PEI_PE_COFF_LOADER_GUID,
    NULL
  },
  { // Pointer to EfiDecompress
    EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
    sizeof (PROTOCOL_HOB),             // Hob size
    0,                                 // reserved
    EFI_DECOMPRESS_PROTOCOL_GUID,
    NULL
  },
  { // Pointer to TianoDecompress
    EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
    sizeof (PROTOCOL_HOB),             // Hob size
    0,                                 // reserved
    EFI_TIANO_DECOMPRESS_PROTOCOL_GUID,
    NULL
  },
  **/
  { // Pointer to ReportStatusCode
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (PROTOCOL_HOB),             // Hob size
        0                                  // reserved
      },
      EFI_STATUS_CODE_RUNTIME_PROTOCOL_GUID
    },
    0
  },
  { // EFILDR Memory Descriptor
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (MEMORY_DESC_HOB),          // Hob size
        0                                  // reserved
      },
      LDR_MEMORY_DESCRIPTOR_GUID
    },
    0,
    NULL
  },
  { // Pci Express Base Address Hob
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (PCI_EXPRESS_BASE_HOB),     // Hob size
        0                                  // reserved
      },
      EFI_PCI_EXPRESS_BASE_ADDRESS_GUID
    },
    {
      0,
      0,
      0,
    }
  },
  { // Acpi Description Hob
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (ACPI_DESCRIPTION_HOB),     // Hob size
        0                                  // reserved
      },
      EFI_ACPI_DESCRIPTION_GUID
    },
    {
      {
        0,
      },
    }
  },
  { // NV Storage FV Resource
    {
      EFI_HOB_TYPE_RESOURCE_DESCRIPTOR,     // HobType
      sizeof (EFI_HOB_RESOURCE_DESCRIPTOR), // HobLength
      0                                     // Reserved
    },
    {
      0                                     // Owner Guid
    },
    EFI_RESOURCE_FIRMWARE_DEVICE,           // ResourceType
    (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
     EFI_RESOURCE_ATTRIBUTE_TESTED |
     EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE),  // ResourceAttribute
    0,                                              // PhysicalStart (Fixed later)
    NV_STORAGE_FVB_SIZE                             // ResourceLength
  },
  { // FVB holding NV Storage
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (FVB_HOB),
        0
      },
      EFI_FLASH_MAP_HOB_GUID
    },
    {
      {0, 0, 0},                       // Reserved[3]
      EFI_FLASH_AREA_GUID_DEFINED,     // AreaType
      EFI_SYSTEM_NV_DATA_FV_GUID ,     // AreaTypeGuid
      1,
      {
        {
          EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV, // SubAreaData.Attributes
          0,                             // SubAreaData.Reserved
          0,                             // SubAreaData.Base (Fixed later)
          NV_STORAGE_FVB_SIZE,           // SubAreaData.Length
          EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL_GUID // SubAreaData.FileSystem
        }
      }, 
      0,                               // VolumeSignature (Fixed later)
      NV_STORAGE_FILE_PATH,            // Mapped file without padding
                                       //  TotalFVBSize = FileSize + PaddingSize = multiple of BLOCK_SIZE
      NV_STORAGE_SIZE + EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH,
                                       // ActuralSize
      EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH
    }
  },
  { // NV Storage Hob
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (FVB_HOB),                  // Hob size
        0                                  // reserved
      },
      EFI_FLASH_MAP_HOB_GUID
    },
    {
      {0, 0, 0},                       // Reserved[3]
      EFI_FLASH_AREA_EFI_VARIABLES,    // AreaType
      { 0 },                           // AreaTypeGuid
      1,
      {
        {
          EFI_FLASH_AREA_SUBFV | EFI_FLASH_AREA_MEMMAPPED_FV, // SubAreaData.Attributes
          0,                             // SubAreaData.Reserved
          0,                             // SubAreaData.Base (Fixed later)
          NV_STORAGE_SIZE,               // SubAreaData.Length
          EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL_GUID // SubAreaData.FileSystem
        }
      }, 
      0,
      NV_STORAGE_FILE_PATH,
      NV_STORAGE_SIZE,
      0
    }
  },
  { // NV Ftw FV Resource
    {
      EFI_HOB_TYPE_RESOURCE_DESCRIPTOR,     // HobType
      sizeof (EFI_HOB_RESOURCE_DESCRIPTOR), // HobLength
      0                                     // Reserved
    },
    {
      0                                     // Owner Guid
    },
    EFI_RESOURCE_FIRMWARE_DEVICE,           // ResourceType
    (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
     EFI_RESOURCE_ATTRIBUTE_TESTED |
     EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE),  // ResourceAttribute
    0,                                              // PhysicalStart (Fixed later)
    NV_FTW_FVB_SIZE                                 // ResourceLength
  },  
  { // FVB holding FTW spaces including Working & Spare space
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (FVB_HOB),
        0
      },
      EFI_FLASH_MAP_HOB_GUID
    },
    {
      {0, 0, 0},                       // Reserved[3]
      EFI_FLASH_AREA_GUID_DEFINED,     // AreaType
      EFI_SYSTEM_NV_DATA_FV_GUID,      // AreaTypeGuid
      1,
      {
        {
          EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV, // SubAreaData.Attributes
          0,                             // SubAreaData.Reserved
          0,                             // SubAreaData.Base (Fixed later)
          NV_FTW_FVB_SIZE,               // SubAreaData.Length
          EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL_GUID   // SubAreaData.FileSystem
        }
      }, 
      0,
      L"",                             // Empty String indicates using memory
      0,
      0
    }
  },
  { // NV Ftw working Hob
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (FVB_HOB),                  // Hob size
        0                                  // reserved
      },
      EFI_FLASH_MAP_HOB_GUID
    },
    {
      {0, 0, 0},                       // Reserved[3]
      EFI_FLASH_AREA_FTW_STATE,        // AreaType
      { 0 },                           // AreaTypeGuid
      1,
      {
        {
          EFI_FLASH_AREA_SUBFV | EFI_FLASH_AREA_MEMMAPPED_FV, // SubAreaData.Attributes
          0,                             // SubAreaData.Reserved
          0,                             // SubAreaData.Base (Fixed later)
          NV_FTW_WORKING_SIZE,           // SubAreaData.Length
          EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL_GUID // SubAreaData.FileSystem
        }
      }, 
      0,                               // VolumeSignature
      L"",
      0,
      0
    }
  },
  { // NV Ftw spare Hob
    {
      {
        EFI_HOB_TYPE_GUID_EXTENSION,       // Hob type
        sizeof (FVB_HOB),                  // Hob size
        0                                  // reserved
      },
      EFI_FLASH_MAP_HOB_GUID
    },
    {
      {0, 0, 0},                       // Reserved[3]
      EFI_FLASH_AREA_FTW_BACKUP,       // AreaType
      { 0 },                           // AreaTypeGuid
      1,
      {
        {
          EFI_FLASH_AREA_SUBFV | EFI_FLASH_AREA_MEMMAPPED_FV, // SubAreaData.Attributes
          0,                             // SubAreaData.Reserved
          0,                             // SubAreaData.Base (Fixed later)
          NV_FTW_SPARE_SIZE,             // SubAreaData.Length
          EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL_GUID // SubAreaData.FileSystem
        }
      }, 
      0,
      L"",
      0,
      0
    }
  },
  { // EndOfHobList
    EFI_HOB_TYPE_END_OF_HOB_LIST,      // HobType
    sizeof (EFI_HOB_GENERIC_HEADER),   // HobLength
    0                                  // Reserved
  }
};

HOB_TEMPLATE  *gHob = &gHobTemplate;

VOID *
PrepareHobMemory (
  IN UINTN                    NumberOfMemoryMapEntries,
  IN EFI_MEMORY_DESCRIPTOR    *EfiMemoryDescriptor
  )
/*++
Description:
  Update the Hob filling MemoryFreeUnder1MB, MemoryAbove1MB, MemoryAbove4GB

Arguments:
  NumberOfMemoryMapEntries - Count of Memory Descriptors
  EfiMemoryDescriptor      - Point to the buffer containing NumberOfMemoryMapEntries Memory Descriptors

Return:
  VOID * : The end address of MemoryAbove1MB (or the top free memory under 4GB)
--*/
{
  UINTN                Index;

  //
  // Prepare Low Memory
  // 0x18 pages is 72 KB.
  //
  gHob->MemoryFreeUnder1MB.ResourceLength = EFI_MEMORY_BELOW_1MB_END - EFI_MEMORY_BELOW_1MB_START;
  gHob->MemoryFreeUnder1MB.PhysicalStart  = EFI_MEMORY_BELOW_1MB_START;

  //
  // Prepare High Memory
  // Assume Memory Map is ordered from low to high
  //
  gHob->MemoryAbove1MB.PhysicalStart   = 0;
  gHob->MemoryAbove1MB.ResourceLength  = 0;
  gHob->MemoryAbove4GB.PhysicalStart   = 0;
  gHob->MemoryAbove4GB.ResourceLength  = 0;

  for (Index = 0; Index < NumberOfMemoryMapEntries; Index++) {
    //
    // Skip regions below 1MB
    //
    if (EfiMemoryDescriptor[Index].PhysicalStart < 0x100000) {
      continue;
    }
    //
    // Process regions above 1MB
    //
    if (EfiMemoryDescriptor[Index].PhysicalStart >= 0x100000) {
      if (EfiMemoryDescriptor[Index].Type == EfiConventionalMemory) {
        if (gHob->MemoryAbove1MB.PhysicalStart == 0) {
          gHob->MemoryAbove1MB.PhysicalStart = EfiMemoryDescriptor[Index].PhysicalStart;
          gHob->MemoryAbove1MB.ResourceLength = LShiftU64 (EfiMemoryDescriptor[Index].NumberOfPages, EFI_PAGE_SHIFT);
        } else if (gHob->MemoryAbove1MB.PhysicalStart + gHob->MemoryAbove1MB.ResourceLength == EfiMemoryDescriptor[Index].PhysicalStart) {
          gHob->MemoryAbove1MB.ResourceLength += LShiftU64 (EfiMemoryDescriptor[Index].NumberOfPages, EFI_PAGE_SHIFT);
        }
      }
      if ((EfiMemoryDescriptor[Index].Type == EfiReservedMemoryType) ||
          (EfiMemoryDescriptor[Index].Type >= EfiACPIReclaimMemory) ) {
        continue;
      }
      if ((EfiMemoryDescriptor[Index].Type == EfiRuntimeServicesCode) ||
          (EfiMemoryDescriptor[Index].Type == EfiRuntimeServicesData)) {
        break;
      }
    }
    //
    // Process region above 4GB
    //
    if (EfiMemoryDescriptor[Index].PhysicalStart >= 0x100000000LL) {
      if (EfiMemoryDescriptor[Index].Type == EfiConventionalMemory) {
        if (gHob->MemoryAbove4GB.PhysicalStart == 0) {
          gHob->MemoryAbove4GB.PhysicalStart  = EfiMemoryDescriptor[Index].PhysicalStart;
          gHob->MemoryAbove4GB.ResourceLength = LShiftU64 (EfiMemoryDescriptor[Index].NumberOfPages, EFI_PAGE_SHIFT);
        }
        if (gHob->MemoryAbove4GB.PhysicalStart + gHob->MemoryAbove4GB.ResourceLength == 
            EfiMemoryDescriptor[Index].PhysicalStart) {
          gHob->MemoryAbove4GB.ResourceLength += LShiftU64 (EfiMemoryDescriptor[Index].NumberOfPages, EFI_PAGE_SHIFT);
        }
      }
    }
  }

  if (gHob->MemoryAbove4GB.ResourceLength == 0) {
    //
    // If there is no memory above 4GB then change the resource descriptor HOB
    //  into another type. I'm doing this as it's unclear if a resource
    //  descriptor HOB of length zero is valid. Spec does not say it's illegal,
    //  but code in EDK does not seem to handle this case.
    //
    gHob->MemoryAbove4GB.Header.HobType = EFI_HOB_TYPE_UNUSED;
  }

  return (VOID *)(UINTN)(gHob->MemoryAbove1MB.PhysicalStart + gHob->MemoryAbove1MB.ResourceLength);
}

VOID *
PrepareHobStack (
  IN VOID *StackTop
  )
{
  gHob->Stack.AllocDescriptor.MemoryLength      = EFI_MEMORY_STACK_PAGE_NUM * EFI_PAGE_SIZE;
  gHob->Stack.AllocDescriptor.MemoryBaseAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)StackTop - gHob->Stack.AllocDescriptor.MemoryLength;

  return (VOID *)(UINTN)gHob->Stack.AllocDescriptor.MemoryBaseAddress;
}

VOID *
PrepareHobMemoryDescriptor (
  VOID                          *MemoryDescriptorTop,
  UINTN                         MemDescCount,
  EFI_MEMORY_DESCRIPTOR         *MemDesc
  )
{
  gHob->MemoryDescriptor.MemDescCount = MemDescCount;
  gHob->MemoryDescriptor.MemDesc      = (EFI_MEMORY_DESCRIPTOR *)((UINTN)MemoryDescriptorTop - MemDescCount * sizeof(EFI_MEMORY_DESCRIPTOR));
  //
  // Make MemoryDescriptor.MemDesc page aligned
  //
  gHob->MemoryDescriptor.MemDesc      = (EFI_MEMORY_DESCRIPTOR *)((UINTN) gHob->MemoryDescriptor.MemDesc & ~EFI_PAGE_MASK);

  CopyMem (gHob->MemoryDescriptor.MemDesc, MemDesc, MemDescCount * sizeof(EFI_MEMORY_DESCRIPTOR));

  return gHob->MemoryDescriptor.MemDesc;
}

VOID
PrepareHobBfv (
  VOID  *Bfv,
  UINTN BfvLength
  )
{
  //UINTN BfvLengthPageSize;

  //
  // Calculate BFV location at top of the memory region.
  // This is like a RAM Disk. Align to page boundry.
  //
  //BfvLengthPageSize = EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (BfvLength));
 
  gHob->Bfv.BaseAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)Bfv;
  gHob->Bfv.Length = BfvLength;

  //
  // Resource descriptor for the FV
  //
  gHob->BfvResource.PhysicalStart = gHob->Bfv.BaseAddress;
  gHob->BfvResource.ResourceLength = gHob->Bfv.Length;
}

VOID
PrepareHobDxeCore (
  VOID                  *DxeCoreEntryPoint,
  EFI_PHYSICAL_ADDRESS  DxeCoreImageBase,
  UINT64                DxeCoreLength
  )
{
  gHob->DxeCore.MemoryAllocationHeader.MemoryBaseAddress = DxeCoreImageBase;
  gHob->DxeCore.MemoryAllocationHeader.MemoryLength = DxeCoreLength;
  gHob->DxeCore.EntryPoint = (EFI_PHYSICAL_ADDRESS)(UINTN)DxeCoreEntryPoint;


  // Remove this to prevent a crash caused by freeing of dxecore.
  // The memory containing gEventQueue was getting freed and
  // reused (for disk DMA), leading to an assert. The problem
  // can be demonstrated using the 6GB simnow shiner model.
  //gHob->MemoryDxeCore.PhysicalStart   = DxeCoreImageBase;
  //gHob->MemoryDxeCore.ResourceLength  = DxeCoreLength;
}

VOID *
PrepareHobNvStorage (
  VOID *NvStorageTop
  )
/*
  Initialize Block-Aligned Firmware Block.

  Variable:
           +-------------------+
           |     FV_Header     |
           +-------------------+
           |                   |
           |VAR_STORAGE(0x4000)|
           |                   |
           +-------------------+
  FTW:
           +-------------------+
           |     FV_Header     |
           +-------------------+
           |                   |
           |   Working(0x2000) |
           |                   |
           +-------------------+
           |                   |
           |   Spare(0x10000)  |
           |                   |
           +-------------------+
*/
{
  EFI_PHYSICAL_ADDRESS StorageFvbBase;
  EFI_PHYSICAL_ADDRESS FtwFvbBase;

  StorageFvbBase = (EFI_PHYSICAL_ADDRESS)(((UINTN)NvStorageTop - NV_STORAGE_FVB_SIZE - NV_FTW_FVB_SIZE) & ~EFI_PAGE_MASK);
  //
  // Create the FVB holding NV Storage in memory
  //
  gHob->NvStorageFvResource.PhysicalStart =
    gHob->NvStorageFvb.FvbInfo.Entries[0].Base = StorageFvbBase;
  //
  // Create the NV Storage Hob
  //
  gHob->NvStorage.FvbInfo.Entries[0].Base = StorageFvbBase + EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH;

  //
  // Create the FVB holding FTW spaces
  //
  FtwFvbBase = (EFI_PHYSICAL_ADDRESS)((UINTN) StorageFvbBase + NV_STORAGE_FVB_SIZE);
  gHob->NvFtwFvResource.PhysicalStart =
    gHob->NvFtwFvb.FvbInfo.Entries[0].Base = FtwFvbBase;

  gHob->NvFtwFvResource.PhysicalStart = FAKE_NVRAM_BASE;
  gHob->NvFtwFvResource.ResourceLength = FAKE_NVRAM_SIZE;
  gHob->NvFtwFvResource.ResourceType = EFI_RESOURCE_MEMORY_MAPPED_IO;
  gHob->NvFtwFvResource.ResourceAttribute = 
    EFI_RESOURCE_ATTRIBUTE_PRESENT      |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED  |
    EFI_RESOURCE_ATTRIBUTE_TESTED       |
    EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE  |
    0;

  //
  // Put FTW Working in front
  //
  gHob->NvFtwWorking.FvbInfo.Entries[0].Base = FtwFvbBase + EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH;

  //
  // Put FTW Spare area after FTW Working area
  //
  gHob->NvFtwSpare.FvbInfo.Entries[0].Base = 
    (EFI_PHYSICAL_ADDRESS)((UINTN) FtwFvbBase + EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH + NV_FTW_WORKING_SIZE);
  
  return (VOID *)(UINTN)StorageFvbBase;
}

//  Maximum UEFI memory needed for page tables and other structures was
//  hard-coded to 80 megs. Replace this with a proper runtime calculation.

VOID
PrepareHobPhit (
  VOID *MemoryTop,
  VOID *FreeMemoryTop,
  UINTN consumedMemory
  )
{
  EFI_PHYSICAL_ADDRESS below4GbTop = (UINT64) MemoryTop;          // top of usable DRAM below 4GB
  EFI_PHYSICAL_ADDRESS below4GbFreeTop = (UINT64) FreeMemoryTop;  // after page tables and other UEFI items allocated

  gHob->Phit.EfiMemoryTop        = below4GbFreeTop;
  gHob->Phit.EfiFreeMemoryTop    = below4GbFreeTop;
  gHob->Phit.EfiMemoryBottom     = below4GbTop - consumedMemory;
  gHob->Phit.EfiFreeMemoryBottom = below4GbTop - consumedMemory + sizeof(HOB_TEMPLATE);

  CopyMem ((VOID *)(UINTN)gHob->Phit.EfiMemoryBottom, gHob, sizeof(HOB_TEMPLATE));
  gHob = (HOB_TEMPLATE *)(UINTN)gHob->Phit.EfiMemoryBottom;

  gHob->Phit.EfiEndOfHobList = (EFI_PHYSICAL_ADDRESS)(UINTN)&gHob->EndOfHobList;
}

static int highestSetBit64 (UINT64 value)
    {
    int bit;
    for (bit = 63; bit >= 0; bit--)
        if (value >> bit) return bit;
    return 0;
    }


VOID
PrepareHobCpu (
  VOID
  )
{
  // Build page tables for the amount of installed DRAM memory
  UINT64 topOfDram;
  
  topOfDram = gHob->MemoryAbove4GB.PhysicalStart + gHob->MemoryAbove4GB.ResourceLength;

  // always build at least 4GB of page tables, even if less DRAM is installed
  if (topOfDram < 0x100000000) topOfDram = 0x100000000;

  // round up to the next GB, just in case E820 does not report the true top
  topOfDram += 0x3FFFFFFF;

  gHob->Cpu.SizeOfMemorySpace = (UINT8) highestSetBit64 (topOfDram) + 1; 
}

VOID
CompleteHobGeneration (
  VOID
  )
{
  gHob->MemoryAllocation.AllocDescriptor.MemoryBaseAddress  = gHob->Phit.EfiFreeMemoryTop;
  //
  // Reserve all the memory under Stack above FreeMemoryTop as allocated
  //
  gHob->MemoryAllocation.AllocDescriptor.MemoryLength       = gHob->Stack.AllocDescriptor.MemoryBaseAddress - gHob->Phit.EfiFreeMemoryTop;

  //
  // adjust Above1MB ResourceLength
  //
  if (gHob->MemoryAbove1MB.PhysicalStart + gHob->MemoryAbove1MB.ResourceLength > gHob->Phit.EfiMemoryTop) {
    gHob->MemoryAbove1MB.ResourceLength = gHob->Phit.EfiMemoryTop - gHob->MemoryAbove1MB.PhysicalStart;
  }
}

