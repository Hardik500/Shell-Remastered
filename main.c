#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <regex.h>
#include <limits.h>

//variables and fuctions required for list command
struct stat s;

//Declaring Current Working Directory
char *getcwd(char *buf, size_t size);

//Regular expression for getting all the files which follows the syntax * or **
regex_t regex;
const char *expression = "^[.]*$";
int reti;

//File pointer for storing the name and content of copy, paste function
FILE *fptr1;
char filename[100];
int charData;
char filearray[1000][1000];
char fileNamearray[1000][1000];
int counter = 0;

//Directory listing main loop
int listDirectory(int counter, int level, char arr[200], char space[100])
{
  // printf("Here Counter = %d and Level = %d and directory is %s\n", counter, level, arr);
  printf("%s|_%s\n", space, arr);

  //Character for seperating each directories or files
  char c = '/';

  //Get the length of the current directory
  size_t len = strlen(arr);

  //Declare a new string with size of the root + that of the char c
  char *modifiedRootDir = malloc(len + 1 + 1);

  //Copy the name of cuurentDirectory in new String
  strcpy(modifiedRootDir, arr);

  modifiedRootDir[len] = c;
  modifiedRootDir[len + 1] = '\0';

  //Check if the counter is equal to or greater than level. If yes exit
  if (counter >= level)
  {
    return 0;
  }

  //Store the pointer to the currentDirectory in a new variable
  char *currentDirctory = modifiedRootDir;

  // Pointer for directory entry
  struct dirent *de;

  // opendir() returns a pointer of DIR type.
  DIR *dr = opendir(currentDirctory);

  // opendir returns NULL if couldn't open directory i.e. it is a file
  if (dr == NULL)
  {
    // printf("\nCould not open current directory\n");
    return 0;
  }

  //Declare root variable
  char root[100];

  //Copy
  strcpy(root, modifiedRootDir);

  char newSpace[200];
  char lengthOfSpace[100] = "  ";
  strcat(newSpace, space);
  strcat(newSpace, lengthOfSpace);

  //Read the contents of the directory
  while ((de = readdir(dr)) != NULL)
  {
    int size = 0;

    struct stat statbuf;

    //The name of each file or folder
    char *x = de->d_name;
    char c2[100];

    strcpy(c2, x);

    strcpy(currentDirctory, c2);

    //Replace the content of arr with that of new directory or file name
    reti = regexec(&regex, currentDirctory, 0, NULL, 0);

    /*Check if the file doesn't match . or .. which is the parent and grand-parent directory*/
    if (!reti)
    {
      //If it matches check for next file
      continue;
    }
    else if (reti == REG_NOMATCH)
    {

      //Check if the file or the path is a direcfory
      stat(arr, &statbuf);

      /*If it is a directory then increase the counter and size and put the directory in loop for next call*/
      counter += 1;

      size_t lengthOfRoot = strlen(root);
      size_t lengthOfCurrentDir = strlen(currentDirctory);

      /*Set the length of the new directory which is equal to the length of the previous root directory*/
      /*as well as the length of the new directory that is found*/
      char *newDir = malloc(lengthOfRoot + lengthOfCurrentDir + 1);

      //Copy the contents of the root directory in a new directory
      strcpy(newDir, root);

      //Concat the root directory and new directory
      strcat(newDir, currentDirctory);

      /*At the end of the string set a null character which specifies the end of the string*/
      newDir[lengthOfCurrentDir + lengthOfRoot + 1] = '\0';

      //Go into loop with new counter, directory name, and new space
      listDirectory(counter, level, newDir, newSpace);

      /*Increase the value of the size which store the number of times the loops occurs*/
      size += 1;

      //Decrease the size of the counter equal to times the loops occurs
      counter -= size;
    }
  }
  closedir(dr);
}

/*
  Function Declarations for builtin shell commands:
 */
int lsh_autoftp(char **args);
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_copy(char **args);
int lsh_paste(char **args);
int lsh_exit(char **args);
int lsh_list(char **args);
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "autoftp",
    "cd",
    "help",
    "exit",
    "list",
    "copy",
    "paste"};

int (*builtin_func[])(char **) = {
    &lsh_autoftp,
    &lsh_cd,
    &lsh_help,
    &lsh_exit,
    &lsh_list,
    &lsh_copy,
    &lsh_paste};

