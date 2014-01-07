#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "print.h"
#include "DirHandle.h"
#include "SDFileSystem.h"
#include "util.h"
#include "Buttons.h"

#define MAXBUF		128
#define MAXCMDARR	19

extern Serial pc;
extern Buttons keypad;

char cwd[MAXBUF];
char buf[MAXBUF];
int status;
bool debug=false;
bool done=false;

typedef struct {
	const char *cmd;
	int (*f)(const char *arg0);
	const char *desc;
} cmd;

extern int autonomousMode();
extern int resetMe();
extern int gyroSwing();
extern int compassSwing();
extern void displayData(int mode);

extern "C" {

void shell(void *args);
void docmd(char *cmdline);
void termInput(char *cmd);
void resolveDirectory(char *newpath, char *path);
void splitName(const char *path, char *dirname, char *basename);
int dols(const char *path);
int docd(const char *path);
int dopwd(const char *s);
int dotouch(const char *path);
int domkdir(const char *path);
int dohead(const char *path);
int docat(const char *path);
int dosend(const char *path);
int doprintfree(const char *path);
int doexit(const char *s);
int dodebug(const char *s);
int dohelp(const char *s);
int doinstrchk(const char *s);
int docompswing(const char *s);
int dogyroswing(const char *s);
int doreset(const char *s);
int doautonomous(const char *s);

const cmd command[MAXCMDARR] = {
    	{ "help", dohelp, "print this help" },
		{ "ls", dols, "list files" },
    	{ "cd", docd, "change directory" },
    	{ "pwd", dopwd, "print working directory" },
    	{ "touch", dotouch, "create, update file" },
    	{ "mkdir", domkdir, "make directory" },
    	{ "head", dohead, "output first part of file" },
    	{ "cat", docat, "output file" },
    	{ "send", dosend, "send file to terminal" },
    	{ "rm", remove, "remove file" },
    	{ "debug", dodebug, "toggle debug mode" },
    	{ "stat", doinstrchk, "instrument check" },
    	{ "comp", docompswing, "compass swing" },
    	{ "gyro", dogyroswing, "gyro swing" },
    	{ "auto", doautonomous, "run autonomous mode" },
    	{ "reset", doreset, "reset the MCU" },
    	{ "free", doprintfree, "heap bytes available" },
//    	{ "exit", doexit, "exit shell" },
    	{ 0, 0, 0 }
};

extern void checkit(const char *s, const int l);
extern "C" size_t xPortGetFreeHeapSize(void);

void shell(void *args) {
    char cmdline[MAXBUF];

    pc.baud(115200);

    strcpy(cwd, "/log");

    fputs("Type help for assistance.\n", stdout);

    status=0;
    done=false;
    while (!done) {
        termInput(cmdline);

        // interrupt operation if keypad button is pressed
        if (keypad.pressed) {
            keypad.pressed = false;
            break;
        }        

        docmd(cmdline);
	}
    fputs("exiting shell\n", stdout);

    return;
}


/** docmd
 * Run a command by looking it up the command requested on the shell command line in the
 * command array.  If it's found, run the associated function. If not, print an error.
 */
void docmd(char *cmdline) {
	char *arg;
	char cmd[MAXBUF];
	char newpath[MAXBUF];
	bool found = false;

    arg = split(cmd, cmdline, MAXBUF, ' ');
    if (strlen(cmd) > 0) {
		resolveDirectory(newpath, arg);

		if (debug) fprintf(stdout, "cmdline:<%s> cmd:<%s> arg:<%s> newpath:<%s>\n", cmdline, cmd, arg, newpath);

		for (int i=0; command[i].cmd; i++) {
			if (!strcmp(cmd, command[i].cmd)) {
				found = true;
				command[i].f(newpath);
			}
		}

		if (!found) {
			fputs(cmd, stdout);
			fputs(": command not found\n", stdout);
		}
    }

	return;
}


/** termInput
 * read input from the terminal
 */
void termInput(char *cmd) {
    int i=0;
    char c;
    bool done = false;
    
    memset(cmd, 0, MAXBUF);
    fputc('(', stdout);
    fputs(cwd, stdout);
    fputs(")# ", stdout);
    do {
    	cmd[i] = 0;
        c = fgetc(stdin);
        if (c == '\r') { // if return is hit, we're done, don't add \r to cmd
            done = true;
        } else if (i < MAXBUF-1) {
            if (c == 0x7f || c == '\b') { // backspace or delete
                if (i > 0) { // if we're at the beginning, do nothing
                    i--;
                    fputs("\b \b", stdout);

                }
            } else {
                fputc(c, stdout);
                cmd[i++] = c;

            }
        }
    } while (!done);
    fputc('\n', stdout);


} 

/** resolveDirectory
 * resolve the directory path provided, given the cwd
 */
void resolveDirectory(char *newpath, char *path) {
    char dirname[32], basename[16];
    
    /** absolute path */
    if (path[0] == '/') {
        strcpy(newpath, path);
    }
    /** relative path */
    else {
        strcpy(newpath, cwd);
        if (path[0] != 0) {
            if (newpath[strlen(newpath)-1] != '/')
                strcat(newpath, "/");
            strcat(newpath, path);    
        }
        /** Resolve .. references */
        splitName(newpath, dirname, basename);
        if (!strcmp(basename, "..")) {
            splitName(dirname, newpath, basename);
        }
    }
}

/** splitCmd
 * copy t to s until space is reached
 * return location of delimiter+1 in t
 * if space not found, return t pointing to end of string
 * if s or t null, return null
 */
char *splitCmd(char *s, char *t, int max)
{
  int i = 0;
  
  if (s == 0 || t == 0)
    return 0;

  while (*t != 0 && i < max) {
    *s++ = *t++;
    i++;
    if (*t == ' ') {
        t++;
        break;
    }
  }
  *s = 0;

  checkit(__FILE__, __LINE__);

  return t;
}

/** splitName
 * split the path into a dirname and a 
 */
void splitName(const char *path, char *dirname, char *basename) {
    int sep;
    
    sep = 0;
    if (debug) fprintf(stdout, "%d\n", strlen(path));
    for (int i=strlen(path)-1; i >= 0; i--) {
        if (debug) fprintf(stdout, "- %c\n", path[i]);
        sep = i;
        if (path[i] == '/') break;
    }
    for (int j=0; j < sep; j++) {
        if (debug) fprintf(stdout, "> %c\n", path[j]);
        dirname[j] = path[j];
        dirname[j+1] = 0;
    }
    for (unsigned int k=sep+1; k != strlen(path); k++) {
        if (debug) fprintf(stdout, "* %c\n", path[k]);
        basename[k-(sep+1)] = path[k];
        basename[k-sep] = 0;    
    }
    if (debug) fprintf(stdout, "d:<%s> b:<%s>\n", dirname, basename);


}

/** ls
 * lists files in the current working directory, 4 columns
 */
int dols(const char *path) {
    if (debug) fprintf(stdout, "%s\n", cwd);
    DIR *d;
    struct dirent *p;

    int count=0;
    if ((d = opendir(path)) != NULL) {
        while ((p = readdir(d)) != NULL) {
        	int pad = 14 - strlen(p->d_name);
        	while (pad--) fputc(' ', stdout);
        	fputs(p->d_name, stdout);
            if (count++ >= 3) {
                count = 0;
                fputc('\n', stdout);
            }
        }
        fputc('\n', stdout);
        if (count < 3)
            fputc('\n', stdout);
        closedir(d);
        status = 0;
    } else {
    	fputs(path, stdout);
    	fputs(": No such directory\n", stdout);
        status = 1;
    }

    return status;
}

/** cd
 * changes current working directory
 */
int docd(const char *path) {
    strcpy(cwd, path);

    return 0;
}

/** pwd
 * print current working directory
 */
int dopwd(const char *s) {
	fputs(cwd, stdout);
	fputc('\n', stdout);
    //fprintf(stdout, "%s\n", cwd);

    return 0;
}

/** touch
 * create an empty file
 */
int dotouch(const char *path) {
    FILE *fp;
    if ((fp = fopen(path, "w")) != NULL) {
        fclose(fp);
        status = 0;
    } else {
    	fputs(path, stdout);
    	fputs(": No such file\n", stdout);
        status = 1;
    }

    return status;
} 

int domkdir(const char *path) {
	return mkdir(path, S_IRWXU|S_IRWXG|S_IRWXO);
}

/** head
 * print the first 10 lines of a file
 */
int dohead(const char *path) {
    FILE *fp;
    char line = 0;
    
    if ((fp = fopen(path, "r")) != NULL) {
        while (!feof(fp) && line++ < 10) {
            fgets(buf, 128, fp);
            fputs(buf, stdout);
        }
        fclose(fp);
        status = 0;
    } else {
    	fputs(path, stdout);
    	fputs(": No such file\n", stdout);
        status = 1;
    }

    return status;
}

/** cat
 * display the content of a file
 */
int docat(const char *path) {
    FILE *fp;

    if ((fp = fopen(path, "r")) != NULL) {
        while (fgets(buf, 127, fp)) {
			fputs(buf, stdout);
        }
        fclose(fp);
        status = 0;
    } else {
    	fputs(path, stdout);
    	fputs(": No such file\n", stdout);
        status = 1;
    }

    return status;
}

/** send
 * Simple serial file transfer protocol
 * Initiates escape sequence: ^A^B, sends filename, ^C, and then file
 * contents followed by ^D
 */
int dosend(const char *path) {
    FILE *fp;
    char dirname[32], basename[16];

    if ((fp = fopen(path, "r")) != NULL) {
        splitName(path, dirname, basename);
        fputc(0x01, stdout);
        fputc(0x02, stdout);
        fputs(basename, stdout);
        fputc(0x03, stdout);
        while (!feof(fp)) {
            if (fgets(buf, 127, fp) != NULL)
                fputs(buf, stdout);
        }
        fclose(fp);
        fputc(0x04, stdout);
        status = 0;
    } else {
    	fputs(path, stdout);
    	fputs(": No such file\n", stdout);
        status = 1;
    }

    return status;
}

int doprintfree(const char *s) {
	printInt(stdout, xPortGetFreeHeapSize());
	fputs(" bytes free.\n", stdout);

	return 0;
}

/** doexit
 * set a flag to exit the shell
 */
int doexit(const char *s) {
	done = true;

	return 0;
}

/** dodebug
 * toggle the debug state variable
 */
int dodebug(const char *s) {
	debug = !debug;

	return 0;
}

/** dohelp
 * print the list of commands and descriptions
 */
int dohelp(const char *s) {
	for (int i=0; command[i].cmd; i++) {
		int pad = 10 - strlen(command[i].cmd);
		while (pad--) fputc(' ', stdout);
		fputs(command[i].cmd, stdout);
		fputs(": ", stdout);
		fputs(command[i].desc, stdout);
		fputc('\n', stdout);
	}

	return 0;
}

/** doinstrchk
 * call external instrument check routine
 */
int doinstrchk(const char *s) {
	displayData(0);

	return 0;
}

/** docompswing
 * perform compass swing, call external function
 */
int docompswing(const char *s) {
	compassSwing();

	return 0;
}

/** dogyroswing
 * perform gyro swing, call external function
 */
int dogyroswing(const char *s) {
	gyroSwing();

	return 0;
}

/** doreset
 * reset the processor
 */
int doreset(const char *s) {
	resetMe();
	return 0; // won't ever reach this line...
}

/** doautonomous
 * call external doAutonomous mode to perform an autonomous run
 */
int doautonomous(const char *s) {
	autonomousMode();

	return 0;
}

}
