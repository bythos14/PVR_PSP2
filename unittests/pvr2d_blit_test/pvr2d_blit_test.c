#include <kernel.h>

SCE_USER_MODULE_LIST("app0:libgpu_es4_ext.suprx", "app0:libpvr2d.suprx");

#include "services.h"
#include "pvr_debug.h"
#include "pvr2d.h"

#include <stdio.h>

#define BLIT_BPP 4

#define FAIL_IF_ERROR(result, calledFunction) CheckError(result, #calledFunction)

static IMG_VOID CheckError(PVR2DERROR eResult, IMG_PCHAR pszCalledFunction)
{
#define PRINT_ERROR(error) case error: PVR_DPF((PVR_DBG_ERROR, "%s returned "#error, pszCalledFunction)); break

	switch (eResult)
	{
	case PVR2D_OK: 
		PVR_DPF((PVR_DBG_MESSAGE, "%s returned PVR2D_OK", pszCalledFunction));
		return;
	PRINT_ERROR(PVR2DERROR_INVALID_PARAMETER);
	PRINT_ERROR(PVR2DERROR_DEVICE_UNAVAILABLE);
	PRINT_ERROR(PVR2DERROR_INVALID_CONTEXT);
	PRINT_ERROR(PVR2DERROR_MEMORY_UNAVAILABLE);
	PRINT_ERROR(PVR2DERROR_DEVICE_NOT_PRESENT);
	PRINT_ERROR(PVR2DERROR_IOCTL_ERROR);
	PRINT_ERROR(PVR2DERROR_GENERIC_ERROR);
	PRINT_ERROR(PVR2DERROR_BLT_NOTCOMPLETE);
	PRINT_ERROR(PVR2DERROR_HW_FEATURE_NOT_SUPPORTED);
	PRINT_ERROR(PVR2DERROR_NOT_YET_IMPLEMENTED);
	PRINT_ERROR(PVR2DERROR_MAPPING_FAILED);
	default:
		PVR_DPF((PVR_DBG_ERROR, "%s returned unrecognized error 0x%08X", pszCalledFunction));
		return;
	}
#undef PRINT_ERROR

	abort();
}

static PVR2DERROR LoadExternalImage(PVR2DCONTEXTHANDLE hContext, IMG_PCHAR pszFilename, PVR2DMEMINFO **ppsMemInfo)
{
	FILE *fd;
	IMG_PVOID pvBuffer;
	IMG_UINT32 ui32Size;
	PVR2DERROR eResult = PVR2D_OK;
		
	fd = fopen(pszFilename, "rb");
	if (fd == IMG_NULL)
	{
		PVR_DPF((PVR_DBG_ERROR, "Failed to open file %s!", pszFilename));
		return PVR2DERROR_GENERIC_ERROR;
	}

	fseek(fd, 0, SEEK_END);
	ui32Size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	eResult = PVR2DMemAlloc(hContext, ui32Size, 0x10, PVR2D_MEM_UNCACHED | PVR2D_PSP2_MEM_MAIN, ppsMemInfo);
	if (eResult != PVR2D_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "Failed to allocate memory"));
		goto close;
	}

	fread((*ppsMemInfo)->pBase, 1, ui32Size, fd);

close:
	fclose(fd);

	return eResult;
}

IMG_INT
main(IMG_INT argc, IMG_CHAR **argv)
{
	PVR2DDEVICEINFO asDevInfo[PVRSRV_MAX_DEVICES];
	PVR2D_INT iDevCount;
	PVR2DERROR eResult;
	PVR2DCONTEXTHANDLE hContext;
	PVR2DMEMINFO *psSrcMemInfo, *psDstMemInfo;
	PVR2DBLTINFO sBlitInfo = { 0 };

	eResult = PVR2DEnumerateDevices(asDevInfo);
	FAIL_IF_ERROR(eResult, PVR2DEnumerateDevices);

	eResult = PVR2DCreateDeviceContext(asDevInfo[0].ulDevID, &hContext, 0);
	FAIL_IF_ERROR(eResult, PVR2DCreateDeviceContext);

	eResult = PVR2DMemAlloc(hContext, 128 * 128 * BLIT_BPP, 0x10, PVR2D_MEM_UNCACHED | PVR2D_PSP2_MEM_CDRAM, &psSrcMemInfo);
	FAIL_IF_ERROR(eResult, PVR2DMemAlloc);
	sceClibMemset(psSrcMemInfo->pBase, 0x00, psSrcMemInfo->ui32MemSize);

	sBlitInfo.CopyCode = 0xF0;
	sBlitInfo.BlitFlags = PVR2D_BLIT_PATH_2DCORE;
	sBlitInfo.Colour = 0xFFFF0000; // Solid Red
	sBlitInfo.SrcFormat = PVR2D_ARGB8888;
	sBlitInfo.DstFormat = PVR2D_ARGB8888;

	sBlitInfo.pSrcMemInfo = psSrcMemInfo;
	sBlitInfo.SrcOffset = 0;
	sBlitInfo.SrcStride = 128 * BLIT_BPP;
	sBlitInfo.pDstMemInfo = psSrcMemInfo;
	sBlitInfo.DstOffset = 0;
	sBlitInfo.DstStride = 128 * BLIT_BPP;

	sBlitInfo.SrcX = 0;
	sBlitInfo.SrcY = 0;
	sBlitInfo.DstX = 0;
	sBlitInfo.DstY = 0;
	sBlitInfo.SizeX = 128;
	sBlitInfo.SizeY = 128;
	sBlitInfo.DSizeX = 128;
	sBlitInfo.DSizeY = 128;
	sBlitInfo.SrcSurfWidth = 128;
	sBlitInfo.SrcSurfHeight = 128;
	sBlitInfo.DstSurfWidth = 128;
	sBlitInfo.DstSurfHeight = 128;

	eResult = PVR2DBlt(hContext, &sBlitInfo);
	FAIL_IF_ERROR(eResult, PVR2DBlt);

	eResult = PVR2DQueryBlitsComplete(hContext, psSrcMemInfo, PVR2D_TRUE);
	FAIL_IF_ERROR(eResult, PVR2DQueryBlitsComplete);

	sceClibPrintf("Pixel at 63,63 = 0x%08X\n", *(IMG_UINT32 *)((IMG_UINT8 *)psSrcMemInfo->pBase + (128 * 63) + 63));

	sceKernelExitProcess(0);
	return 0;
fail:
	sceKernelExitProcess(-1);
	return -1;
}