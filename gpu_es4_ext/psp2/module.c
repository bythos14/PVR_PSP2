#include <kernel.h>

int _PVRSRVCreateUserModeHeap();

int __module_stop(SceSize argc, const void *args)
{
	return SCE_KERNEL_STOP_SUCCESS;
}

int __module_exit()
{
	return SCE_KERNEL_STOP_SUCCESS;
}

int __module_start(SceSize argc, void *args)
{
	SceInt32 ret;

	ret = _PVRSRVCreateUserModeHeap();

	if (ret != SCE_OK)
	{
		return SCE_KERNEL_START_NO_RESIDENT;
	}

	return SCE_KERNEL_START_SUCCESS;
}