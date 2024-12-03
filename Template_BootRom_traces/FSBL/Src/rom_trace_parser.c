/* ===========================================================================*/
/*! @file rom_trace_parser.c
    @author esagnard
*/
/* ===========================================================================
 * (C) Copyright 2019 ST Microelectronics, All rights reserved
 *
 * This source code and any compilation or derivative thereof is the sole
 * property of ST Microelectronics Corporation and is provided pursuant to a Software
 * License Agreement. This code is the proprietary information of ST Microelectronics
 * Corporation and is confidential in nature. Its use and dissemination by
 * any party other than ST Microelectronics Corporation is strictly limited by the
 * confidential information provisions of the Agreement referenced above.
 * ===========================================================================
 *
 *   DESCRIPTION:
 *   ------------
 *  This file implements ROM trace parser

 *============================================================================*/


/*============================================================================*/
/* Standard Include files                                                     */
/*============================================================================*/
#include <stdint.h>
#include <stdio.h>

#include "rom_trace_parser.h"


/*============================================================================*/
/* Project Include files                                                      */
/*============================================================================*/


/*============================================================================*/
/* Types and defines                                                          */
/*============================================================================*/
typedef enum
{
    INFO = 0,
    WARN = 1,
    ERR  = 2,
    DEBUG = 3,
    eLevel_Number = 4

} infMSG_levels_t;


#define  START_WORD 0xFFDDBB00


/*============================================================================*/
/* Global data                                                                */
/*============================================================================*/

static char * infMSG_levels_TXTs[] = {
    "INFO",
    "WARN",
    "ERR ",
    "DEBUG",
};
                          ;

/*============================================================================*/
/* Internal Prototypes                                                        */
/*============================================================================*/


/*============================================================================*/
/* Exported functions                                                         */
/*============================================================================*/

/*============================================================================*/
/**
    @brief This function will convert a trace ID info text.

	@param[in]             unitID: Unit ID number

    @return
    - ROM_TRACE_ERR_OK if everything went well
    - ROM_TRACE_ERR_BAD_ID if the ID isn't found.

*/
/*============================================================================*/

