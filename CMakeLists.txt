cmake_minimum_required(VERSION 3.16)
project(7zip-zstd-wine
  VERSION 1.0.0
  DESCRIPTION "7-Zip Zstandard with Wine/Linux support"
  LANGUAGES C CXX RC
)

# ============================================================================
# Options
# ============================================================================
option(Z7_WINE_LINUX "Enable Wine/Linux path conversion integration" ON)
option(DISABLE_RAR "Disable RAR format support" OFF)
option(ENABLE_ASM "Enable assembly optimizations" OFF)

# ============================================================================
# Compile definitions
# ============================================================================
add_library(z7_defs INTERFACE)
target_compile_definitions(z7_defs INTERFACE
  $<$<CONFIG:Debug>:DEBUG>
  $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
  _WIN32
  _WIN64
  WIN32
  WIN64
  _REENTRANT
  _FILE_OFFSET_BITS=64
  _LARGEFILE_SOURCE
  Z7_LANG
  Z7_DEVICE_FILE
  Z7_LONG_PATH
  UNICODE
  _UNICODE
  _WIN32_WINNT=0x0600
  WINVER=0x0600
  Z7_EXTERNAL_CODECS
)

if(Z7_WINE_LINUX)
  target_compile_definitions(z7_defs INTERFACE Z7_WINE_LINUX)
endif()

if(DISABLE_RAR)
  target_compile_definitions(z7_defs INTERFACE DISABLE_RAR DISABLE_RAR_COMPRESS)
endif()

# ============================================================================
# Compile options
# ============================================================================
add_library(z7_opts INTERFACE)
target_compile_options(z7_opts INTERFACE
  -O2
  -Wall
  -Wextra
  -Wno-unknown-pragmas
  -Wno-cast-function-type
  -Wno-maybe-uninitialized
  -Wno-unused-parameter
  -fPIC
  $<$<CONFIG:Debug>:-g>
  $<$<NOT:$<CONFIG:Debug>>:-s>
)

# ============================================================================
# Include directories
# ============================================================================
add_library(z7_includes INTERFACE)
target_include_directories(z7_includes INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
  ${CMAKE_SOURCE_DIR}/C
  ${CMAKE_SOURCE_DIR}/C/brotli
  ${CMAKE_SOURCE_DIR}/C/zstd
  ${CMAKE_SOURCE_DIR}/C/zstdmt
  ${CMAKE_SOURCE_DIR}/C/lz4
  ${CMAKE_SOURCE_DIR}/C/lz5
  ${CMAKE_SOURCE_DIR}/C/lizard
  ${CMAKE_SOURCE_DIR}/C/fast-lzma2
  ${CMAKE_SOURCE_DIR}/C/hashes
  ${CMAKE_SOURCE_DIR}/CPP
  ${CMAKE_SOURCE_DIR}/CPP/Common
  ${CMAKE_SOURCE_DIR}/CPP/Windows
  ${CMAKE_SOURCE_DIR}/CPP/7zip/Common
  ${CMAKE_SOURCE_DIR}/CPP/7zip/Archive
  ${CMAKE_SOURCE_DIR}/CPP/7zip/Archive/Common
  ${CMAKE_SOURCE_DIR}/CPP/7zip/Compress
  ${CMAKE_SOURCE_DIR}/CPP/7zip/Crypto
  ${CMAKE_SOURCE_DIR}/CPP/7zip/UI/Common
  ${CMAKE_SOURCE_DIR}/CPP/7zip/UI/FileManager
  ${CMAKE_SOURCE_DIR}/CPP/7zip/UI/GUI
  ${CMAKE_SOURCE_DIR}/CPP/7zip/UI/Agent
  ${CMAKE_SOURCE_DIR}/CPP/7zip/UI/Explorer
  ${CMAKE_SOURCE_DIR}/DarkMode/lib
)

# ============================================================================
# Base compile interface
# ============================================================================
add_library(z7_base INTERFACE)
target_link_libraries(z7_base INTERFACE z7_defs z7_opts z7_includes)