int lsh_num_builtins()
{
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args. two arguments args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int lsh_cd(char **args)
{
  if (args[1] == NULL)
  {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  }
  else
  {
    if (chdir(args[1]) != 0)
    {
      perror("lsh");
    }
  }
  return 1;
}

/**
   @brief Builtin command: print help.
   @param args List of args. only args[0]=help.
   @return Always returns 1, to continue executing.
 */
int lsh_help(char **args)
{
  int i;
  printf("LSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < lsh_num_builtins(); i++)
  {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  only args[0]=exit.
   @return Always returns 0, to terminate execution.
 */
int lsh_exit(char **args)
{
  return 0;
}
/**
   @brief Bultin command: list files.
   @param args List of args. only args[0] is "list"
   @return Always returns 1, to continue executing.
 */
int lsh_list(char **args)
{
  //This is the root directory
  char arr[200] = ".";

  //Declare level
  int level = atoi(args[1]);
  // printf("Enter the level to which you want to list files: ");

  //Get the level from the user
  // scanf("%d", &level);

  //Compile the regular expression
  reti = regcomp(&regex, expression, 0);
  if (reti)
  {
    fprintf(stderr, "Could not compile regex\n");
  }

  //Go into the main loop
  listDirectory(0, level, arr, "");

  return 1;
}
/**
   @brief Bultin command: change directory.
   @param args List of args. only args[0] is "autoftp".
   @return Always returns 1, to continue executing.
 */

int lsh_autoftp(char **args)
{
  FILE *pp;
  pp = popen("./f.sh", "r");
  if (pp != NULL)
  {
    while (1)
    {
      char *line;
      char buf[1000];
      line = fgets(buf, sizeof buf, pp);
      if (line == NULL)
      {
        break;
      }
      else
        printf("%s", line);
    }
    pclose(pp);
  }
  return 1;
}
/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_copy(char **args)
{
  char cwd[FILENAME_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
  {
    //  printf("Current working dir: %s\n", cwd);
  }
  else
  {
    //  perror("getcwd() error");
  }

  if (args[1] == NULL)
  {
    fprintf(stderr, "lsh: expected argument to \"copy\"\n");
  }

  int count = 0;
  while (args[++count] != NULL)
    ;
  count -= 1;
  int uniqueCounter = 1;

  while (uniqueCounter <= count)
  {
    strcpy(filename, args[uniqueCounter]);
    strcpy(fileNamearray[counter], filename);

    char filePath[sizeof(cwd) + 1 + sizeof(filename) + 1];
    strcpy(filePath, cwd);
    strcat(filePath, "/");
    strcat(filePath, filename);
    strcat(filePath, "\0");
    printf("%s\n", filePath);

    strcpy(filearray[counter], filePath);
    counter += 1;
    uniqueCounter += 1;
  }
  return 1;

  // while(1){
  // 	printf("Enter the filename to open for reading. Press \"quit\" to exit from reading \n");
  // 	scanf("%s", filename);
  //    strcpy(fileNamearray[counter], filename);

  //    char filePath[sizeof(cwd) + 1 +sizeof(filename) + 1];
  // 	strcpy(filePath,cwd);
  // 	strcat(filePath,"/");
  // 	strcat(filePath,filename);
  // 	strcat(filePath,"\0");
  // 	printf("%s\n",filePath);

  // 	if (strcmp(filename,"quit") == 0)
  // 	{
  // 		break;

  // 		printf("Cannot open file %s \n", filename);
  // 		exit(0);
  // 	}

  // 	strcpy(filearray[counter], filePath);
  // 	counter+=1;
  // }
  //  return 1;
}
/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_paste(char **args)
{
  for (int i = 0; i < counter; i++)
  {
    printf("%s %s\n", filearray[i], filearray[i]);

    fptr1 = fopen(fileNamearray[i], "w");

    // Read contents from file
    FILE *fptr = fopen(filearray[i], "r");
    charData = fgetc(fptr1);
    while ((charData = fgetc(fptr)) != EOF)
    {
      fputc(charData, fptr1);
    }

    fclose(fptr1);
    printf("\nContents copied to %s\n", filearray[i]);
  }
  return 1;
}
/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0)
  {
    // Child process
    if (execvp(args[0], args) == -1)
    {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  }
  else if (pid < 0)
  {
    // Error forking
    perror("lsh");
  }
  else
  {
    // Parent process
    do
    {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL)
  {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++)
  {
    if (strcmp(args[0], builtin_str[i]) == 0)
    {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}

#define LSH_RL_BUFSIZE 1024
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *lsh_read_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer)
  {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    // Read a character
    c = getchar();

    if (c == EOF)
    {
      exit(EXIT_SUCCESS);
    }
    else if (c == '\n')
    {
      buffer[position] = '\0';
      return buffer;
    }
    else
    {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize)
    {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer)
      {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token, **tokens_backup;

  if (!tokens)
  {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL)
  {
    tokens[position] = token;
    position++;

    if (position >= bufsize)
    {
      bufsize += LSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens)
      {
        free(tokens_backup);
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void lsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do
  {
    printf("> ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}