#include "defs.h"

/*
  Function:  goRunner
  Purpose:   the loop the runner runs
       in:   the runner
   result:
*/
void *goRunner(void* arg){
  RunnerType *r = (RunnerType*)arg;

  while (r->ent.currPos > END_POS && !r->dead){
    int step = randm(10);
    int nextPos = r->ent.currPos;
    int sudoHealth = r->health;
    char str[64];
    if (step < 3){
      nextPos += step + 1;
      if (nextPos > START_POS){
        nextPos = START_POS;
      }
    }else {
      step = randm(4) + 1;
      nextPos -= step;
      sudoHealth -= step;
    }
    sem_wait(&(race->mutex));
    for (int i = 0; i < race->numDorcs; i++){
      if (race->dorcs[i]->currPos == r->ent.currPos && race->dorcs[i]->path == r->ent.path){
        sudoHealth -= 3;
        sprintf(str, "STATUS: collision between %s and dorc", r->name);
        writeStatus(race, str);
        break;
      }
    }
    if (sudoHealth <= 0){
      sudoHealth = 0;
      r->dead = 1;
      strcpy(r->ent.avatar, "+");
      sprintf(str, "STATUS: %s is dead", r->name);
      writeStatus(race, str);
    }
    if (nextPos < END_POS){
      nextPos = END_POS;
    }
    r->health = sudoHealth;
    scrPrt(" ", r->ent.currPos, r->ent.path);
    r->ent.currPos = nextPos;
    scrPrt(r->ent.avatar, r->ent.currPos, r->ent.path);
    drawHealth(race);

    if (r->dead == 0 && r->ent.currPos == END_POS && strcmp(race->winner, "") == 0){
      strcpy(race->winner, r->name);
    }
    sem_post(&(race->mutex));
    usleep(250000);
  }
}

/*
  Function:  goDorc
  Purpose:   the loop the dorc runs
       in:   the dorc
   result:
*/
void *goDorc(void *arg){
  EntityType *d = (EntityType*)arg;
  int col = d->path;
  int down = randm(5) + 1;
  while (d->currPos < START_POS){
    int nextStep = randm(2) * 2;
    if (randm(2) == 0){
      if (col - nextStep < PATH_1){
        col = PATH_1;
      }else {
        col -= nextStep;
      }
    }else {
      if (col + nextStep > PATH_1 + MAX_P_SPACE){
        col = PATH_1 + MAX_P_SPACE;
      }else {
        col += nextStep;
      }
    }
    if (d->currPos + down > START_POS){
      break;
    }
    sem_wait(&(race->mutex));
    scrPrt(" ", d->currPos, d->path);
    d->currPos += down;
    d->path = col;
    scrPrt(d->avatar, d->currPos, d->path);
    sem_post(&(race->mutex));
    usleep(700000);
  }
  sem_wait(&(race->mutex));
  scrPrt(" ", d->currPos, d->path);
  sem_post(&(race->mutex));
}
