#include "credits/offset_table.h"

s32 getSubtitleScheduleEntryCount(SubtitleSchedule *schedule) {
    return *(s32 *)(schedule->entryCountOffset + (s32)schedule);
}

s16 getSubtitleScheduleEntryStartFrame(SubtitleSchedule *schedule, s32 index) {
    SubtitleScheduleEntry *entry;
    if (index < getSubtitleScheduleEntryCount(schedule)) {
        entry =
            (SubtitleScheduleEntry *)(schedule->entriesOffset + (s32)schedule + index * sizeof(SubtitleScheduleEntry));
        return entry->startFrame;
    }
    return -1;
}

u8 *getSubtitleScheduleEntryCommands(SubtitleSchedule *schedule, s32 index) {
    SubtitleScheduleEntry *entry;
    s32 commandsOffset;
    if (index < getSubtitleScheduleEntryCount(schedule)) {
        entry =
            (SubtitleScheduleEntry *)(schedule->entriesOffset + (s32)schedule + index * sizeof(SubtitleScheduleEntry));
        commandsOffset = entry->commandsOffset;
        if (commandsOffset != -1) {
            return (u8 *)(commandsOffset + (s32)schedule);
        }
    }
    return 0;
}

s16 getSubtitleScheduleEntryCommandCount(SubtitleSchedule *schedule, s32 index) {
    SubtitleScheduleEntry *entry;
    if (index < getSubtitleScheduleEntryCount(schedule)) {
        entry =
            (SubtitleScheduleEntry *)(schedule->entriesOffset + (s32)schedule + index * sizeof(SubtitleScheduleEntry));
        return entry->commandCount;
    }
    return -1;
}
