#include "common.h"
#include "gamestate.h"

void schedulePlayerHaloTask(Player *);

void scheduleCourseTasks(s32 courseId, s32 playerCount);

void scheduleLevelEnvironmentTasks(s32 poolId);

void spawnBossHomingProjectileTask(void *boss);

void spawnBossHomingProjectileVariant1Task(Player *);

void *spawnItemHomingProjectile(void *, u32, void *, s16, s32);

void func_8004C10C_4CD0C(Player *);
