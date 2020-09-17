#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>

const int  BUFFER_SIZE = 80;
const char GNOME_TERMINAL_PATH[] = "/usr/bin/gnome-terminal";
const char XFCE_TERMINAL_PATH[] = "/usr/bin/xfce4-terminal";
const char GNOME_TERMINAL[] = "gnome-terminal";
const char XFCE_TERMINAL[] = "xfce4-terminal";
const char GNOME_FULL_SCREEN_OPTION[] = "--full-screen";
const char XFCE_FULL_SCREEN_OPTION[] = "--fullscreen";

bool existTmuxSession(const char* sname);

int
main(void) {

  pid_t child;
  
  if (existTmuxSession("devel:")) {

    child = fork();
    if (child == 0) {
      std::string currdesktop{::getenv("XDG_CURRENT_DESKTOP")};
      if (currdesktop == "GNOME") {
	execl(GNOME_TERMINAL_PATH,
	      GNOME_TERMINAL,
	      "--command",
	      "tmux attach -t devel",
	      GNOME_FULL_SCREEN_OPTION,
	      NULL);
	exit(1);
      }
      else if (currdesktop == "XFCE") {
	execl(XFCE_TERMINAL_PATH,
	      XFCE_TERMINAL,
	      "-e",
	      "tmux attach -t devel",
	      XFCE_FULL_SCREEN_OPTION,
	      NULL);
      }
    }
  }
  else {
    
    child = fork();

    if (child == 0) {
      std::string currdesktop(::getenv("XDG_CURRENT_DESKTOP"));
      if (currdesktop == "GNOME") {
	execl(GNOME_TERMINAL_PATH,
	      GNOME_TERMINAL,
	      GNOME_FULL_SCREEN_OPTION,
	      "--window-with-profile",
	      "Desarrollo Programas",
	      NULL);
      }
      else if (currdesktop == "XFCE") {
	execl(XFCE_TERMINAL_PATH,
	      XFCE_TERMINAL,
	      XFCE_FULL_SCREEN_OPTION,
	      "--command=tmux",
	      // "--font=\"Monospace Regular 12\"",
	      NULL);
      }
      exit(1);
    }
  }
  exit(0);
}

bool existTmuxSession(const char* sname) {
  FILE* pIn = popen("tmux ls", "r");
  char p[BUFFER_SIZE];
  char pFirst[BUFFER_SIZE];
  do {

    if (fgets(p, BUFFER_SIZE, pIn) == NULL) break;

    // std::cout << p << std::endl;
    
    sscanf(p, "%s", pFirst);

    // std::cout << "pFirst: " << pFirst << std::endl;
    
    if (::strcmp(pFirst, sname) == 0) return true;
  } while (true);

  return false;
}
