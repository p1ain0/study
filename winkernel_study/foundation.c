//#include <ntddk.h>
#include <ntifs.h>
#pragma warning(disable:4189)
// 提供一个Unload函数只是为了让程序能够动态卸载,方便调试
VOID DriverUnload(PDRIVER_OBJECT driver)
{
    driver;
    // 但是实际上我们什么都不做，只打印一句话:
    DbgPrint("first: Our driver is unloading…\r\n");
}

// DriverEntry，入口函数。相当于main。
NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING reg_path)
{
    driver;
    reg_path;


    KdBreakPoint();
    // 这是我们的内核模块的入口，可以在这里写入我们想写的东西。
    DbgPrint("first: Hello world!");

    // 设置一个卸载函数便于这个函数能退出。
    driver->DriverUnload = DriverUnload;

    // enviroment 
    PEPROCESS eprocess = IoGetCurrentProcess();
    HANDLE pid = PsGetCurrentProcessId();

    KIRQL irql = KeGetCurrentIrql();

    if(FALSE)
        KeBugCheckEx(0xb,0xb,0xb,0xb,0xb);
    
    // string
    UNICODE_STRING ustring = { 0 };
    RtlInitUnicodeString(&ustring, L"fsfsf");
    DbgPrint("Unicode String: %wZ", &ustring);

    //link list
    typedef struct _TestListEntry {
        ULONG m_DataA;
        ULONG m_DataB;
        ULONG m_DataC;
        LIST_ENTRY m_ListEntry;
        ULONG m_DataD;
    }TestListEntry, *PTestListEntry;

    LIST_ENTRY ListHeader = { 0 };
    InitializeListHead(&ListHeader);

    TestListEntry EntryA = { 0 };
    TestListEntry EntryB = { 0 };
    TestListEntry EntryC = { 0 };

    EntryA.m_DataA = 'A';
    EntryB.m_DataA = 'B';
    EntryC.m_DataA = 'C';
    InsertHeadList(&ListHeader, &EntryA.m_ListEntry);
    InsertHeadList(&ListHeader, &EntryB.m_ListEntry);
    InsertHeadList(&ListHeader, &EntryC.m_ListEntry);

    PLIST_ENTRY pListEntry = NULL;
    pListEntry = ListHeader.Flink;
    while (pListEntry != &ListHeader)
    {
        PTestListEntry pTestEntry = CONTAINING_RECORD(pListEntry, TestListEntry, m_ListEntry);
        DbgPrint("ListPtr = %p, Entry = %p, Tag = %c\n", pListEntry, pTestEntry, (CHAR)pTestEntry->m_DataA);
        pListEntry = pListEntry->Flink;
    }

    //spin lock
    KSPIN_LOCK spin_lock;
    KeInitializeSpinLock(&spin_lock);
    KIRQL irql2;
    KeAcquireSpinLock(&spin_lock, &irql2);
    //do something...
    KeReleaseSpinLock(&spin_lock, irql2);
    
    //queue spin lock
    KSPIN_LOCK queue_spin_lock;
    KeInitializeSpinLock(&spin_lock);
    KLOCK_QUEUE_HANDLE lock_queue_handle;
    KeAcquireInStackQueuedSpinLock(&queue_spin_lock, &lock_queue_handle);
    //do something...
    KeReleaseInStackQueuedSpinLock(&lock_queue_handle);

    return STATUS_SUCCESS;
}

// object and handle

BOOLEAN EventOperationSample()
{
    BOOLEAN bSucc = FALSE;
    HANDLE hCreateEvent = NULL;
    PVOID pCreateEventObject = NULL;
    HANDLE hOpenEvent = NULL;
    PVOID pOpenEventObject = NULL;

    OBJECT_ATTRIBUTES ObjAttr = { 0 };
    UNICODE_STRING uNameString = { 0 };
    RtlInitUnicodeString(&uNameString, L"\\BaseNameObjects\\TestEvent");
    InitializeObjectAttributes(&ObjAttr, &uNameString, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
    ZwCreateEvent(&hCreateEvent, EVENT_ALL_ACCESS, &ObjAttr, SynchronizationEvent, FALSE);

    if (hCreateEvent == NULL)
    {
        goto end;
    }

    ObReferenceObjectByHandle(hCreateEvent, EVENT_ALL_ACCESS, *ExEventObjectType, KernelMode, &pCreateEventObject, NULL);
    if (pCreateEventObject == NULL)
    {
        goto end;
    }

    ZwOpenEvent(&hOpenEvent, EVENT_ALL_ACCESS, &ObjAttr);
    if (hOpenEvent == NULL)
    {
        goto end;
    }

    ObReferenceObjectByHandle(hOpenEvent, EVENT_ALL_ACCESS, *ExEventObjectType, KernelMode, &pOpenEventObject, NULL);
    if (pOpenEventObject == NULL)
    {
        goto end;
    }

    DbgPrint("Create Handle:%p, Create Pointer = %p\n", hCreateEvent, pCreateEventObject);
    DbgPrint("Open Handle:%p, Create Pointer = %p\n", hOpenEvent, pOpenEventObject);
    bSucc = TRUE;

end:
    if (pCreateEventObject != NULL)
    {
        ObDereferenceObject(pCreateEventObject);
        pCreateEventObject = NULL;
    }

    if (hCreateEvent != NULL)
    {
        ZwClose(hCreateEvent);
        hCreateEvent = NULL;
    }

    if (pOpenEventObject != NULL)
    {
        ObDereferenceObject(pOpenEventObject);
        pOpenEventObject = NULL;
    }

    if (hOpenEvent != NULL)
    {
        ZwClose(hOpenEvent);
        hOpenEvent = NULL;
    }

    return bSucc;
}

BOOLEAN RegisterDemo(PUNICODE_STRING RegisterPath)
{
    OBJECT_ATTRIBUTES ObjAttr = { 0 };
    HANDLE hKey = NULL;
    ULONG ulDisposition = 0;
    NTSTATUS nStatus = STATUS_SUCCESS;
    InitializeObjectAttributes(
        &ObjAttr,
        RegisterPath,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
        NULL,
        NULL);
    nStatus = ZwCreateKey(&hKey, KEY_WRITE, &ObjAttr, 0, NULL, REG_OPTION_NON_VOLATILE, &ulDisposition);
    if (hKey)
    {
        UNICODE_STRING usValueName = { 0 };
        ULONG ulNewStartValue = 2;
        RtlInitUnicodeString(&usValueName, L"Start");
        nStatus = ZwSetValueKey(hKey, &usValueName, 0, REG_DWORD, (PVOID)&ulNewStartValue, sizeof(ulNewStartValue));

        ZwClose(hKey);
        hKey = NULL;
    }
    return TRUE;
}

//file demo

BOOLEAN FileDemo()
{

}
