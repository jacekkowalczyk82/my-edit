/* 
BSD 3-Clause License

Copyright (c) 2021, jacekkowalczyk82
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/ 
// This is the very first text editor I am trying to build using the C programming language 
// I was following the manual at https://viewsourcecode.org/snaptoken/kilo/
// possibly my version will slightly ifferent from the original kilo and from the version from manual
// I am "creating" it for learning 


/*** includes ***/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
#include <ctype.h>
#include <errno.h>

//#include <string.h>

/*** data ***/

struct termios orig_termios;


/*** terminal ***/

void die(const char *s) {
  perror(s);
  exit(1);
}


void disableRawMode() {
  //tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
    die("tcsetattr");
  }
}

void enableRawMode() {
  //tcgetattr(STDIN_FILENO, &orig_termios);
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) { 
    die("tcgetattr");
  }

  atexit(disableRawMode);
  struct termios raw = orig_termios;

  //https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;  

  //tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    die("tcsetattr");
  }
}


/*** init ***/

int main() {
  enableRawMode();

  while (1) {
    char c = '\0';
  
    //read(STDIN_FILENO, &c, 1);
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      die("read");    
    }

    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    
    if (c == 'q') {
      break;
    }
  }
  
  return 0;
}