static uint32_t infMSG_ID2String (uint32_t inputID, char** outString)
{
    uint32_t result = ROM_TRACE_ERR_OK;

    switch(inputID)
    {
        case 0x2C000101:
		    *outString="SECBOOT_AuthWrongMagicNumber";
            break;

        case 0x2C000102:
		    *outString="SECBOOT_AuthImageLength";
            break;

        case 0x2C000103:
		    *outString="SECBOOT_AuthenticationExtensionHeaderMissing";
            break;

        case 0x2C000104:
		    *outString="SECBOOT_AuthEccAlgoP256NIST";
            break;

        case 0x2C000105:
		    *outString="SECBOOT_AuthEccAlgoBrainPool256";
            break;

        case 0x2C000106:
		    *outString="SECBOOT_AuthEccAlgoP384NIST";
            break;

        case 0x2C000107:
		    *outString="SECBOOT_AuthEccAlgoBrainPool384";
            break;

        case 0x2C000108:
		    *outString="SECBOOT_AuthEccAlgoUnknown";
            break;

        case 0x2C00010A:
		    *outString="SECBOOT_AuthImageNotEncrypted";
            break;

        case 0x2C00010B:
		    *outString="SECBOOT_AuthImageEncrypted";
            break;

        case 0x2C00010D:
		    *outString="SECBOOT_AuthImageDecryptionFailed";
            break;

        case 0x2C00010E:
		    *outString="SECBOOT_AuthImageDecryptionSucceed";
            break;

        case 0x2C00010F:
		    *outString="SECBOOT_AuthPubKeyVerifFailed";
            break;

        case 0x2C000112:
		    *outString="SECBOOT_AuthImageSignatureKo";
            break;

        case 0x2C000113:
		    *outString="SECBOOT_AuthImageSignatureOk";
            break;

        case 0x2C000114:
		    *outString="SECBOOT_AuthPubKeyVerifOk";
            break;

        case 0x2C000201:
		    *outString="SECBOOT_AuthWrongImageChecksum";
            break;

        case 0x2C000202:
		    *outString="SECBOOT_AuthPublicKeyIsRevoked";
            break;

        case 0x2C000203:
		    *outString="SECBOOT_AuthPublicKeyNotRevoked";
            break;

        case 0x2C000204:
		    *outString="SECBOOT_AuthWrongImageVersion";
            break;

        case 0x2C000207:
		    *outString="SECBOOT_AuthPubKHashTableNotEqualToRef";
            break;

        case 0x2C000208:
		    *outString="SECBOOT_AuthImageEntryPoint";
            break;

        case 0x2C000302:
		    *outString="SECBOOT_DecryptUsingCrypDriver";
            break;

        case 0x2C000303:
		    *outString="SECBOOT_DecryptUsingSaesDriver";
            break;

        case 0x2C00030A:
		    *outString="SECBOOT_DecryptHashIntegrityError";
            break;

        case 0x2C004008:
		    *outString="SECBOOT_AuthDecisionIsJumpToImage";
            break;

        case 0x00000010:
		    *outString="BOOTCORE_LogicalResetSystem";
            break;

        case 0x00000011:
		    *outString="BOOTCORE_LogicalResetStKeyProvisioning";
            break;

        case 0x00000012:
		    *outString="BOOTCORE_LogicalResetInvalid";
            break;

        case 0x00000020:
		    *outString="BOOTCORE_ChipModeInvalid";
            break;

        case 0x00000021:
		    *outString="BOOTCORE_ChipModeStVirgin";
            break;

        case 0x00000022:
		    *outString="BOOTCORE_ChipModeStOpen";
            break;

        case 0x00000023:
		    *outString="BOOTCORE_ChipModeOpen";
            break;

        case 0x00000024:
		    *outString="BOOTCORE_ChipModeStClosedVirgin";
            break;

        case 0x00000025:
		    *outString="BOOTCORE_ChipModeClosedUnlocked";
            break;

        case 0x00000026:
		    *outString="BOOTCORE_DevBoot";
            break;

        case 0x00000027:
		    *outString="BOOTCORE_ChipModeClosedLockedUnprovd";
            break;

        case 0x00000028:
		    *outString="BOOTCORE_ChipModeClosedLockedProvd";
            break;

        case 0x00000030:
		    *outString="BOOTCORE_BootActionInvalid";
            break;

        case 0x00000031:
		    *outString="BOOTCORE_BootActionNoBoot";
            break;

        case 0x00000032:
		    *outString="BOOTCORE_BootActionStKeyProv";
            break;

        case 0x00000033:
		    *outString="BOOTCORE_BootActionSecureBootProcess";
            break;

        case 0x00000034:
		    *outString="BOOTCORE_BootActionDevBoot";
            break;

        case 0x00000052:
		    *outString="BOOTCORE_BootCfgOtpWordValue";
            break;

        case 0x00000053:
		    *outString="BOOTCORE_BootCfgAfmuxOtpWord1Value";
            break;

        case 0x00000054:
		    *outString="BOOTCORE_BootCfgAfmuxOtpWord2Value";
            break;

        case 0x00000055:
		    *outString="BOOTCORE_BootCfgAfmuxOtpWord3Value";
            break;

        case 0x00000056:
		    *outString="BOOTCORE_BootCfgHseValue";
            break;

        case 0x00000057:
		    *outString="BOOTCORE_BootCfgFsblDecryptPrio";
            break;

        case 0x00000058:
		    *outString="BOOTCORE_OtpHwConfigurationSLW";
            break;

        case 0x00000060:
		    *outString="BOOTCORE_NoUsbDpPullupOtpBitValue";
            break;

        case 0x00000061:
		    *outString="BOOTCORE_NoCpuPllOtpBitValue";
            break;

        case 0x00000062:
		    *outString="BOOTCORE_Pll1StartNotDisabledByOtpBit";
            break;

        case 0x00000065:
		    *outString="BOOTCORE_DebugNotLocked";
            break;

        case 0x00000066:
		    *outString="BOOTCORE_DebugLocked";
            break;

        case 0x00000074:
		    *outString="BOOTCORE_NoDataCacheBitValue";
            break;

        case 0x00000092:
		    *outString="BOOTCORE_BootModeDEVBOOT";
            break;

        case 0x00000093:
		    *outString="BOOTCORE_DevBootAborted";
            break;

        case 0x00000094:
		    *outString="BOOTCORE_BootModeNOBOOT";
            break;

        case 0x00000095:
		    *outString="BOOTCORE_NOBOOTLoopSVCSecure";
            break;

        case 0x00000097:
		    *outString="BOOTCORE_BootModeSECUREBOOT";
            break;

        case 0x00000155:
		    *outString="BOOTCORE_BootRomVer";
            break;

        case 0x00000156:
		    *outString="BOOTCORE_BootRomChipVer";
            break;

        case 0x00000157:
		    *outString="BOOTCORE_BootRomCutVer";
            break;

        case 0x00000158:
		    *outString="BOOTCORE_BootRomForRtlVer";
            break;

        case 0x00000159:
		    *outString="BOOTCORE_BootRomTargetPlatform";
            break;

        case 0x0000015A:
		    *outString="BOOTCORE_BootRomMaskVer";
            break;

        case 0x00001001:
		    *outString="BOOTCORE_BootPinsSrcSel";
            break;

        case 0x00001002:
		    *outString="BOOTCORE_OtpOverrideBootPins";
            break;

        case 0x0000101F:
		    *outString="BOOTCORE_SRCSEL_UNKNOWN";
            break;

        case 0x00001020:
		    *outString="BOOTCORE_SRCSEL_SERIAL";
            break;

        case 0x00001021:
		    *outString="BOOTCORE_SRCSEL_SDCARD_SDMMC1";
            break;

        case 0x00001022:
		    *outString="BOOTCORE_SRCSEL_EMMC_SDMMC1";
            break;

        case 0x00001023:
		    *outString="BOOTCORE_SRCSEL_DEV_BOOT_NONE";
            break;

        case 0x00001024:
		    *outString="BOOTCORE_SRCSEL_SNOR_OSPI1";
            break;

        case 0x00001025:
		    *outString="BOOTCORE_SRCSEL_SNAND_OSPI1";
            break;

        case 0x00001026:
		    *outString="BOOTCORE_SRCSEL_PNAND_FMC";
            break;

        case 0x00001027:
		    *outString="BOOTCORE_SRCSEL_SDCARD_SDMMC2";
            break;

        case 0x00001028:
		    *outString="BOOTCORE_SRCSEL_EMMC_SDMMC2";
            break;

        case 0x00001029:
		    *outString="BOOTCORE_SRCSEL_HYPERFLASH_OSPI1";
            break;

        case 0x00001032:
		    *outString="BOOTCORE_SerialFallback";
            break;

        case 0x00001050:
		    *outString="BOOTCORE_ImageVersionCounterNewProgVal";
            break;

        case 0x00001051:
		    *outString="BOOTCORE_ImageVersionTooBig";
            break;

        case 0x00001052:
		    *outString="BOOTCORE_VerifyErrorWhileProgrammingImageCounter";
            break;

        case 0x00001053:
		    *outString="BOOTCORE_AuthNewImageVersionCounterProgSuccess";
            break;

        case 0x00002400:
		    *outString="BOOTCORE_HseFrequencyDetected";
            break;

        case 0x00002401:
		    *outString="BOOTCORE_HseDigBypassDetected";
            break;

        case 0x00002402:
		    *outString="BOOTCORE_HseAnalogBypassDetected";
            break;

        case 0x00002403:
		    *outString="BOOTCORE_HseNoBypass";
            break;

        case 0x00002404:
		    *outString="BOOTCORE_HseFreqDetectDisabled";
            break;

        case 0x00002405:
		    *outString="BOOTCORE_HseBypassDetectDisabled";
            break;

        case 0x00002406:
		    *outString="BOOTCORE_HseFrequencyDetectFailed";
            break;

        case 0x00002407:
		    *outString="BOOTCORE_HseFrequencyFallBackSolution";
            break;

        case 0x00002408:
		    *outString="BOOTCORE_HseFreqDetectMeasuredKHz";
            break;

        case 0x00002500:
		    *outString="BOOTCORE_HwResetPOR";
            break;

        case 0x00002501:
		    *outString="BOOTCORE_HwResetBOR";
            break;

        case 0x00002502:
		    *outString="BOOTCORE_HwResetPin";
            break;

        case 0x00002506:
		    *outString="BOOTCORE_HwResetSft";
            break;

        case 0x00002508:
		    *outString="BOOTCORE_HwResetIwdgSys";
            break;

        case 0x00002509:
		    *outString="BOOTCORE_HwResetWwdgSys";
            break;

        case 0x1D000001:
		    *outString="DWNLDMGR_AssertBlocking";
            break;

        case 0x1D000002:
		    *outString="DWNLDMGR_WrongMagicNumber";
            break;

        case 0x1D000003:
		    *outString="DWNLDMGR_ImageLengthTooSmall";
            break;

        case 0x1D000004:
		    *outString="DWNLDMGR_ImageLengthTooBig";
            break;

        case 0x1D000005:
		    *outString="DWNLDMGR_EntryPointOutOfRange";
            break;

        case 0x1D000006:
		    *outString="DWNLDMGR_WrongImageChecksum";
            break;

        case 0x1D000007:
		    *outString="DWNLDMGR_WrongHeaderVersion";
            break;

        case 0x1D000008:
		    *outString="DWNLDMGR_UnknownExtensionHeader";
            break;

        case 0x1D000009:
		    *outString="DWNLDMGR_FoundFsblDecryptionHeader";
            break;

        case 0x1D00000A:
		    *outString="DWNLDMGR_FoundPkhKeyRevocationHeader";
            break;

        case 0x1D00000B:
		    *outString="DWNLDMGR_WrongExtensionHeaderLength";
            break;

        case 0x1D00000C:
		    *outString="DWNLDMGR_WrongCipherKeySize";
            break;

        case 0x1D00000D:
		    *outString="DWNLDMGR_ExtensionFlagsMismatch";
            break;

        case 0x1D00000E:
		    *outString="DWNLDMGR_WrongNumberOfPublicKeyHash";
            break;

        case 0x1D00000F:
		    *outString="DWNLDMGR_WrongPublicKeyIndex";
            break;

        case 0x1D000010:
		    *outString="DWNLDMGR_FoundPkhAuthenticationHeader";
            break;

        case 0x1D000011:
		    *outString="DWNLDMGR_FoundPaddingHeader";
            break;

        case 0x80000001:
		    *outString="OSPI_TryingSECBL1";
            break;

        case 0x80000002:
		    *outString="OSPI_TryingSECBL2";
            break;

        case 0x80000003:
		    *outString="OSPI_TryingSingleMode";
            break;

        case 0x80000004:
		    *outString="OSPI_TryingDualMode";
            break;

        case 0x80000005:
		    *outString="OSPI_FoundGpt";
            break;

        case 0x80000006:
		    *outString="OSPI_FoundMagicAtDefaultLocation";
            break;

        case 0x80000007:
		    *outString="OSPI_UseOffsets";
            break;

        case 0x80000008:
		    *outString="OSPI_WrongImageLen";
            break;

        case 0x80000009:
		    *outString="OSPI_Timeout";
            break;

        case 0x8000000A:
		    *outString="OSPI_Assertion";
            break;

        case 0x13000001:
		    *outString="MMC_AcknowledgeErr";
            break;

        case 0x13000002:
		    *outString="MMC_AcknowledgeTimeout";
            break;

        case 0x13000003:
		    *outString="MMC_CardNotDetected";
            break;

        case 0x13000004:
		    *outString="MMC_DataCrcErr";
            break;

        case 0x13000005:
		    *outString="MMC_DataTimeout";
            break;

        case 0x13000006:
		    *outString="MMC_InvalidParameter";
            break;

        case 0x13000007:
		    *outString="MMC_DmaTransferErr";
            break;

        case 0x13000008:
		    *outString="MMC_CmdTimeoutErr";
            break;

        case 0x13000009:
		    *outString="MMC_CmdCrcErr";
            break;

        case 0x1300000A:
		    *outString="MMC_AddAlignErr";
            break;

        case 0x1300000B:
		    *outString="MMC_NotEnoughBootDataRxErr";
            break;

        case 0x1300000C:
		    *outString="MMC_ImageCopiedIsNotComplete";
            break;

        case 0x1300000D:
		    *outString="MMC_ImageCopiedHeaderSizeZero";
            break;

        case 0x1300000E:
		    *outString="MMC_AttemptBootDespiteErrors";
            break;

        case 0x1300000F:
		    *outString="MMC_GotDataEndFlagWithNoErrors";
            break;

        case 0x13000010:
		    *outString="MMC_NbBytesCopiedInSysram";
            break;

        case 0x13000011:
		    *outString="MMC_ImageCopiedHeaderNotFound";
            break;

        case 0x13000012:
		    *outString="MMC_TrySecBL1";
            break;

        case 0x13000013:
		    *outString="MMC_NotFSBLFound";
            break;

        case 0x13000014:
		    *outString="MMC_FsblsFound";
            break;

        case 0x13000015:
		    *outString="MMC_FailedSecBL1TrySecBL2";
            break;

        case 0x13000016:
		    *outString="MMC_InitError";
            break;

        case 0x13000017:
		    *outString="MMC_TransferFailed";
            break;

        case 0x13000018:
		    *outString="MMC_GPTFound";
            break;

        case 0x13000019:
		    *outString="MMC_NotGPTFound";
            break;

        case 0x1300001F:
		    *outString="MMC_Assertion";
            break;

        case 0x14000000:
		    *outString="USB_Init";
            break;

        case 0x14000001:
		    *outString="USB_Loop";
            break;

        case 0x14000002:
		    *outString="USB_Selection";
            break;

        case 0x14000004:
		    *outString="USB_DFU_DownloadComplete";
            break;

        case 0x14000005:
		    *outString="USB_DFU_GetStatus";
            break;

        case 0x14000006:
		    *outString="USB_DFU_GetStatus_GoTo_Idle";
            break;

        case 0x14000007:
		    *outString="USB_DFU_END_OF_DNLOAD";
            break;

        case 0x14000008:
		    *outString="USB_DFU_DNLOAD";
            break;

        case 0x14000009:
		    *outString="USB_DFU_UPLOAD";
            break;

        case 0x1400000A:
		    *outString="USB_DFU_GETSTATUS";
            break;

        case 0x1400000B:
		    *outString="USB_DFU_CLRSTATUS";
            break;

        case 0x1400000C:
		    *outString="USB_DFU_GETSTATE";
            break;

        case 0x1400000D:
		    *outString="USB_DFU_ABORT";
            break;

        case 0x1400000E:
		    *outString="USB_DFU_DETACH";
            break;

        case 0x1400000F:
		    *outString="USB_DFU_LEAVE";
            break;

        case 0x14000010:
		    *outString="USB_DFU_DataOut";
            break;

        case 0x14000011:
		    *outString="USB_DFU_GetStatusMedia";
            break;

        case 0x14000012:
		    *outString="USB_DFU_SRAMWrite";
            break;

        case 0x14000013:
		    *outString="USB_DFU_SRAMWrite_CRC";
            break;

        case 0x14000014:
		    *outString="USB_DFU_SRAMWrite_imgL";
            break;

        case 0x14000015:
		    *outString="USB_DFU_Init";
            break;

        case 0x14000016:
		    *outString="USB_DFU_SET_INTERFACE";
            break;

        case 0x14000017:
		    *outString="USB_DFU_SET_INTERFACE_ERR";
            break;

        case 0x14000018:
		    *outString="USB_DFU_Get_INTERFACE";
            break;

        case 0x14000019:
		    *outString="USB_DFU_LEAVE_EP0_TxReady";
            break;

        case 0x14000020:
		    *outString="USB_ClockTree_Failed";
            break;

        case 0x14000021:
		    *outString="USB_WrongDataTransfer";
            break;

        case 0x14000022:
		    *outString="USB_DataTransfered";
            break;

        case 0x14000023:
		    *outString="USB_Power_Failed";
            break;

        case 0x14000030:
		    *outString="USB_DFU_DetachComplete";
            break;

        case 0x14000031:
		    *outString="USB_DEBUG";
            break;

        case 0x14000040:
		    *outString="USB_DFU_SPEC_CMD_NOT_SUPPORTED";
            break;

        case 0x14000041:
		    *outString="USB_DFU_GetUsrStringDesc";
            break;

        case 0x14000050:
		    *outString="USB_CallingSetLockedUnprovd";
            break;

        case 0x14000051:
		    *outString="USB_WrongDestCheck";
            break;

        case 0x140FFFFF:
		    *outString="USB_Assertion";
            break;

        case 0x15000001:
		    *outString="UART_Selection";
            break;

        case 0x15000002:
		    *outString="UART_PeripheralSerialBootLoopStart";
            break;

        case 0x15000003:
		    *outString="UART_PacketNumber";
            break;

        case 0x15000004:
		    *outString="UART_PacketSize";
            break;

        case 0x15000005:
		    *outString="UART_PacketXor";
            break;

        case 0x15000006:
		    *outString="UART_ImageLength";
            break;

        case 0x15000007:
		    *outString="UART_Cursor";
            break;

        case 0x15000008:
		    *outString="UART_NACK";
            break;

        case 0x15000009:
		    *outString="UART_Abort";
            break;

        case 0x1500000A:
		    *outString="UART_ActivityDetected";
            break;

        case 0x1500000B:
		    *outString="UART_ActivityDetecting";
            break;

        case 0x1500000C:
		    *outString="UART_ReceivedByte";
            break;

        case 0x1500000D:
		    *outString="UART_DownloadFinished";
            break;

        case 0x1500000E:
		    *outString="UART_Start";
            break;

        case 0x15000020:
		    *outString="UART_CallingSetLockedUnprovd";
            break;

        case 0x15000100:
		    *outString="UART_AssertBlocking";
            break;

        case 0x29000000:
		    *outString="TIMEOUT_Assertion";
            break;

        case 0x12000000:
		    *outString="SD_InvalidParameter";
            break;

        case 0x12000001:
		    *outString="SD_InitFailed";
            break;

        case 0x12000002:
		    *outString="SD_TransferFailed";
            break;

        case 0x12000003:
		    *outString="SD_CardDetected";
            break;

        case 0x12000004:
		    *outString="SD_NoCardResponse";
            break;

        case 0x12000005:
		    *outString="SD_TryFSBL1";
            break;

        case 0x12000006:
		    *outString="SD_FailedFSBL1TryFSBL2";
            break;

        case 0x12000007:
		    *outString="SD_InitDetailedErrorDesc";
            break;

        case 0x12000008:
		    *outString="SD_ImageTooBig";
            break;

        case 0x12000009:
		    *outString="SD_ImageLengthZero";
            break;

        case 0x1200000A:
		    *outString="SD_StdCapacityCardV11";
            break;

        case 0x1200000B:
		    *outString="SD_StdCapacityCardV20";
            break;

        case 0x1200000C:
		    *outString="SD_HighCapacityCardSDHCOrSDXC";
            break;

        case 0x1200000D:
		    *outString="SD_UnknownCapacityCard";
            break;

        case 0x1200000E:
		    *outString="SD_OverallRetryCnt";
            break;

        case 0x1200000F:
		    *outString="SD_ImageHeaderNotFound";
            break;

        case 0x12000010:
		    *outString="SD_NotGPTFound";
            break;

        case 0x12000011:
		    *outString="SD_FsblsFound";
            break;

        case 0x12000012:
		    *outString="SD_GPTFound";
            break;

        case 0x12000013:
		    *outString="SD_NoFSBLFound";
            break;

        case 0x1200001F:
		    *outString="SD_Assertion";
            break;

        case 0x2E000001:
		    *outString="INFHARDLIB_AssertBlocking";
            break;


        default:
            result = ROM_TRACE_ERR_KO;
            break;
    }

    return (result);
}


