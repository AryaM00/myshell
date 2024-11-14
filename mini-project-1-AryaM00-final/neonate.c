#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <termios.h>
#include <signal.h>
#include <sys/select.h>

struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void enable_raw_mode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);  // Ensure raw mode is disabled when the program exits

    raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode (line buffering)
    raw.c_cc[VMIN] = 0;  // Set to non-blocking mode
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

pid_t get_most_recent_pid() {
    DIR *proc_dir;
    struct dirent *entry;
    struct stat statbuf;
    time_t latest_time = 0;
    pid_t latest_pid = 0;

    proc_dir = opendir("/proc");
    if (!proc_dir) {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(proc_dir)) != NULL) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) > 0) {  // Check if it's a PID directory
            char path[4096];
            snprintf(path, sizeof(path), "/proc/%s", entry->d_name);
            if (stat(path, &statbuf) == 0) {
                if (statbuf.st_mtime > latest_time) {
                    latest_time = statbuf.st_mtime;
                    latest_pid = atoi(entry->d_name);
                }
            }
        }
    }

    closedir(proc_dir);
    return latest_pid;
}

void neonate(char *command) {
    // Parse the command
    char *token = (char *)malloc(1000 * sizeof(char));
    char words[100][1000];
    int word_count = 0;

    while ((token = strtok_r(command, " \t\n", &command))) {
        strcpy(words[word_count], token);
        word_count++;
    }

    if (word_count != 3) {
        printf("Please enter a valid command\n");
        return;
    }

    if (strcmp(words[0], "neonate") != 0) {
        printf("Please enter a valid command\n");
        return;
    }

    if (strcmp(words[1], "-n") != 0) {
        printf("Please enter a valid command\n");
        return;
    }

    int time_arg = atoi(words[2]);
    if (time_arg <= 0) {
        printf("Please enter a valid time argument\n");
        return;
    }

    // Enable raw mode to capture keystrokes without blocking
    enable_raw_mode();

    struct timeval tv;
    fd_set readfds;

    while (1) {
        // Set up the timeout interval for `select`
        tv.tv_sec = time_arg;  // Sleep interval
        tv.tv_usec = 0;

        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        // Wait for input or timeout
        int retval = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror("select()");
            break;
        } else if (retval) {
            // Input is available, check for 'x'
            char ch;
            read(STDIN_FILENO, &ch, 1);
            if (ch == 'x') {
                break;  // Exit immediately when 'x' is pressed
            }
        } else {
            // Timeout occurred, retrieve most recent PID
            pid_t recent_pid = get_most_recent_pid();
            if (recent_pid > 0) {
                printf("Most recent PID: %d\n", recent_pid);
            } else {
                printf("Error retrieving PID.\n");
            }
        }
    }

    disable_raw_mode();
}