# ============================================================================
# C Library sources (from C/)
# ============================================================================
set(CORE_C_SOURCES
  C/7zBuf2.c
  C/7zCrc.c
  C/7zCrcOpt.c
  C/7zStream.c
  C/Aes.c
  C/AesOpt.c
  C/Alloc.c
  C/Bcj2.c
  C/Bcj2Enc.c
  C/Blake2s.c
  C/Bra.c
  C/Bra86.c
  C/BraIA64.c
  C/BwtSort.c
  C/CpuArch.c
  C/Delta.c
  C/HuffEnc.c
  C/LzFind.c
  C/Lzma2Dec.c
  C/Lzma2DecMt.c
  C/Lzma2Enc.c
  C/LzmaDec.c
  C/LzmaEnc.c
  C/Md5.c
  C/MtCoder.c
  C/MtDec.c
  C/Ppmd7.c
  C/Ppmd7Dec.c
  C/Ppmd7aDec.c
  C/Ppmd7Enc.c
  C/Ppmd8.c
  C/Ppmd8Dec.c
  C/Ppmd8Enc.c
  C/Sha1.c
  C/Sha1Opt.c
  C/Sha256.c
  C/Sha256Opt.c
  C/Sha3.c
  C/Sha512.c
  C/Sha512Opt.c
  C/Sort.c
  C/SwapBytes.c
  C/Xz.c
  C/XzDec.c
  C/XzEnc.c
  C/XzIn.c
  C/XzCrc64.c
  C/XzCrc64Opt.c
  C/DllSecur.c
  C/LzFindMt.c
  C/LzFindOpt.c
  C/Threads.c
)

