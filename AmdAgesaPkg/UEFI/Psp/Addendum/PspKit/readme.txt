+---------------------------------------------------------------------------------------------------+
|                                                                                                   |
|                                       PSP     KIT                                                 |
|                                                                                                   |
|   Tool Kit for build a BIOS image following HW Validated Boot requirement          |
+---------------------------------------------------------------------------------------------------+

+---------------------------------------------------------------------------------------------------+
|Contents:                                                                                          |
+---------------------------------------------------------------------------------------------------+
|-[PspDirectoryTool]        Tool for convert BIOS image to PSP format                       
|                                             
|-BuildPspImage.bat         Sample batch file for Create RTM signature & Embed the PSP images 
|
|-PSPDirectory.txt          Configure file describe PSP directory table required information:
                            including BIOS image info and each entry's file name,Type,
                            Size.Will be used when PspDirectoryTool creating PSP Directory table &
                            the final BIOS image
