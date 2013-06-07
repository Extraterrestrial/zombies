#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
//#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#else
#ifdef __linux
#include <unistd.h>
#define Sleep(time) usleep((time)*1000)
#endif
#endif


#define NAMES_SIZE 57

char * names[] = { "Amazon", "Assassin", "Sorceress",
		   "Paladin", "Necromancer", "Druid",
		   "Barbarian", "Akara", "Warriv",
		   "Anya", "Larzuk", "Natalya",
		   "Asheara", "Charsi", "Tyrael",
		   "Deckard", "Flavie", "Gheed",
		   "Kashya", "Atma", "Drognan",
		   "Elzix", "Fara", "Geglash",
		   "Greiz", "Jehryn", "Kaelan",
		   "Lysander", "Meshif", "Ormus",
		   "Alkor", "Hratli", "Halbu",
		   "Jamella", "Malah", "Nihlatak",
		   "Qual-Kehk", "Diablo", "Mephisto",
		   "Baal", "Duriel", "Radament",
		   "Andariel", "Rakanishu", "Torum",
		   "Negative", "Adrn", "Firk",
		   "Obsessed", "Grin", "Xardas",
		   "Plain", "Muny", "Archeg",
		   "Myatko", "Irknet", "Aler"
		 };
char ** zombie = NULL;
int zombie_size = 0;

pthread_t pthread;
pthread_attr_t attr;

int killme, score;

int zomb_time = 3000;
int wave = 0;



void zombies_lookout()
{
  int i = 0;
  
  for(i = 0; i < zombie_size; i++)
  printf("Zombie \"%s\".\n", zombie[i]);

  return;
}

int zombies_check_corpses()
{
  int i, corpse_count;

  for(i = 0; i < zombie_size; i++) {
    if(!strcmp(zombie[i], "_CORPSE"))
      corpse_count++;
  }
  return corpse_count;
}

int zombies_reload_ammo(int ammo)
{
  printf("Reloading shotgun...\n");
  Sleep(3000);
  ammo = 7;
  return ammo;
}
int zombies_start()
{
  int i = 0;

  srand(time(NULL));

  wave++;
  zombie_size += 5;

  /* Generating array for zombies */
  zombie = malloc(sizeof(char *) * zombie_size);

  /* fill array with zombie names */
  for(i = 0; i < zombie_size; i++) {
    zombie[i] = strdup(names[rand() % NAMES_SIZE]);
  }

  printf("Wave %d, get ready...\n", wave);
  Sleep(3000);
  zombies_lookout();

  for(i = 0; i < zombie_size; i++) {
    if(strcmp(zombie[i], "_CORPSE")) {
      printf("Zombie \"%s\" is nearby, shoot them NOW!!!\n", zombie[i]);
      Sleep(zomb_time);
    }
    else {
      Sleep(zomb_time);
      if(zombies_check_corpses() == zombie_size)
        return 0;
    }
    if(strcmp(zombie[i], "_CORPSE")) {
      printf("\nZombie \"%s\" kill you!\n", zombie[i]);
      printf("You was eaten by zombies.\n");
      printf("Your dead of valor will be remembered.\n");

      killme = 1;
      printf("Your score: %d zombies.\n", score);
      
      /* shutdown */
      free(zombie);
      exit(0);
    }
  }

  if(wave % 5 == 0) {
    if(zomb_time > 1300)
      zomb_time-=200;
    zombie_size = 0;
  }

  return 0;
}

static void * threadfunc(void * arg)
{
  while(1) {
    zombies_start();
    
    if(killme == 1)
      break;
  }
}

int shoot()
{
  char * target = malloc(255);
  int i = 0;
  static char ammo = 7;

  Sleep(1);
  printf("Target: ");
	scanf("%s", target);

	if(!strcmp(target, "l")) {
	  zombies_lookout();
	  return 1;
  }

  else if(!strcmp(target, "a")) {
  	printf("Ammo: %d/7\n", ammo);
  	return 1;
  }

  else if(!strcmp(target, "r")) {
    ammo = zombies_reload_ammo(ammo);
    return 1;
  }

  if(ammo == 0) {
    printf("You have no ammo in the shotgun.\n");
    return 1;
  }

  else if(!strcmp(target, "_CORPSE")) {
    printf("You can't kill corpse, lol\n");
    ammo--;
    return 1;
  }

  printf("Player shoots with good shotgun...\n");
  ammo--;
  for(i = 0; i < zombie_size; i++) {
    if(!strcmp(zombie[i], target)) {
      strcpy(zombie[i], "_CORPSE");
      printf("Zombie killed!\n\n");
      score++;
      return 0;
    }
  }
  printf("You are missed, zombie is still alive...\n");
  return 1;

}

int main()
{
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  Sleep(1);
  pthread_create(&pthread, &attr, threadfunc, NULL);
  while(1) {
    shoot();
  }
  return 0;
}