file(GLOB BROTLI_C_SOURCES C/brotli/*.c)
file(GLOB ZSTD_C_SOURCES C/zstd/*.c)
file(GLOB ZSTDMT_C_SOURCES C/zstdmt/*.c)
file(GLOB LZ4_C_SOURCES C/lz4/*.c)
file(GLOB LZ5_C_SOURCES C/lz5/*.c)
file(GLOB LIZARD_C_SOURCES C/lizard/*.c)
file(GLOB FASTLZMA2_C_SOURCES C/fast-lzma2/*.c)
file(GLOB HASHES_C_SOURCES C/hashes/*.c)

# ============================================================================
# CPP Common sources
# ============================================================================
set(COMMON_SOURCES
  CPP/Common/CRC.cpp
  CPP/Common/CrcReg.cpp
  CPP/Common/DynLimBuf.cpp
  CPP/Common/IntToString.cpp
  CPP/Common/Lang.cpp
  CPP/Common/ListFileUtils.cpp
  CPP/Common/MyMap.cpp
  CPP/Common/MyString.cpp
  CPP/Common/MyVector.cpp
  CPP/Common/MyWindows.cpp
  CPP/Common/MyXml.cpp
  CPP/Common/NewHandler.cpp
  CPP/Common/Random.cpp
  CPP/Common/StringConvert.cpp
  CPP/Common/StringToInt.cpp
  CPP/Common/UTFConvert.cpp
  CPP/Common/Wildcard.cpp
  CPP/Common/XzCrc64Init.cpp
  CPP/Common/XzCrc64Reg.cpp
  CPP/Common/Md5Reg.cpp
  CPP/Common/Sha1Prepare.cpp
  CPP/Common/Sha1Reg.cpp
  CPP/Common/Sha256Prepare.cpp
  CPP/Common/Sha256Reg.cpp
  CPP/Common/Sha3Reg.cpp
  CPP/Common/Sha512Prepare.cpp
  CPP/Common/Sha512Reg.cpp
  CPP/Common/LzFindPrepare.cpp
  CPP/Common/CommandLineParser.cpp
)

# ============================================================================
# CPP Windows sources
# ============================================================================
set(WINDOWS_SOURCES
  CPP/Windows/Clipboard.cpp
  CPP/Windows/CommonDialog.cpp
  CPP/Windows/DLL.cpp
  CPP/Windows/ErrorMsg.cpp
  CPP/Windows/FileDir.cpp
  CPP/Windows/FileFind.cpp
  CPP/Windows/FileIO.cpp
  CPP/Windows/FileLink.cpp
  CPP/Windows/FileName.cpp
  CPP/Windows/FileSystem.cpp
  CPP/Windows/MemoryGlobal.cpp
  CPP/Windows/MemoryLock.cpp
  CPP/Windows/Menu.cpp
  CPP/Windows/Net.cpp
  CPP/Windows/ProcessUtils.cpp
  CPP/Windows/PropVariant.cpp
  CPP/Windows/PropVariantConv.cpp
  CPP/Windows/PropVariantUtils.cpp
  CPP/Windows/Registry.cpp
  CPP/Windows/ResourceString.cpp
  CPP/Windows/SecurityUtils.cpp
  CPP/Windows/Shell.cpp
  CPP/Windows/System.cpp
  CPP/Windows/SystemInfo.cpp
  CPP/Windows/TimeUtils.cpp
  CPP/Windows/Window.cpp
  # Windows/Control
  CPP/Windows/Control/ComboBox.cpp
  CPP/Windows/Control/Dialog.cpp
  CPP/Windows/Control/ListView.cpp
  CPP/Windows/Control/PropertyPage.cpp
  CPP/Windows/Control/Window2.cpp
)

# ============================================================================
# 7-Zip Common sources (CPP/7zip/Common/)
# ============================================================================
set(Z7_COMMON_SOURCES
  CPP/7zip/Common/CreateCoder.cpp
  CPP/7zip/Common/CWrappers.cpp
  CPP/7zip/Common/FilePathAutoRename.cpp
  CPP/7zip/Common/FileStreams.cpp
  CPP/7zip/Common/FilterCoder.cpp
  CPP/7zip/Common/InBuffer.cpp
  CPP/7zip/Common/InOutTempBuffer.cpp
  CPP/7zip/Common/LimitedStreams.cpp
  CPP/7zip/Common/LockedStream.cpp
  CPP/7zip/Common/MethodId.cpp
  CPP/7zip/Common/MethodProps.cpp
  CPP/7zip/Common/MultiOutStream.cpp
  CPP/7zip/Common/OffsetStream.cpp
  CPP/7zip/Common/OutBuffer.cpp
  CPP/7zip/Common/ProgressUtils.cpp
  CPP/7zip/Common/PropId.cpp
  CPP/7zip/Common/StreamObjects.cpp
  CPP/7zip/Common/StreamUtils.cpp
  CPP/7zip/Common/UniqBlocks.cpp
)

# ============================================================================
# Archive handler sources (CPP/7zip/Archive/)
# ============================================================================
set(ARCHIVE_SOURCES
  CPP/7zip/Archive/ApfsHandler.cpp
  CPP/7zip/Archive/ApmHandler.cpp
  CPP/7zip/Archive/ArHandler.cpp
  CPP/7zip/Archive/ArjHandler.cpp
  CPP/7zip/Archive/Base64Handler.cpp
  CPP/7zip/Archive/BrotliHandler.cpp
  CPP/7zip/Archive/Bz2Handler.cpp
  CPP/7zip/Archive/ComHandler.cpp
  CPP/7zip/Archive/CpioHandler.cpp
  CPP/7zip/Archive/CramfsHandler.cpp
  CPP/7zip/Archive/DeflateProps.cpp
  CPP/7zip/Archive/DmgHandler.cpp
  CPP/7zip/Archive/ElfHandler.cpp
  CPP/7zip/Archive/ExtHandler.cpp
  CPP/7zip/Archive/FatHandler.cpp
  CPP/7zip/Archive/FlvHandler.cpp
  CPP/7zip/Archive/GptHandler.cpp
  CPP/7zip/Archive/GzHandler.cpp
  CPP/7zip/Archive/HandlerCont.cpp
  CPP/7zip/Archive/HfsHandler.cpp
  CPP/7zip/Archive/IhexHandler.cpp
  CPP/7zip/Archive/LizardHandler.cpp
  CPP/7zip/Archive/LpHandler.cpp
  CPP/7zip/Archive/Lz4Handler.cpp
  CPP/7zip/Archive/Lz5Handler.cpp
  CPP/7zip/Archive/LzHandler.cpp
  CPP/7zip/Archive/LzhHandler.cpp
  CPP/7zip/Archive/LzmaHandler.cpp
  CPP/7zip/Archive/MachoHandler.cpp
  CPP/7zip/Archive/MbrHandler.cpp
  CPP/7zip/Archive/MslzHandler.cpp
  CPP/7zip/Archive/MubHandler.cpp
  CPP/7zip/Archive/NtfsHandler.cpp
  CPP/7zip/Archive/PeHandler.cpp
  CPP/7zip/Archive/PpmdHandler.cpp
  CPP/7zip/Archive/QcowHandler.cpp
  CPP/7zip/Archive/RpmHandler.cpp
  CPP/7zip/Archive/SparseHandler.cpp
  CPP/7zip/Archive/SplitHandler.cpp
  CPP/7zip/Archive/SquashfsHandler.cpp
  CPP/7zip/Archive/SwfHandler.cpp
  CPP/7zip/Archive/UefiHandler.cpp
  CPP/7zip/Archive/VdiHandler.cpp
  CPP/7zip/Archive/VhdHandler.cpp
  CPP/7zip/Archive/VhdxHandler.cpp
  CPP/7zip/Archive/VmdkHandler.cpp
  CPP/7zip/Archive/XarHandler.cpp
  CPP/7zip/Archive/XzHandler.cpp
  CPP/7zip/Archive/ZHandler.cpp
  CPP/7zip/Archive/ZstdHandler.cpp
  CPP/7zip/Archive/7z/7zCompressionMode.cpp
  CPP/7zip/Archive/7z/7zDecode.cpp
  CPP/7zip/Archive/7z/7zEncode.cpp
  CPP/7zip/Archive/7z/7zExtract.cpp
  CPP/7zip/Archive/7z/7zFolderInStream.cpp
  CPP/7zip/Archive/7z/7zHandler.cpp
  CPP/7zip/Archive/7z/7zHandlerOut.cpp
  CPP/7zip/Archive/7z/7zHeader.cpp
  CPP/7zip/Archive/7z/7zIn.cpp
  CPP/7zip/Archive/7z/7zOut.cpp
  CPP/7zip/Archive/7z/7zProperties.cpp
  CPP/7zip/Archive/7z/7zRegister.cpp
  CPP/7zip/Archive/7z/7zSpecStream.cpp
  CPP/7zip/Archive/7z/7zUpdate.cpp
  CPP/7zip/Archive/Cab/CabBlockInStream.cpp
  CPP/7zip/Archive/Cab/CabHandler.cpp
  CPP/7zip/Archive/Cab/CabHeader.cpp
  CPP/7zip/Archive/Cab/CabIn.cpp
  CPP/7zip/Archive/Cab/CabRegister.cpp
  CPP/7zip/Archive/Chm/ChmHandler.cpp
  CPP/7zip/Archive/Chm/ChmIn.cpp
  CPP/7zip/Archive/Iso/IsoHandler.cpp
  CPP/7zip/Archive/Iso/IsoHeader.cpp
  CPP/7zip/Archive/Iso/IsoIn.cpp
  CPP/7zip/Archive/Iso/IsoRegister.cpp
  CPP/7zip/Archive/Nsis/NsisDecode.cpp
  CPP/7zip/Archive/Nsis/NsisHandler.cpp
  CPP/7zip/Archive/Nsis/NsisIn.cpp
  CPP/7zip/Archive/Nsis/NsisRegister.cpp
  CPP/7zip/Archive/Tar/TarHandler.cpp
  CPP/7zip/Archive/Tar/TarHandlerOut.cpp
  CPP/7zip/Archive/Tar/TarHeader.cpp
  CPP/7zip/Archive/Tar/TarIn.cpp
  CPP/7zip/Archive/Tar/TarOut.cpp
  CPP/7zip/Archive/Tar/TarRegister.cpp
  CPP/7zip/Archive/Tar/TarUpdate.cpp
  CPP/7zip/Archive/Udf/UdfHandler.cpp
  CPP/7zip/Archive/Udf/UdfIn.cpp
  CPP/7zip/Archive/Wim/WimHandler.cpp
  CPP/7zip/Archive/Wim/WimHandlerOut.cpp
  CPP/7zip/Archive/Wim/WimIn.cpp
  CPP/7zip/Archive/Wim/WimRegister.cpp
  CPP/7zip/Archive/Zip/ZipAddCommon.cpp
  CPP/7zip/Archive/Zip/ZipHandler.cpp
  CPP/7zip/Archive/Zip/ZipHandlerOut.cpp
  CPP/7zip/Archive/Zip/ZipIn.cpp
  CPP/7zip/Archive/Zip/ZipItem.cpp
  CPP/7zip/Archive/Zip/ZipOut.cpp
  CPP/7zip/Archive/Zip/ZipRegister.cpp
  CPP/7zip/Archive/Zip/ZipUpdate.cpp
)

if(NOT DISABLE_RAR)
  list(APPEND ARCHIVE_SOURCES
    CPP/7zip/Archive/Rar/RarHandler.cpp
    CPP/7zip/Archive/Rar/Rar5Handler.cpp
  )
endif()

# ============================================================================
# Archive Common sources
# ============================================================================
set(ARCHIVE_COMMON_SOURCES
  CPP/7zip/Archive/Common/CoderMixer2.cpp
  CPP/7zip/Archive/Common/DummyOutStream.cpp
  CPP/7zip/Archive/Common/FindSignature.cpp
  CPP/7zip/Archive/Common/HandlerOut.cpp
  CPP/7zip/Archive/Common/InStreamWithCRC.cpp
  CPP/7zip/Archive/Common/ItemNameUtils.cpp
  CPP/7zip/Archive/Common/MultiStream.cpp
  CPP/7zip/Archive/Common/OutStreamWithCRC.cpp
  CPP/7zip/Archive/Common/OutStreamWithSha1.cpp
  CPP/7zip/Archive/Common/ParseProperties.cpp
)

# ============================================================================
# Compression codec sources (CPP/7zip/Compress/)
# ============================================================================
set(COMPRESS_SOURCES
  CPP/7zip/Compress/Bcj2Coder.cpp
  CPP/7zip/Compress/Bcj2Register.cpp
  CPP/7zip/Compress/BcjCoder.cpp
  CPP/7zip/Compress/BcjRegister.cpp
  CPP/7zip/Compress/BitlDecoder.cpp
  CPP/7zip/Compress/BranchMisc.cpp
  CPP/7zip/Compress/BranchRegister.cpp
  CPP/7zip/Compress/ByteSwap.cpp
  CPP/7zip/Compress/BZip2Crc.cpp
  CPP/7zip/Compress/BZip2Decoder.cpp
  CPP/7zip/Compress/BZip2Encoder.cpp
  CPP/7zip/Compress/BZip2Register.cpp
  CPP/7zip/Compress/CopyCoder.cpp
  CPP/7zip/Compress/CopyRegister.cpp
  CPP/7zip/Compress/Deflate64Register.cpp
  CPP/7zip/Compress/DeflateDecoder.cpp
  CPP/7zip/Compress/DeflateEncoder.cpp
  CPP/7zip/Compress/DeflateRegister.cpp
  CPP/7zip/Compress/DeltaFilter.cpp
  CPP/7zip/Compress/ImplodeDecoder.cpp
  CPP/7zip/Compress/LzfseDecoder.cpp
  CPP/7zip/Compress/LzhDecoder.cpp
  CPP/7zip/Compress/Lzma2Decoder.cpp
  CPP/7zip/Compress/Lzma2Encoder.cpp
  CPP/7zip/Compress/Lzma2Register.cpp
  CPP/7zip/Compress/LzmaDecoder.cpp
  CPP/7zip/Compress/LzmaEncoder.cpp
  CPP/7zip/Compress/LzmaRegister.cpp
  CPP/7zip/Compress/LzmsDecoder.cpp
  CPP/7zip/Compress/LzOutWindow.cpp
  CPP/7zip/Compress/LzxDecoder.cpp
  CPP/7zip/Compress/PpmdDecoder.cpp
  CPP/7zip/Compress/PpmdEncoder.cpp
  CPP/7zip/Compress/PpmdRegister.cpp
  CPP/7zip/Compress/PpmdZip.cpp
  CPP/7zip/Compress/QuantumDecoder.cpp
  CPP/7zip/Compress/ShrinkDecoder.cpp
  CPP/7zip/Compress/XpressDecoder.cpp
  CPP/7zip/Compress/XzDecoder.cpp
  CPP/7zip/Compress/XzEncoder.cpp
  CPP/7zip/Compress/ZlibDecoder.cpp
  CPP/7zip/Compress/ZlibEncoder.cpp
  CPP/7zip/Compress/ZDecoder.cpp
  CPP/7zip/Compress/ZstdDecoder.cpp
  # ZS additions
  CPP/7zip/Compress/BrotliDecoder.cpp
  CPP/7zip/Compress/BrotliEncoder.cpp
  CPP/7zip/Compress/BrotliRegister.cpp
  CPP/7zip/Compress/Lz4Decoder.cpp
  CPP/7zip/Compress/Lz4Encoder.cpp
  CPP/7zip/Compress/Lz4Register.cpp
  CPP/7zip/Compress/LizardDecoder.cpp
  CPP/7zip/Compress/LizardEncoder.cpp
  CPP/7zip/Compress/LizardRegister.cpp
  CPP/7zip/Compress/Lz5Decoder.cpp
  CPP/7zip/Compress/Lz5Encoder.cpp
  CPP/7zip/Compress/Lz5Register.cpp
  CPP/7zip/Compress/ZstdEncoder.cpp
  CPP/7zip/Compress/ZstdRegister.cpp
  CPP/7zip/Compress/FastLzma2Register.cpp
)

if(NOT DISABLE_RAR)
  list(APPEND COMPRESS_SOURCES
    CPP/7zip/Compress/Rar1Decoder.cpp
    CPP/7zip/Compress/Rar2Decoder.cpp
    CPP/7zip/Compress/Rar3Decoder.cpp
    CPP/7zip/Compress/Rar3Vm.cpp
    CPP/7zip/Compress/Rar5Decoder.cpp
    CPP/7zip/Compress/RarCodecsRegister.cpp
  )
endif()

# ============================================================================
# Crypto sources (CPP/7zip/Crypto/)
# ============================================================================
set(CRYPTO_SOURCES
  CPP/7zip/Crypto/7zAes.cpp
  CPP/7zip/Crypto/7zAesRegister.cpp
  CPP/7zip/Crypto/HmacSha1.cpp
  CPP/7zip/Crypto/HmacSha256.cpp
  CPP/7zip/Crypto/MyAes.cpp
  CPP/7zip/Crypto/MyAesReg.cpp
  CPP/7zip/Crypto/Pbkdf2HmacSha1.cpp
  CPP/7zip/Crypto/RandGen.cpp
  CPP/7zip/Crypto/WzAes.cpp
  CPP/7zip/Crypto/ZipCrypto.cpp
  CPP/7zip/Crypto/ZipStrong.cpp
)

if(NOT DISABLE_RAR)
  list(APPEND CRYPTO_SOURCES
    CPP/7zip/Crypto/Rar20Crypto.cpp
    CPP/7zip/Crypto/Rar5Aes.cpp
    CPP/7zip/Crypto/RarAes.cpp
  )
endif()

# ============================================================================
# Multi-threading C++ sources
# ============================================================================
set(MT_SOURCES
  CPP/7zip/Common/MemBlocks.cpp
  CPP/7zip/Common/OutMemStream.cpp
  CPP/7zip/Common/ProgressMt.cpp
  CPP/7zip/Common/StreamBinder.cpp
  CPP/7zip/Common/VirtThread.cpp
)

# ============================================================================
# UI sources
# ============================================================================
set(UI_COMMON_SOURCES
  CPP/7zip/UI/Common/WineUtils.cpp
  CPP/7zip/UI/Common/ArchiveCommandLine.cpp
  CPP/7zip/UI/Common/ArchiveExtractCallback.cpp
  CPP/7zip/UI/Common/ArchiveName.cpp
  CPP/7zip/UI/Common/ArchiveOpenCallback.cpp
  CPP/7zip/UI/Common/Bench.cpp
  CPP/7zip/UI/Common/CompressCall.cpp
  CPP/7zip/UI/Common/CompressCall2.cpp
  CPP/7zip/UI/Common/DefaultName.cpp
  CPP/7zip/UI/Common/EnumDirItems.cpp
  CPP/7zip/UI/Common/Extract.cpp
  CPP/7zip/UI/Common/ExtractingFilePath.cpp
  CPP/7zip/UI/Common/HashCalc.cpp
  CPP/7zip/UI/Common/LoadCodecs.cpp
  CPP/7zip/UI/Common/OpenArchive.cpp
  CPP/7zip/UI/Common/PropIDUtils.cpp
  CPP/7zip/UI/Common/SetProperties.cpp
  CPP/7zip/UI/Common/SortUtils.cpp
  CPP/7zip/UI/Common/TempFiles.cpp
  CPP/7zip/UI/Common/Update.cpp
  CPP/7zip/UI/Common/UpdateAction.cpp
  CPP/7zip/UI/Common/UpdateCallback.cpp
  CPP/7zip/UI/Common/UpdatePair.cpp
  CPP/7zip/UI/Common/UpdateProduce.cpp
  CPP/7zip/UI/Common/WorkDir.cpp
  CPP/7zip/UI/Common/ZipRegistry.cpp
)

set(FILEMANAGER_SOURCES
  CPP/7zip/UI/FileManager/App.cpp
  CPP/7zip/UI/FileManager/BrowseDialog.cpp
  CPP/7zip/UI/FileManager/BrowseDialog2.cpp
  CPP/7zip/UI/FileManager/ClassDefs.cpp
  CPP/7zip/UI/FileManager/EnumFormatEtc.cpp
  CPP/7zip/UI/FileManager/ExtractCallback.cpp
  CPP/7zip/UI/FileManager/FileFolderPluginOpen.cpp
  CPP/7zip/UI/FileManager/FilePlugins.cpp
  CPP/7zip/UI/FileManager/FM.cpp
  CPP/7zip/UI/FileManager/FoldersPage.cpp
  CPP/7zip/UI/FileManager/FormatUtils.cpp
  CPP/7zip/UI/FileManager/FSFolder.cpp
  CPP/7zip/UI/FileManager/FSFolderCopy.cpp
  CPP/7zip/UI/FileManager/HelpUtils.cpp
  CPP/7zip/UI/FileManager/LangUtils.cpp
  CPP/7zip/UI/FileManager/MemDialog.cpp
  CPP/7zip/UI/FileManager/MenuPage.cpp
  CPP/7zip/UI/FileManager/MyLoadMenu.cpp
  CPP/7zip/UI/FileManager/OpenCallback.cpp
  CPP/7zip/UI/FileManager/OptionsDialog.cpp
  CPP/7zip/UI/FileManager/Panel.cpp
  CPP/7zip/UI/FileManager/PanelCopy.cpp
  CPP/7zip/UI/FileManager/PanelCrc.cpp
  CPP/7zip/UI/FileManager/PanelDrag.cpp
  CPP/7zip/UI/FileManager/PanelFolderChange.cpp
  CPP/7zip/UI/FileManager/PanelItemOpen.cpp
  CPP/7zip/UI/FileManager/PanelItems.cpp
  CPP/7zip/UI/FileManager/PanelKey.cpp
  CPP/7zip/UI/FileManager/PanelListNotify.cpp
  CPP/7zip/UI/FileManager/PanelMenu.cpp
  CPP/7zip/UI/FileManager/PanelOperations.cpp
  CPP/7zip/UI/FileManager/PanelSelect.cpp
  CPP/7zip/UI/FileManager/PanelSort.cpp
  CPP/7zip/UI/FileManager/PanelSplitFile.cpp
  CPP/7zip/UI/FileManager/ProgramLocation.cpp
  CPP/7zip/UI/FileManager/PropertyName.cpp
  CPP/7zip/UI/FileManager/RegistryAssociations.cpp
  CPP/7zip/UI/FileManager/RegistryUtils.cpp
  CPP/7zip/UI/FileManager/RootFolder.cpp
  CPP/7zip/UI/FileManager/SplitUtils.cpp
  CPP/7zip/UI/FileManager/StringUtils.cpp
  CPP/7zip/UI/FileManager/SysIconUtils.cpp
  CPP/7zip/UI/FileManager/TextPairs.cpp
  CPP/7zip/UI/FileManager/UpdateCallback100.cpp
  CPP/7zip/UI/FileManager/ViewSettings.cpp
  CPP/7zip/UI/FileManager/AboutDialog.cpp
  CPP/7zip/UI/FileManager/ComboDialog.cpp
  CPP/7zip/UI/FileManager/CopyDialog.cpp
  CPP/7zip/UI/FileManager/EditDialog.cpp
  CPP/7zip/UI/FileManager/EditPage.cpp
  CPP/7zip/UI/FileManager/LangPage.cpp
  CPP/7zip/UI/FileManager/ListViewDialog.cpp
  CPP/7zip/UI/FileManager/MessagesDialog.cpp
  CPP/7zip/UI/FileManager/OverwriteDialog.cpp
  CPP/7zip/UI/FileManager/PasswordDialog.cpp
  CPP/7zip/UI/FileManager/ProgressDialog2.cpp
  CPP/7zip/UI/FileManager/SettingsPage.cpp
  CPP/7zip/UI/FileManager/SplitDialog.cpp
  CPP/7zip/UI/FileManager/SystemPage.cpp
  CPP/7zip/UI/FileManager/VerCtrl.cpp
  CPP/7zip/UI/FileManager/AltStreamsFolder.cpp
  CPP/7zip/UI/FileManager/FSDrives.cpp
  CPP/7zip/UI/FileManager/LinkDialog.cpp
  CPP/7zip/UI/FileManager/NetFolder.cpp
)

set(GUI_SOURCES
  CPP/7zip/UI/GUI/BenchmarkDialog.cpp
  CPP/7zip/UI/GUI/CompressDialog.cpp
  CPP/7zip/UI/GUI/ExtractDialog.cpp
  CPP/7zip/UI/GUI/ExtractGUI.cpp
  CPP/7zip/UI/GUI/GUI.cpp
  CPP/7zip/UI/GUI/HashGUI.cpp
  CPP/7zip/UI/GUI/UpdateCallbackGUI.cpp
  CPP/7zip/UI/GUI/UpdateCallbackGUI2.cpp
  CPP/7zip/UI/GUI/UpdateGUI.cpp
)

set(AGENT_SOURCES
  CPP/7zip/UI/Agent/Agent.cpp
  CPP/7zip/UI/Agent/AgentOut.cpp
  CPP/7zip/UI/Agent/AgentProxy.cpp
  CPP/7zip/UI/Agent/ArchiveFolder.cpp
  CPP/7zip/UI/Agent/ArchiveFolderOpen.cpp
  CPP/7zip/UI/Agent/ArchiveFolderOut.cpp
  CPP/7zip/UI/Agent/UpdateCallbackAgent.cpp
)

set(EXPLORER_SOURCES
  CPP/7zip/UI/Explorer/ContextMenu.cpp
  CPP/7zip/UI/Explorer/MyMessages.cpp
  CPP/7zip/UI/Explorer/RegistryContextMenu.cpp
)

# ============================================================================
# Resource files
# ============================================================================
set(FM_RESOURCE_RC
  CPP/7zip/Bundles/Fm/resource.rc
)

# ============================================================================
# Helper: collect all C sources with proper include paths
# ============================================================================
function(add_z7_c_library NAME)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES")
  add_library(${NAME} STATIC ${ARG_SOURCES})
  target_link_libraries(${NAME} PUBLIC z7_base)
  set_target_properties(${NAME} PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
  )
endfunction()

function(add_z7_cpp_library NAME)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES")
  add_library(${NAME} STATIC ${ARG_SOURCES})
  target_link_libraries(${NAME} PUBLIC z7_base)
  set_target_properties(${NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
  )
endfunction()

# ============================================================================
# Build C libraries
# ============================================================================
add_z7_c_library(z7_c_core SOURCES ${CORE_C_SOURCES})
add_z7_c_library(z7_c_brotli SOURCES ${BROTLI_C_SOURCES})
add_z7_c_library(z7_c_zstd SOURCES ${ZSTD_C_SOURCES})
add_z7_c_library(z7_c_zstdmt SOURCES ${ZSTDMT_C_SOURCES})
add_z7_c_library(z7_c_lz4 SOURCES ${LZ4_C_SOURCES})
add_z7_c_library(z7_c_lz5 SOURCES ${LZ5_C_SOURCES})
add_z7_c_library(z7_c_lizard SOURCES ${LIZARD_C_SOURCES})
add_z7_c_library(z7_c_fastlzma2 SOURCES ${FASTLZMA2_C_SOURCES})
add_z7_c_library(z7_c_hashes SOURCES ${HASHES_C_SOURCES})

if(Z7_WINE_LINUX)
  target_compile_definitions(z7_c_core PRIVATE _7ZIP_WINE_LINUX)
endif()

# ============================================================================
# Build CPP libraries
# ============================================================================
add_z7_cpp_library(z7_common SOURCES ${COMMON_SOURCES})
add_z7_cpp_library(z7_windows SOURCES ${WINDOWS_SOURCES})
add_z7_cpp_library(z7_7z_common SOURCES ${Z7_COMMON_SOURCES})
add_z7_cpp_library(z7_mt SOURCES ${MT_SOURCES})
add_z7_cpp_library(z7_archive SOURCES ${ARCHIVE_SOURCES})
add_z7_cpp_library(z7_archive_common SOURCES ${ARCHIVE_COMMON_SOURCES})
add_z7_cpp_library(z7_compress SOURCES ${COMPRESS_SOURCES})
add_z7_cpp_library(z7_crypto SOURCES ${CRYPTO_SOURCES})

# UI libraries
add_z7_cpp_library(z7_ui_common SOURCES ${UI_COMMON_SOURCES})
add_z7_cpp_library(z7_filemanager SOURCES ${FILEMANAGER_SOURCES})
add_z7_cpp_library(z7_gui SOURCES ${GUI_SOURCES})
add_z7_cpp_library(z7_agent SOURCES ${AGENT_SOURCES})
add_z7_cpp_library(z7_explorer SOURCES ${EXPLORER_SOURCES})

# ============================================================================
# 7zFM executable target
# ============================================================================
add_executable(7zfm.exe WIN32
  ${FM_RESOURCE_RC}
)

target_link_libraries(7zfm.exe PRIVATE
  -Wl,--whole-archive
  z7_filemanager
  z7_gui
  z7_agent
  z7_explorer
  -Wl,--no-whole-archive
  z7_ui_common
  z7_archive
  z7_archive_common
  z7_compress
  z7_crypto
  z7_mt
  z7_7z_common
  z7_windows
  z7_common
  z7_c_core
  z7_c_brotli
  z7_c_zstd
  z7_c_zstdmt
  z7_c_lz4
  z7_c_lz5
  z7_c_lizard
  z7_c_fastlzma2
  z7_c_hashes
  -lole32
  -lgdi32
  -lcomctl32
  -lcomdlg32
  -lshell32
  -loleaut32
  -luuid
  -ladvapi32
  -luser32
  -lhtmlhelp
  -lmpr
  -static-libgcc 
  -static-libstdc++ 
  -static
)

target_link_options(7zfm.exe PRIVATE
  -mwindows
  -Wl,--allow-multiple-definition
)

set_target_properties(7zfm.exe PROPERTIES
  WIN32_EXECUTABLE TRUE
  OUTPUT_NAME "7zFM"
  SUFFIX ".exe"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/7zFM"
)

# Add mingw-passthru headers if available (for missing Windows headers)
if(EXISTS "${CMAKE_SOURCE_DIR}/mingw-passthru")
  target_include_directories(z7_includes INTERFACE ${CMAKE_SOURCE_DIR}/mingw-passthru)
endif()

# ============================================================================
# DarkMode support (submodule)
# ============================================================================
if(EXISTS "${CMAKE_SOURCE_DIR}/DarkMode/lib/DarkMode.h")
  target_include_directories(z7_includes INTERFACE ${CMAKE_SOURCE_DIR}/DarkMode/lib)
endif()

# ============================================================================
# Message at configure time
# ============================================================================
message(STATUS "7-Zip Zstd + Wine Cross-compilation")
message(STATUS "  Z7_WINE_LINUX: ${Z7_WINE_LINUX}")
message(STATUS "  DISABLE_RAR:   ${DISABLE_RAR}")
message(STATUS "  Target:        7zFM.exe (File Manager)")
message(STATUS "")
message(STATUS "To build:")
message(STATUS "  mkdir build && cd build")
message(STATUS "  cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-mingw-w64.cmake")
message(STATUS "  make -j\$(nproc)")
