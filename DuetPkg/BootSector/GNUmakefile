## @file
#  Just use jwasm to compile boot sector image.
#
#  Copyright (c) 2010 - 2012, Intel Corporation. All rights reserved.<BR>
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution. The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#
##

PP_DEFS += -DIMAGE_BUFFER_LOW=$(IMAGE_BUFFER_LOW)
PP_DEFS += -DIMAGE_BUFFER_HIGH=$(IMAGE_BUFFER_HIGH)
PP_DEFS += -DX64_PAGE_TABLE_BASE=$(X64_PAGE_TABLE_BASE)
PP_DEFS += -DSTART64CODESIZE=$(START64CODESIZE)
PP_DEFS += -DEFI64CODESTART=$(EFI64CODESTART)
PP_DEFS += -DEFI64CODESIZE=$(EFI64CODESIZE)
PP_DEFS += -DX64_STACK_TOP=$(X64_STACK_TOP)
ASM=$(EDK_TOOLS_PATH)/Bin/linux/jwasm -bin $(PP_DEFS)
DLINK=ld

ASSEMBLY_CODE_FILE_LIST = $(MODULE_DIR)/bootsect.asm \
                          $(MODULE_DIR)/bs16.asm \
                          $(MODULE_DIR)/bs32.asm \
                          $(MODULE_DIR)/efi32.asm \
                          $(MODULE_DIR)/efi64.asm \
                          $(MODULE_DIR)/Gpt.asm \
                          $(MODULE_DIR)/Mbr.asm \
                          $(MODULE_DIR)/start.asm \
                          $(MODULE_DIR)/start16.asm \
                          $(MODULE_DIR)/start32.asm \
                          $(MODULE_DIR)/start64.asm \
                          $(MODULE_DIR)/st16_64.asm \
                          $(MODULE_DIR)/st32_64.asm

TARGET_FILES = $(OUTPUT_DIR)/bootsect.com \
               $(OUTPUT_DIR)/bs16.com \
               $(OUTPUT_DIR)/bs32.com \
               $(OUTPUT_DIR)/Gpt.com \
               $(OUTPUT_DIR)/Mbr.com \
               $(OUTPUT_DIR)/start.com \
               $(OUTPUT_DIR)/start16.com \
               $(OUTPUT_DIR)/start32.com \
               $(OUTPUT_DIR)/efi32.com2 \
               $(OUTPUT_DIR)/start64.com \
               $(OUTPUT_DIR)/st16_64.com \
               $(OUTPUT_DIR)/st32_64.com \
               $(OUTPUT_DIR)/efi64.com2

.PHONY : all
all: $(TARGET_FILES)

$(OUTPUT_DIR)/bootsect.com: $(MODULE_DIR)/bootsect.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/bootsect.com $(MODULE_DIR)/bootsect.asm

$(OUTPUT_DIR)/bs16.com: $(MODULE_DIR)/bs16.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/bs16.com $(MODULE_DIR)/bs16.asm

$(OUTPUT_DIR)/bs32.com: $(MODULE_DIR)/bs32.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/bs32.com $(MODULE_DIR)/bs32.asm

$(OUTPUT_DIR)/Gpt.com: $(MODULE_DIR)/Gpt.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/Gpt.com $(MODULE_DIR)/Gpt.asm

$(OUTPUT_DIR)/Mbr.com: $(MODULE_DIR)/Mbr.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/Mbr.com $(MODULE_DIR)/Mbr.asm

$(OUTPUT_DIR)/start.com: $(MODULE_DIR)/start.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/start.com $(MODULE_DIR)/start.asm

$(OUTPUT_DIR)/start16.com: $(MODULE_DIR)/start16.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/start16.com $(MODULE_DIR)/start16.asm

$(OUTPUT_DIR)/start32.com: $(MODULE_DIR)/start32.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/start32.com $(MODULE_DIR)/start32.asm

$(OUTPUT_DIR)/start64.com: $(MODULE_DIR)/start64.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/start64.com $(MODULE_DIR)/start64.asm

$(OUTPUT_DIR)/st16_64.com: $(MODULE_DIR)/st16_64.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/st16_64.com $(MODULE_DIR)/st16_64.asm

$(OUTPUT_DIR)/st32_64.com: $(MODULE_DIR)/st32_64.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/st32_64.com $(MODULE_DIR)/st32_64.asm

$(OUTPUT_DIR)/efi32.com2: $(MODULE_DIR)/efi32.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/efi32.com2 $(MODULE_DIR)/efi32.asm

$(OUTPUT_DIR)/efi64.com2: $(MODULE_DIR)/efi64.asm
	$(ASM) -c -Fo=$(OUTPUT_DIR)/efi64.com2 $(MODULE_DIR)/efi64.asm

clean:
ifneq ($(OUTPUT_DIR), )
	rm -r $(OUTPUT_DIR)
endif
ifneq ($(DEBUG_DIR), )
	rm -r $(DEBUG_DIR)
endif


