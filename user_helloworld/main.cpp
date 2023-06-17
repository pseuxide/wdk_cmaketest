#include <iostream>
#include <Windows.h>
#include <winternl.h>
#include <stdio.h>
#include <ntddbeep.h>

#pragma comment(lib, "ntdll")

int main(int argc, const char* argv[]) {
    printf("beep [<frequency> <duration_in_msec>]\n");
    int freq = 800, duration = 1000;
    if (argc > 2) {
        freq = atoi(argv[1]);
        duration = atoi(argv[2]);
    }

    HANDLE hFile;
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING name;
    RtlInitUnicodeString(&name, DD_BEEP_DEVICE_NAME_U);
    InitializeObjectAttributes(&attr, &name, OBJ_CASE_INSENSITIVE, nullptr, nullptr);
    IO_STATUS_BLOCK ioStatus;

    // Open handle of the device \\Device\\Beep
    NTSTATUS status = ::NtOpenFile(&hFile, GENERIC_WRITE, &attr, &ioStatus, 0, 0);
    if (NT_SUCCESS(status)) {
        BEEP_SET_PARAMETERS params;
        params.Frequency = freq;
        params.Duration = duration;
        DWORD bytes;
        printf("Playing freq: %u, duration: %u\n", freq, duration);
        ::DeviceIoControl(hFile, IOCTL_BEEP_SET, &params, sizeof(params), nullptr, 0, &bytes, nullptr);
        ::Sleep(duration);
        ::CloseHandle(hFile);
    }
}
