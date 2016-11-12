#ifndef USERTHREAD_H
#define USERTHREAD_H

#ifdef USER_PROGRAM
#ifdef CHANGED
typedef struct ThreadArgs ThreadArgs;
extern void StartUserThread(void *);
extern int do_ThreadCreate(int f, int arg);
extern void do_ThreadExit();

#endif //CHANGED

#endif // USER_PROGRAM

#endif /* USERTHREAD_H */
