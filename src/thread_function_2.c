s32 D_8009B020_9BC20;
s32 D_8009B024_9BC24;
OSMesgQueue *eventQueue1;
OSMesgQueue *eventQueue3;
OSMesgQueue *frameBufferQueue;

void thread_function_2(void* arg) {
    OSMesg msg;
    s32 task;
    s32 next_task;
    s32 status_flags = 0;
    s32 current_task;
    s32 pending_task;
    s32 event;
    s32 mask;

    while (TRUE) {
        osRecvMesg(&eventQueue1, &msg, OS_MESG_BLOCK);
        event = (u32)msg;
        
        if (event == 7) { // Task start event
            if (D_8009B020_9BC20 == 0) {
                current_task = ((s32*)msg)[1];
                
                if (!(status_flags & 2)) {
                    status_flags |= 1;
                    mask = osSetIntMask(OS_IM_NONE);
                    osWritebackDCacheAll();
                    osSetIntMask(mask);
                    osSpTaskStart(current_task);
                } else {
                    status_flags |= 4;
                }
            }
        } else if (event == 0xB) { // Task swap event
            if (D_8009B024_9BC24 >= 5) {
                pending_task = ((s32*)msg)[2];
                
                if (status_flags & 1) {
                    status_flags |= 8;
                    osSpTaskYield();
                } else {
                    s32 mask = osSetIntMask(OS_IM_NONE);
                    osWritebackDCacheAll();
                    osSetIntMask(mask);
                    osSpTaskStart(pending_task);
                    status_flags |= 2;
                }
            }
        } else if (event == 6) { // Task completion event
            if (status_flags & 2) {
                status_flags &= ~2;
                
                if (status_flags & 4) {
                    if (D_8009B020_9BC20 == 0) {
                        s32 mask = osSetIntMask(OS_IM_NONE);
                        osWritebackDCacheAll();
                        osSetIntMask(mask);
                        osSpTaskStart(current_task);
                        status_flags |= 1;
                    }
                    status_flags &= ~4;
                }
                
                if (status_flags & 0x10) {
                    osSpTaskStart(current_task);
                    status_flags = (status_flags & ~0x10) | 1;
                }
                osSendMesg(&frameBufferQueue, (OSMesg)6, OS_MESG_BLOCK);
            } else if (status_flags & 1) {
                status_flags &= ~1;
                
                if (status_flags & 8) {
                    status_flags |= 0x20;
                } else {
                    if (osSpTaskYielded(current_task) == 0) {
                        status_flags |= 0x20;
                    } else {
                        status_flags |= 0x10;
                    }
                    mask = osSetIntMask(OS_IM_NONE);
                    osWritebackDCacheAll();
                    osSetIntMask(mask);
                    osSpTaskStart(pending_task);
                    status_flags = (status_flags & ~8) | 2;
                }
                if (status_flags & 0x20) {
                    status_flags &= ~0x20;
                    osSendMesg(&eventQueue3, (OSMesg)6, OS_MESG_BLOCK);
                }
            }
        }
    }
}