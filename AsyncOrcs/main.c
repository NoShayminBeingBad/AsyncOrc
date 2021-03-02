#include "defs.h"

int main(){
  if (initRaceInfo(STATUS_ROW, &race) < 0) {
    printf("semaphore initialization error\n");
    exit(1);
  }

  RunnerType *temp;

  initRunner("Timmy", MAX_HEALTH, "T", START_POS, PATH_1, &temp);
  addRunner(race, temp);
  initRunner("Harold", MAX_HEALTH, "H", START_POS, PATH_2, &temp);
  addRunner(race, temp);

  srand((unsigned)time(NULL));
  initNcurses();

  scrPrt("--------------------", 1, 0);
  scrPrt("--------------------", START_POS + 1, 0);

  scrPrt("Health:", HEALTH_ROW, HEALTH_COL);
  for (int i = 0; i < race->numRunners; i++){
    scrPrt(race->runners[i]->ent.avatar, HEALTH_ROW, HEALTH_COL + 11 + (i*4));
  }
  drawHealth(race);

  for (int i = 0; i < race->numRunners;i++){
    pthread_create(&(race->runners[i]->ent.thr), NULL, goRunner, race->runners[i]);
  }

  while (!isDone(race)){
    if (randm(10) < 3){
      EntityType *temp;
      initDorc(&temp);
      if (addDorc(race, temp) == 0){
        pthread_create(&(temp->thr), NULL, goDorc, temp);
      }
    }
    usleep(250000);
  }

  for (int i = 0; i < race->numRunners;i++){
    pthread_join(race->runners[i]->ent.thr, NULL);
  }

  for (int i = 0; i < race->numDorcs; i++){
    pthread_cancel(race->dorcs[i]->thr);
  }

  char str[64];
  if (strcmp(race->winner, "") == 0){
    strcpy(str, "OUTCOME: All runners are dead");
  }else {
    sprintf(str, "OUTCOME: The winner is %s", race->winner);
  }
  writeStatus(race, str);

  freeRace();
  cleanupNcurses(START_POS + 2);
}