/*============================================================================*/
/**
    @brief This function will convert a trace level info text.

    @param[in]             inputID: The level to parse
    @param[out]            outString: The level in text format

    @return
    - ROM_TRACE_ERR_OK if everything went well
    - ROM_TRACE_ERR_BAD_ID if the ID isn't found.

*/
/*============================================================================*/

static uint32_t infMSG_Level2String (uint32_t inputID, char** outString)
{
    if (inputID <= eLevel_Number)
    {
        *outString = infMSG_levels_TXTs[inputID];
        return ROM_TRACE_ERR_OK;
    }

    return ROM_TRACE_ERR_KO;
}


/*
 * Read next word
 */
static int readNextWord(romTrace_Iterator_t *iterPtr, uint32_t *p_word)
{
    if (iterPtr->remaining_size > 0)
    {
        iterPtr->remaining_size -= 4;
        *p_word = *iterPtr->p_cur_trace;
        iterPtr->p_cur_trace++;
        return ROM_TRACE_ERR_OK;
    }
    return ROM_TRACE_ERR_KO ;
}


/*============================================================================*/
/**
    @brief This function starts ROM trace iteration.

    It initializes itetator structure and looks for start word.

    @param[in]             iterPtr: pointer to trace iterator
    @param[in]             inPtr: Pointer to a binary trace buffer
    @param[in]             inSize: size of binary trace buffer

    @return
    - ROM_TRACE_ERR_OK            if everything went well
    - ROM_TRACE_ERR_WRONG_SIZE    if given size is not a multiple of 4 bytes
    - ROM_TRACE_ERR_NO_START_WORD if function cannot find start word.

*/
/*============================================================================*/
int32_t romTrace_IterateInit(romTrace_Iterator_t *iterPtr, uint8_t *inPtr, uint32_t inSize)
{
    uint32_t *readPtr = (uint32_t*)inPtr;

    iterPtr->p_trace_buffer = inPtr;
    iterPtr->trace_buffer_size = inSize;

    if ((inSize % 4) != 0)
    {
        return ROM_TRACE_ERR_WRONG_SIZE;
    }

    while (inSize > 0 && *readPtr != START_WORD)
    {
        readPtr++;
        inSize -= 4;
    }
    if (inSize > 0)
    {
        iterPtr->p_cur_trace = readPtr;
        iterPtr->remaining_size = inSize;
        return ROM_TRACE_ERR_OK;
    }
    else
    {
        return ROM_TRACE_ERR_NO_START_WORD;
    }
}


