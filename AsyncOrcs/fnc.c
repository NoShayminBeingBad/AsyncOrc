#include "defs.h"

/*
  Function:  initRaceInfo
  Purpose:   inilalize a RaceInfoType
       in:   the row where STATUS starts to print
       in:   the RaceInfoType being created
   result:   the result of sem_init
*/
int initRaceInfo(int statR, RaceInfoType **r){
  *r = malloc(sizeof(RaceInfoType));
  (*r)->numRunners = 0;
  (*r)->numDorcs = 0;
  strcpy((*r)->winner, "");
  (*r)->statusRow = statR;
  return sem_init(&((*r)->mutex), 0, 1);
}

/*
  Function:  initDorc
  Purpose:   inilalize a Dorc
       in:   the EntityType becoming a dorc
   result:
*/
void initDorc(EntityType **d){
  int r = randm(MAX_P_SPACE);
  initEntity("d", END_POS + 1, PATH_1 + r, d);
}

/*
  Function:  initEntity
  Purpose:   initialize an entity
       in:   the avatar
       in:   the starting row
       in:   starting col
       in:   the entity being created
   result:
*/
void initEntity(char a[], int pos, int p, EntityType **en){
  *en = malloc(sizeof(EntityType));
  strcpy((*en)->avatar, a);
  (*en)->currPos = pos;
  (*en)->path = p;
}

/*
  Function:  initRunner
  Purpose:   the name
       in:   starting health
       in:   avatar
       in:   starting row
       in:   starting col
       in:   runner being created
   result:
*/
void initRunner(char n[], int heal, char a[], int pos, int p, RunnerType **r){
  *r = malloc(sizeof(RunnerType));
  strcpy((*r)->name, n);
  (*r)->health = heal;
  (*r)->dead = 0;

  strcpy((*r)->ent.avatar, a);
  (*r)->ent.currPos = pos;
  (*r)->ent.path = p;
}

/*
  Function:  addRunner
  Purpose:   adding a runner to a race
       in:   the race being added to
       in:   the runner
   result:   if it worked or not (0 worked)
*/
int addRunner(RaceInfoType *r, RunnerType *run){
  if (r->numRunners >= MAX_RUNNERS){
    return 1;
  }
  r->runners[r->numRunners] = run;
  r->numRunners++;
  return 0;
}

/*
  Function:  addDorc
  Purpose:   adding a dorc to a race
       in:   the race being added to
       in:   the entity being added
   result:   if it worked or not (0 worked)
*/
int addDorc(RaceInfoType *r, EntityType *en){
  if (r->numDorcs >= MAX_DORCS - 1){
    return 1;
  }
  r->dorcs[r->numDorcs] = en;
  r->numDorcs++;
  return 0;
}

/*
  Function:  drawHealth
  Purpose:   draw the health of runners
       in:   the race
   result:
*/
void drawHealth(RaceInfoType *r){
  for (int i = 0; i < (*r).numRunners; i++){
    char str[2];
    sprintf(str, "%d", (*r).runners[i]->health);
    scrPrt("  ", HEALTH_ROW + 2, HEALTH_COL + 10 + (i*4));
    scrPrt(str, HEALTH_ROW + 2, HEALTH_COL + 10 + (i*4));
  }
}

/*
  Function:  writeStatus
  Purpose:   writing a status message
       in:   the race
       in:   the message
   result:
*/
void writeStatus(RaceInfoType *r, char* message){
  scrPrt(message, r->statusRow, STATUS_COL);
  r->statusRow++;
}

/*
  Function:  freeRace
  Purpose:   freeing all memory in race
   result:
*/
void freeRace(){
  for (int i = 0; i < race->numRunners; i++){
    free(race->runners[i]);
  }
  for (int i = 0; i < race->numDorcs; i++){
    free(race->dorcs[i]);
  }
  free(race);
}

/*
  Function:  isDone
  Purpose:   check if the race is done
       in:   the 
   result:
*/
bool isDone(RaceInfoType *r){
  for (int i = 0; i < r->numRunners; i++){
    if (r->runners[i]->dead == 0 && r->runners[i]->ent.currPos > END_POS){
      return false;
    }
  }
  return true;
}
