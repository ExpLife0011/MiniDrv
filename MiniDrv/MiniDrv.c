#include <ntddk.h>
#include <stdarg.h>
#include <ntstrsafe.h>
	
/*
	https://docs.microsoft.com/zh-cn/previous-versions//jj572863(v=vs.85)
	Add <KernelBufferOverflowLib>$(DDK_LIB_PATH)\BufferOverflowK.lib<KernelBufferOverflowLib> into project file
	
	https://docs.microsoft.com/zh-cn/windows-hardware/drivers/kernel/single-binary-opt-in-pool-nx-optin
	Add C_DEFINES=$(C_DEFINES) -DPOOL_NX_OPTIN=1 into PreprocessorDefinitions
*/
	
#define DEBUG_ON
#define DEBUG_BUFSIZE       256
	
#ifdef DEBUG_ON
static void DEBUG(PCCH format, ...)
{
	va_list args;
	char buf[DEBUG_BUFSIZE + 1];
	if (KeGetCurrentIrql() != PASSIVE_LEVEL)
	{
		return;
	}
	va_start(args, format);
	RtlStringCbVPrintfA(buf, DEBUG_BUFSIZE, format, args);
	DbgPrint("MINIDRV: %s\n", buf);
	va_end(args);
}
static void DEBUG_ERROR(PCCH format, NTSTATUS status, ...)
{
	va_list args;
	char buf[DEBUG_BUFSIZE + 1];
	if (KeGetCurrentIrql() != PASSIVE_LEVEL)
	{
		return;
	}
	va_start(args, status);
	RtlStringCbVPrintfA(buf, DEBUG_BUFSIZE, format, args);
	DbgPrint("MINIDRV: *** ERROR ***: (status = %x): %s\n", status, buf);
	va_end(args);
}
#else       // DEBUG_ON
#define DEBUG(format, ...)
#define DEBUG_ERROR(format, status, ...)
#endif      // DEBUG_ON

VOID Unload(
	_In_ struct _DRIVER_OBJECT *DriverObject
)
{
	UNREFERENCED_PARAMETER(DriverObject);

	DEBUG("[+] Unload");
}
	
NTSTATUS DriverEntry(
	_In_ struct _DRIVER_OBJECT *DriverObject,
	_In_ PUNICODE_STRING       RegistryPath
)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	DEBUG("[+] DriverEntry");

	ExInitializeDriverRuntime(DrvRtPoolNxOptIn);

	DriverObject->DriverUnload = Unload;

	return STATUS_SUCCESS;
}
