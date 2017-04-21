/*
 * Yash Bhutwala
 * CS 315 Lab 12
 * 12/1/2015
 */

#include "yashShell.h"

int main(int argc, const char *argv[]) {
  int enter_cnt = 0;

  while (1) {
    printf("yashShell> ");
    fflush(stdout);
    char *cmd = calloc(CMD_LEN, sizeof(char));
    cmd = fgets(cmd, CMD_LEN, stdin);
    char dest[3] = "";
    strncpy(dest, cmd, 2);
    dest[2] = '\0';

    if (equals(cmd, EXIT)) {
      break;
    }

    if (equals(cmd, ENTER)) {
      if (0 == enter_cnt) {
        enter_cnt = 1;
        continue;
      } else {
        enter_cnt = 0;
        cmd[0] = '\0';
        cmd[0] = 'l';
        cmd[1] = 's';
      }
    }

    if (equals(cmd, HISTORY)) {
      history(NULL);
      cmd[0] = '0';
    } else {
      history(cmd); // store the command in history
    }

    if (equals(cmd, DATE)) {
      get_date();
      continue;
    }

    if (equals(dest, CD)) {
      change_dir(cmd + 3);
      continue;
    }

    tokenize_and_exec(cmd);
  }
  return 0;
}

void tokenize_and_exec(char *cmd) {
  int i, j;
  int status1, status2, status3;
  pid_t pid, pid1, pid2;
  char **toks;
  char **toks2; // tokens for executing command

  char *arg_tok, *arg_buf;
  char *cmd_tok, *cmd_buf;

  int pipeFound;
  int flag;
  int pipefd[2];

  cmd_tok = strtok_r(cmd, CMD_SEPARATOR, &cmd_buf);
  while (cmd_tok != NULL) {
    // printf("cmd_tok %s\n", cmd_tok);
    i = 0;
    j = 0;
    pipeFound = 0;
    flag = 0;
    toks = calloc(ARGS_LEN, sizeof(char *));
    toks2 = calloc(ARGS_LEN, sizeof(char *));

    arg_tok = strtok_r(cmd_tok, ARG_SEPARATOR, &arg_buf);
    while (arg_tok != NULL) {
      // printf("arg_tok: %s\n", arg_tok);
      if (strcmp(arg_tok, "|") == 0) {
        pipeFound = 1;
      }
      if (1 == pipeFound) {
        if (1 == flag) {
          toks2[j] = arg_tok;
          j++;
        } else {
          flag = 1;
        }
      } else {
        toks[i] = arg_tok;
        i++;
      }
      arg_tok = strtok_r(NULL, ARG_SEPARATOR, &arg_buf);
    }

    if (equals(toks[0], CD)) {
      change_dir(toks[1]);
      cmd_tok = strtok_r(NULL, CMD_SEPARATOR, &cmd_buf);
      continue;
    }

    if (1 == pipeFound) {
      if (-1 == pipe(pipefd)) {
        printf("Pipe failed\n");
      }
      pid1 = fork();
      if (pid1 == 0) {
        /* I am the child doing the left side of the pipe */
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        execvp(toks[0], toks);
      } else {
        pid2 = fork();
        if (pid2 == 0) {
          /* I am the child doing the right side of the pipe */
          close(pipefd[1]);
          dup2(pipefd[0], 0);
          execvp(toks2[0], toks2);
        } else {
          /* I am the parent */
          close(pipefd[0]);
          close(pipefd[1]);
          waitpid(pid1, &status1, 0);
          free(toks2);
        }
      }
      waitpid(pid2, &status2, 0);
      if (0 == WEXITSTATUS(status2)) {
        printf("[yashShell: program terminated successfully]\n");
      } else {
        printf("[yashShell: program terminated abnormally] [%d]\n",
               WEXITSTATUS(status2));
      }
    } else {
      // Fork off and have child execute first command
      pid = fork();
      if (!pid) {
        execvp(toks[0], toks);
      } else {
        wait(&status3);
        if (0 == WEXITSTATUS(status3)) {
          printf("[yashShell: program terminated successfully]\n");
        } else {
          printf("[yashShell: program terminated abnormally] [%d]\n",
                 WEXITSTATUS(status3));
        }
        free(toks);
      }
    }
    cmd_tok = strtok_r(NULL, CMD_SEPARATOR, &cmd_buf);
  }
}

int equals(const char *cmd, const char *other) {
  return strncmp(other, cmd, strlen(other)) ? 0 : 1;
}

void history(const char *cmd) {
  static int pos = -1;
  static int count = 1;
  static struct hist_item *history_list;

  if (pos == -1)
    history_list = calloc(HIST_LEN, sizeof(struct hist_item));

  if (cmd) {
    pos = (pos + 1) % HIST_LEN;
    history_list[pos].index = count;
    strncpy(history_list[pos].item, cmd, strlen(cmd));
    count++;
  } else {
    printf("\nHISTORY: Last %d items\n", HIST_LEN);
    int i;
    int items = 0;
    while (items < HIST_LEN) {
      i = (pos + items + 1) % HIST_LEN;
      if (history_list[i].index != 0)
        printf("%3d: %s", history_list[i].index, history_list[i].item);
      items++;
    }

    printf("\n");
  }
}

void get_date(void) {
  struct timeval tv;
  time_t t;
  int rc;
  char *rv;

  rc = gettimeofday(&tv, NULL);
  if (rc) {
    perror(
        "[yashShell: program terminated abnormally]: [gettimeofday failed]\n");
  }
  t = tv.tv_sec;
  rv = ctime(&t);
  if (NULL == rv) {
    perror("[yashShell: program terminated abnormally]: [ctime failed]\n");
  }
  printf("%s", rv);
  printf("[yashShell: program terminated successfully]\n");
  fflush(stdout);
}

void change_dir(char *path) {
  char *homedir = getenv("HOME");
  // char* currdir = getenv("PATH");
  char *lastdir = getenv("OLDPWD");

  char *my_path, *my_path_buf;
  my_path = strtok_r(path, ARG_SEPARATOR, &my_path_buf);

  int ret_stat;
  if (NULL == my_path || equals(my_path, "~")) {
    ret_stat = chdir(homedir);
  } else if (equals(my_path, "-")) {
    ret_stat = chdir(lastdir);
  } else {
    ret_stat = chdir(my_path);
  }

  if (-1 == ret_stat) {
    printf("[yashShell: program terminated abnormally] [%d]\n", ret_stat);
  } else {
    printf("[yashShell: program terminated successfully]\n");
  }
}
