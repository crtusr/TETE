#ifndef DEBUG_H
#define DEBUG_H

#define CHECKINT(a) mvprintw(0, 0, "%d", a); getch()
#define CHECKCHAR(a) mvprintw(0, 0, "%c", a); getch()
#define CHECKINTX(a) mvprintw(0, 0, "%x", a); getch()
#define CHECKCHARX(a) mvprintw(0, 0, "%x", a); getch()
#define CHECKSTR(a,b) move(0, 0); for(int asd = 0; asd < b; asd++) {printw("%c ", a[asd]);} getch()
#define CHECKSTRX(a,b) move(0, 0); for(int asd = 0; asd < b; asd++) {printw("%x ", a[asd]);} getch()
#define CHECKSTACK(ptr,b) move(0, 0); for(int asd = 0; asd < b; asd++) {printw("%c ", *(ptr+asd));} getch()
#define CHECKSTACKX(ptr,b) move(0, 0); for(int asd = 0; asd < b; asd++) {printw("%x ", *(ptr+asd));} getch()
  
#endif

