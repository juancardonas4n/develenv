#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <filesystem>

const int  BUFFER_SIZE = 80;
const char GNOME_TERMINAL_PATH[] = "/usr/bin/gnome-terminal";
const char PTYXIS_TERMINAL_PATH[] = "/usr/bin/ptyxis";
const char XFCE_TERMINAL_PATH[] = "/usr/bin/xfce4-terminal";
const char GNOME_TERMINAL[] = "gnome-terminal";
const char PTYXIS_TERMINAL[] = "ptyxis";
const char XFCE_TERMINAL[] = "xfce4-terminal";
const char GNOME_FULL_SCREEN_OPTION[] = "--full-screen";
const char XFCE_FULL_SCREEN_OPTION[] = "--fullscreen";
const char GNOME_PROFILE_OPTION[] = "--profile=devel";

bool existTmuxSession(const char* sname);
bool isGnomeInstalled();
bool isPtyxisInstalled();
bool isPathInstalled(const char*);
void launchGnomeTerminal();
void launchAndAttachGnomeTerminal();

int
main(void) {

  pid_t child;

  if (existTmuxSession("devel:")) {

    child = fork();
    if (child == 0) {
      std::string currdesktop{::getenv("XDG_CURRENT_DESKTOP")};
      if (currdesktop == "GNOME" || currdesktop == "ubuntu:GNOME") {
        // execl(GNOME_TERMINAL_PATH,
        //       GNOME_TERMINAL,
        //       GNOME_FULL_SCREEN_OPTION,
        //       GNOME_PROFILE_OPTION,
        //       "--",
        //       "tmux",
        //       "attach",
        //       "-t",
        //       "devel",
        //       NULL);
	launchAndAttachGnomeTerminal();
        exit(1);
      }
      else if (currdesktop == "XFCE") {
        execl(XFCE_TERMINAL_PATH,
              XFCE_TERMINAL,
              "-e",
              "tmux",
              "attach",
              "-t",
              "devel",
              XFCE_FULL_SCREEN_OPTION,
              NULL);
      }
    }
  }
  else {

    child = fork();

    if (child == 0) {
      
      std::string currdesktop(::getenv("XDG_CURRENT_DESKTOP"));
      
      if (currdesktop == "GNOME" ||
	  currdesktop == "ubuntu:GNOME") {
	launchGnomeTerminal();
        exit(1);
      }
      else if (currdesktop == "XFCE") {
        execl(XFCE_TERMINAL_PATH,
              XFCE_TERMINAL,
              XFCE_FULL_SCREEN_OPTION,
              "--font=\"Monospace Regular 12\"",
              "-e",
              "tmux",
              "new",
              "-s",
              "devel",
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

    sscanf(p, "%s", pFirst);

    if (::strcmp(pFirst, sname) == 0) return true;
  } while (true);

  return false;
}

bool
isPtyxisInstalled() {
  return isPathInstalled(PTYXIS_TERMINAL_PATH);
}

bool
isGnomeInstalled() {
  return isPathInstalled(GNOME_TERMINAL_PATH);
}


bool isPathInstalled(const char* cstr_path) {
  return std::filesystem::status(cstr_path).type() ==
    std::filesystem::file_type::regular;
}

void launchGnomeTerminal() {
  if (isGnomeInstalled()) {
    execl(GNOME_TERMINAL_PATH,
	  GNOME_TERMINAL,
	  GNOME_FULL_SCREEN_OPTION,
	  GNOME_PROFILE_OPTION,
	  "--",
	  "tmux",
	  "new",
	  "-s",
	  "devel",
	  NULL);
  }
  else if (isPtyxisInstalled()) {
    execl(PTYXIS_TERMINAL_PATH,
	  PTYXIS_TERMINAL,
	  "--new-window",
	  "--tab-with-profile=devel",
	  "-x",
	  "tmux",
	  "new",
	  "-s",
	  "devel",
	  NULL);
  }
}

void launchAndAttachGnomeTerminal() {
  if (isGnomeInstalled()) {
    execl(GNOME_TERMINAL_PATH,
	  GNOME_TERMINAL,
	  GNOME_FULL_SCREEN_OPTION,
	  GNOME_PROFILE_OPTION,
	  "--",
	  "tmux",
	  "attach",
	  "-t",
	  "devel",
	  NULL);
  }
  else if (isPtyxisInstalled()) {
    execl(PTYXIS_TERMINAL_PATH,
	  PTYXIS_TERMINAL,
	  "--new-window",
	  "--tab-with-profile=devel",
	  "-x",
	  "tmux",
	  "attach",
	  "-t",
	  "devel",
	  NULL);
  }
}