/*============================================================================*/
/**
    @brief This function convert next trace into text.

    @param[in]             iterPtr: pointer to trace iterator
    @param[out]            outPtr: Output string containing the parsed trace
    @param[in]             outSize: maximum outPtr size

    @return
    - ROM_TRACE_ERR_OK            if everything went well
    - ROM_TRACE_ERR_WRONG_SIZE    if given size is not a multiple of 4 bytes
    - ROM_TRACE_ERR_NO_START_WORD if function cannot find start word.

*/
/*============================================================================*/
int32_t romTrace_IterateNext(romTrace_Iterator_t *iterPtr, char *outPtr, uint32_t *timestamp, uint32_t outSize)
{
    uint32_t startWord;
    int32_t size;
    uint32_t level;
    uint32_t messageCode;
    int len;

    char * msgTXT;
    char * levelTXT;

    /* Get Trace content */
    do 
    {
        if (readNextWord(iterPtr, &startWord) != ROM_TRACE_ERR_OK)
        {
            return ROM_TRACE_ERR_END_OF_BUFFER;
        }

    } while (startWord != START_WORD);

    if (readNextWord(iterPtr, (uint32_t*)&size) != ROM_TRACE_ERR_OK)
    {
        return ROM_TRACE_ERR_END_OF_BUFFER;
    }

    if (size < (sizeof(*timestamp) + sizeof(level) + sizeof(messageCode)))
    {
        return ROM_TRACE_ERR_WRONG_SIZE;
    }

    if (readNextWord(iterPtr, timestamp) != ROM_TRACE_ERR_OK)
    {
        return ROM_TRACE_ERR_END_OF_BUFFER;
    }
    size -= (sizeof(*timestamp));

    if (readNextWord(iterPtr, &level) != ROM_TRACE_ERR_OK)
    {
        return ROM_TRACE_ERR_END_OF_BUFFER;
    }
    size -= (sizeof(level));

    if (infMSG_Level2String(level, &levelTXT) != ROM_TRACE_ERR_OK)
    {
        return ROM_TRACE_ERR_WRONG_LEVEL;
    }

    if (readNextWord(iterPtr, &messageCode) != ROM_TRACE_ERR_OK)
    {
        return ROM_TRACE_ERR_END_OF_BUFFER;
    }
    size -= (sizeof(messageCode));

    if (infMSG_ID2String(messageCode, &msgTXT) != ROM_TRACE_ERR_OK)
    {
        // message code unknown, skip arguments
        while  (size > 0)
        {
            uint32_t tmp;
            if (readNextWord(iterPtr, &tmp) != ROM_TRACE_ERR_OK)
            {
                return ROM_TRACE_ERR_END_OF_BUFFER;
            }
            size -= sizeof (uint32_t);
        }

        return ROM_TRACE_ERR_KO;
    }

    else /* if (infMSG_ID2String(messageCode, &msgTXT) != ROM_TRACE_ERR_OK */
    {
        len = snprintf(outPtr, outSize, "< [%s] - %s",
                     levelTXT,
                     msgTXT);
        outPtr += len;
        outSize = (outSize > len)? (outSize - len) : 0;

        /* Get optional data */
        if (size > 0)
        {
            len = snprintf(outPtr, outSize, " (");
            outPtr += len;
            outSize = (outSize > len)? (outSize - len) : 0;

            while  (size > 0)
            {
                uint32_t tmp;

                if (readNextWord(iterPtr, &tmp) != ROM_TRACE_ERR_OK)
                {
                    return ROM_TRACE_ERR_END_OF_BUFFER;
                }

                len = snprintf(outPtr, outSize, " 0x%08X,", tmp);
                outPtr += len;
                outSize = (outSize > len)? (outSize - len) : 0;
                size -= sizeof (uint32_t);
            }

            outPtr--; outSize++; // remove last ','
            len = snprintf(outPtr, outSize, " )");
            outPtr += len;
            outSize = (outSize > len)? (outSize - len) : 0;
        }

        snprintf(outPtr, outSize, " >");
    }

    return ROM_TRACE_ERR_OK;

}



/*============================================================================*/
/* End of File                                                                */
/*============================================================================*/

