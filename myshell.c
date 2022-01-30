/* myshell program by Cypress Payne
  written for CSC 3350 lab 2*/

//include statements
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

//important global variables
#define MAXCHAR 256
char PATH[256] = "/bin";
char* PATHDIR[10];

//the printHelp function takes nothing and returns nothing, it simply prints information to the screen and exits
void printHelp(void){
  char help[] = "MyShell Built in commands: \n"
"// [] are optional \n"
"<> are required \n"
"help: lists all built-in commands\n"
"exit : terminates shell process\n"
"pwd : prints working directory\n"
"cd [dir] : changes directories to main or to specified directory\n"
"setpath <dir> [dir] ... [dir] : overwrites current paths and saves all\n"
"	\t paths specified\n"
"new_cat [.txt] : copies text from input or from file into output stream.\n"
"	\t Can be exited using command close\n"
"new_echo [arg] ... [arg] : copies arguments into output stream and exits.\n"
"new_tail [.txt] [-n num] : takes num end lines from .txt file or input stream\n "
	"\t and copies them into output stream. If no file is selected, the input\n"
"	\t stream ends once CTRL-D is pressed.\n"
"new_wc [.txt] [-l] OR [-c] : gives wordcount of chosen text or input after CTRL-D\n"
"	\t is pressed. -l displays the line count and word count. -c displays\n"
"	\t the character count and wordcount.\n"
"portman <arg> <arg> : takes to strings and combines them to create a string\n "
	"\t made from the first half of the first string and the second half\n"
"	\t of the second string.\n"
"sleepykitten : contains a sleepykitten short adventure game with prompts\n"
	"\t provided. \n";
  printf("%s\n", help);
}

//pwdFunction is void and has no inputs. It uses getcwd to get the directory and prints that location
void pwdFunction(){
  char * dir = getcwd(NULL, 0);
  fprintf(stdout, "%s\n", dir);
}

//cdfunction changes the directory. It requires an array of arguments and the size of that array and returns nothing
void cdFunction(int size, char* args[]){
  char * path;
  //if the array has only one element, it has no arguments so cd finds the home directory
  if(size == 1){
    path = getenv("HOME");
    if(chdir(path)){
      perror("path not found");
    }
  }
  //else it finds the directory given
  else if(size == 2){
    path = args[1];
    if(chdir(path)){
      //if it doesn't exist, an error is printed
      perror("path not found");
    }
  }
}

//the new_cat function takes an argument of int argc (size) and an array of arguments and prints a file or the input
//to the output stream
void new_cat (int argc, char* argv[]){
  FILE *fp;
  char str[100];
  /* this if statement checks if the number of arguments passed is two*/
  if (argc == 2){
    /*if it is two, then fopen assumes the argument passed is a file and tries to open it.
      the r means it opens it as a read only file*/
      fp = fopen(argv[1],"r");
   /*if the argument is not a file or doesn't exist, then the file pointer is NULL and this if
     statement will print an error message to stderr and exit the program*/
     if (fp == NULL){
       fprintf(stderr, "%s\n", "Err: unable to open file");
       return;
     }
   /*else, the file was successfully opened*/
    else{
     /* if fgets is NULL it means the file was empty and this statement will print an error
	    message to stderr*/
      if(fgets(str, 100, fp) == NULL){
          fprintf(stderr, "%s\n", "Err: File empty");
      }
       /*else, while fgets, a function that retrieves a string from a stream (fp in this case)
         and stores it in an array of chars (str) of a certain size (100), is not NULL, fprintf
         will print the chars placed in str to the stdout stream*/
      while(fgets(str, 100, fp)){
  	     fprintf(stdout, "%s", str);
      }
       /*then the file is closed and the program exited*/
      fclose(fp);
      return;
    }
  }
  /*else if argc == 1, this means no arguments were passed*/
  else if(argc == 1){
     /*so, fgets is used to get up to 100 characters from the stdin stream and store them in the array
       str. While fgets does not return NULL, fprintf will print these characters to the stdout stream*/
       char close[10] = "close\n";
       while(fgets(str, 100, stdin) != NULL){
         if(strcmp(str, close) == 0){
           return;
         }
          fprintf(stdout, "%s", str);
       }
     return;
   }
  /*else, if argc > 2, then too many arguments were passed so an error message will be sent to the stderr
    stream, and then the main function will return and the program will exit*/
   else{
     fprintf(stderr, "%s\n", "Err: Too many arguments given");
   }
}

//new echo takes an int argc and an array of arguments and echos a text file or an input into the output stream
void new_echo(int argc, char *argv[]){
  /*this if statement checks if no arguments were passed when the program is run, if so, then
    it only needs to print a new line, done by fprintf into the stdout stream*/
  if(argc == 1){
    fprintf(stdout, "%s\n", "");
  }
  /*else, if an argument or more has been passed, then all of these arguments must be printed*/
  else if(argc > 1){
    /*the while loop checks whether the array at the i-th position is filled or not. While it is,
      fprintf will print that character into the stdout stream and increase i to move down the array*/
    int i = 1;
    while(argv[i] != NULL){
      fprintf(stdout, "%s ", argv[i]);
      i++;
    }
    /*once all the arguments have been printed, another new line is printed to give the program a
      clean look*/
    fprintf(stdout, "%s\n", "");
  }
  return;
}

//setpath takes an int size and an array of arguments and replaces the current paths stored in PATHDIR with
//those specified by the user.
void setPath(int size, char* args[]){
  //not enough arguments, print error and return
  if(size == 1){
    fprintf(stderr, "%s\n", "Err: expecting argument for setpath");
    return;
  }
  //else replace current path with new path
  else{
    char pass[256];
    for(size_t i = 0; i < size; i++){
      strcpy(pass, args[i]);
      PATHDIR[i] = pass;
    }
  }
  return;
}

//new_tail takes an int argc and an array of arguments and prints the tailing end of a string, either a textfile
//or input to the output stream
void new_tail(int argc, char* argv[]){

  FILE *readto;
  FILE *readfrom;
  char in[100];
  char out[100];
  int num, linetotal, linecurr, start;

  /*if no arguments are passed, argc will be one. This means the program should run with stdin as
   the input text and the number of lines to be read equaling 5*/
  if(argc == 1){
    num = 5;
    readfrom = stdin;
  }
  /*else if argc == 2 this means that one argument has been passed. The program assumes it is a
    text file (and will produce an error if not). So num is 5 and the input text is the text file*/
  else if(argc == 2){
    num = 5;
    readfrom = fopen(argv[1], "r");
    /*if the file is not found, an error will be produced, readfrom will be NULL and an error will
      be produced and the program will exit*/
    if(readfrom == NULL){
      fprintf(stderr, "%s\n", "Err: file input not valid");
      return;
    }
  }
  /* else if argc == 3 this means two arguments have been passed. They must be -n and then a number
     int that order or the arguments will not be accepted*/
  else if (argc == 3){
    char str[10];
    char n[10];
    /*the argument is passed as a char so it is converted to an int using atoi*/
    int newNum = atoi(argv[2]);
    strcpy(str, argv[1]);
    strcpy(n, "-n");
    /*this if statement checks if the tag (-n) is actually -n or is an incorrect argument. If it does
      not pass, an error is sent and the program exits*/
    if (strcmp(str, n)){
      fprintf(stderr, "%s\n", "Err: tag invalid");
      return;
    }
    /* if -n was the first input then this if checks that the second argument is a valid number. If
       not, it produces an error and exits the program*/
    else if(newNum == 0){
      fprintf(stderr, "%s\n", "Err: number argument invalid");
      return;
    }
    /*if -n is correct and the second argument is an int, the int is set as the number of lines and
      the input is what is typed */
    else {
      num = newNum;
      readfrom = stdin;
    }

  }
  /* else if argc is 4, that means three arguments have been passed. This means there are two ways the
     arguments could be passed that are valid:
     1) -n number textfile
     2) textfile -n number */
  else if (argc == 4){
    char str[10];
    char n[10];
    int newNum;

    strcpy(str, argv[1]);
    strcpy(n, "-n");
    /*this if statement checks if the first argument is -n*/
    if(strcmp(str, n)){
      strcpy(str, argv[2]);
      /*if it is not, then it checks if the second argument is -n, if it is not, then
	the arguments are incorrect and an error is sent*/
      if(strcmp(str, n)){
	fprintf(stderr, "%s\n", "Err: 1 or more arguments invalid");
	return;
      }
      /*else, the second argument is -n so it knows the third argument is the txt file and the first
	is the number of lines to be read*/
      else{
	newNum = atoi(argv[3]);
	/*if the number of lines is not a valid number, an error occurs*/
	if(newNum == 0){
	  fprintf(stderr, "%s\n", "Err: number argument invalid");
	  return;
	}
	/*else all is well and the program can continue with the number of lines and the input as the
	  text file*/
	num = newNum;
	readfrom = fopen(argv[1], "r");
      }
    }
    /*if the first argument IS -n, then we know that the second argument is the number and the third
      is the text file*/
    else{
     newNum = atoi(argv[2]);
     /* then it checks that the int is a valid number, if not an error occurs*/
     if(newNum == 0){
       fprintf(stderr, "%s\n", "Err: number argument invalid");
       return;
     }
     /*else, it can continue with the number of lines as the number passed and the text file as the
       input to be copied*/
     num = newNum;
     readfrom = fopen(argv[3], "r");
    }
  }
  /*if more than 3 arguments are passed, an error occurs and the program exits*/
  else {
    fprintf(stderr, "%s\n", "Err: too many arguments");
    return;
  }

  /*now with the number of lines and the appropriate input, the program can copy the last number
    of lines to stdout */

  /*first, a temporary new document is created that can be written to and read. If the document already
    is filled, it is made to be blank*/
  readto = fopen("temp.txt", "w+");

  /*then, while the input is copied into an array of chars, the number of lines is counted
    and the input is copied to the temporary file*/
  while (fgets(in, 100, readfrom) != NULL){
     linetotal++;
     fprintf(readto, "%s", in);
  }
  fclose(readfrom);
  fclose(readto);
  /*then the temporary file is opened as a read only file*/
  readto = fopen("temp.txt", "r");
  fprintf(stdout, "%s\n", "");
  /*while the temporary file is read, the end lines will be printed*/
  while(fgets(out, 100, readto) != NULL){
    /* if the number of lines to be printed is equal to or greater than the number of lines
       in the whole file, the entire file will be printed in stdout*/
    if(linetotal <= num){
	fprintf(stdout, "%s", out);
    }
    /*else, once the current line leaves only the number of lines remaining that the user wants
      to be printed, the remaining will be printed in stdout*/
    else if(linecurr == (linetotal - num)){
         fprintf(stdout, "%s", out);
         num--;
    }
       linecurr++;
  }
  fclose(readto);
  fprintf(stdout, "%s\n", "");
  return;
}

//new_wc takes an int argc and an array of arguments and calculates the wordcount of a file or an input
void new_wc(int argc, char* argv[]){

  FILE *input;
  FILE *temp;
  char arr[250];
  char curc;
  char str[10];
  char testc[10] = "-c";
  char testl[10] = "-l";
  int lselect = 0;
  int cselect = 0;
  int wcount = 0;
  int lcount = 0;
  int ccount = 0;

//no arguments
  if(argc == 1){
    temp = stdin;
    input = fopen("temp.txt", "w+");

    while(fgets(arr, 250, temp) != NULL){
      fprintf(input, "%s", arr);
    }
    fclose(input);
    input = fopen("temp.txt", "r");
  }

//one argument - a text file
  else if(argc == 2){
    strcpy(str, argv[1]);
    if(!strcmp(str, testc)){
	cselect = 1;
	lselect = 0;
	temp = stdin;
	input = fopen("temp.txt", "w+");

	while(fgets(arr, 250, temp) != NULL){
	    fprintf(input, "%s", arr);
	  }
	fclose(input);
	input = fopen("temp.txt", "r");
    }
    else if(!strcmp(str, testl)){
      lselect = 1;
      cselect = 0;
      temp = stdin;
      input = fopen("temp.txt", "w+");

      while(fgets(arr, 250, temp) != NULL){
	fprintf(input, "%s", arr);
      }
      fclose(input);
      input = fopen("temp.txt", "r");
    }
    else{
      input = fopen(argv[1], "r");

      if(input == NULL){
	fprintf(stderr, "%s\n", "Err: cannot open file");
	exit(0);
      }
    }
  }
//multiple arguments, test file and - command
  else if(argc == 3){
    strcpy(str, argv[1]);
    if(!strcmp(str, testl)){
      lselect = 1;
      cselect = 0;
      input = fopen(argv[2], "r");
    }
    else if(!strcmp(str, testc)){
      cselect = 1;
      lselect = 0;
      input = fopen(argv[2], "r");
    }
    else{
      input = fopen(argv[1], "r");
      strcpy(str, argv[2]);

      if(!strcmp(str, testl)){
        lselect = 1;
	cselect = 0;
      }
      else if(!strcmp(str, testc)){
        cselect = 1;
	lselect = 0;
      }
    }
  }

  else{
    fprintf(stderr, "%s\n", "Err: too many arguments");
    return;
  }

  if(input == NULL){
    fprintf(stderr, "%s\n", "Err: file input not valid");
    return;
    }

  while((curc = fgetc(input)) != EOF){
    ccount++;
    if(curc == ' ' || curc == '\n'){
      wcount++;
    }
    if(curc == '\n'){
      lcount++;
    }
  }
  fclose(input);
  fprintf(stdout, "Word Count: %d\n", wcount);
  if(lselect == 1){
    fprintf(stdout, "Line Count: %d\n", lcount);
  }
  if(cselect == 1){
    fprintf(stdout, "Character Count: %d\n", ccount);
  }
  return;
}

//this void function plays out a small rpg about a sleepy kitten
void sleepykitten(){
  int done = 0;
  char line[10];
  char quiet[10] = "quiet\n";
  char loud[10] = "loud\n";
  char q[10] = "q\n";
  fprintf(stdout, "%s\n", "press q to quit");
  while(done == 0){
    fprintf(stdout, "%s\n", "The kitten is sleepy.");
    fprintf(stdout, "%s\n", "How loud are you? (quiet, loud)");
    fgets(line, 10, stdin);

    if(strcmp(line, q) == 0){
      return;
    }
    else if(strcmp(line, quiet) == 0){
        fprintf(stdout, "%s\n", "The kitten is asleep");
        done = 1;
      }
    else if(strcmp(line, loud) == 0){
      fprintf(stdout, "%s\n", "You woke the kitten!");
      done = 1;
    }
    else{
      fprintf(stderr, "%s\n", "I'm sorry I don't understand");
    }
  }
}

//this void function takes an int argc and an array of arguments. it requires two arguments and splits the
//two strings, recombining them with the first half of the first string and the second half of the second
//string to create a portmanteau
void portman(int argc, char *argv[]){
  //enough arguments
	if(argc == 3){
    int i = 1;
    char first[200];
    char second[200];
    char shalf[100];
    char fhalf[100];
    //copy arguments into strings
    strcpy(first, argv[1]);
    strcpy(second, argv[2]);
    int j = 0;
  //copy first half of first string
  for(size_t i = 0; i < strlen(first)/2; i++){
      fhalf[i] = first[i];
    }
  //copy second half of second string
  for(size_t i = strlen(second)/2; i < strlen(second); i++){
    shalf[j] = second[i];
    j++;
  }
  //use strcat to add shalf to the end of fhalf
  strcat(fhalf,shalf);
  //print the now combined portmanteau and return
  fprintf(stdout, "%s\n", fhalf);
  return;
  }
  //else there weren't the right ammount of arguments
  else{
    fprintf(stderr, "%s\n", "Err: must have only two arguments");
    return;
  }
}

//this function checks if the command given by the user was a built in function or not
//it takes an int size and an array of arguments. it returns an int to communicate
//whether a builtin function matching the argument was found
int checkBuiltinFunc(char* args[], int size){
  int comNum = 11;
  int comSwitch = 0;
  char* comList[comNum];
  //list of built in commands
  comList[0] = "exit";
  comList[1] = "help";
  comList[2] = "pwd";
  comList[3] = "cd";
  comList[4] = "new_cat";
  comList[5] = "setpath";
  comList[6] = "new_echo";
  comList[7] = "new_tail";
  comList[8] = "new_wc";
  comList[9] = "sleepykitten";
  comList[10] = "portman";

  for(size_t i = 0; i < comNum; i++){
    if(strcmp(args[0], comList[i]) == 0){
      comSwitch = i + 1;
      break;
    }
  }
//corresponding functions
    switch(comSwitch){
      case 1:
        exit(0);
      case 2:
        printHelp();
        return 1;
      case 3:
        pwdFunction();
        return 1;
      case 4:
        cdFunction(size, args);
        return 1;
      case 5:
        new_cat(size, args);
        return 1;
      case 6:
        setPath(size, args);
        return 1;
      case 7:
        new_echo(size, args);
        return 1;
      case 8:
        new_tail(size, args);
        return 1;
      case 9:
        new_wc(size, args);
        return 1;
      case 10:
        sleepykitten();
        return 1;
      case 11:
        portman(size, args);
        return 1;
      default:
        break;
  }
  return 0;
}

//this function receives an array of arguments and runs a process using a fork
//it returns an int to main to communicate success or failure
int execArgs(char * args[]){
  //fork child
  pid_t pid = fork();

  if(pid == -1){
    fprintf(stderr, "%s\n", "Err: Fork failed");
    return 1;
  }
  else if (pid == 0){
    if(execvp(args[0], args) < 0){
      fprintf(stderr, "%s\n", "Err: could not execute");
      return 1;
    }
  }
  else{
    //wait for child
    wait(NULL);
    return 0;
  }
}

//the main function runs a loop that everything is done in, this includes getting
//commands from the user, parsing those commands, and using other functions inside
//the loop to properly process the commands before return an int indicating the
//success of the main function. A zero indicates that the main function has completed
//successfully
int main(int argc, char* argv[]){
  //first, check if argument has been passed
  //if too many arguments, pass error and exit
  if(argc > 2){
    fprintf(stderr, "%s\n", "Err: too many arguments");
    exit(1);
  }
  //if one argument was passed, check that it is -h, the only argument accepted
  else if(argc == 2){
    char str[10] = "-h";
    char input[10];
    strcpy(input, argv[1]);
    fprintf(stdout, "%s", input);
    //if it is, print the help text
    if(!strcmp(input, str)){
      printHelp();
      exit(1);
    }
    //else print an error and exit shell
    else{
      fprintf(stderr, "%s\n", "Err: command not recognized");
      exit(0);
    }
  }

  //variables
    char line[256];  //a char array to store the user's command line in
    char* args[50];  //a char* array to store the parsed command in each individual part
    int argcount; //an int to store the number of arguments given by the user
    PATHDIR[0] = PATH;

//main loop
while(1){
  printf("MyShell>> ");                    //print shell prompt
  //fgets gets the user's input from stdin and stores it in line as long as it is less than MAXCHAR bytes
  fgets(line, MAXCHAR, stdin);
  //this portion of code gets red of the \n that may be at the end of the line
  size_t length = strlen(line);
  if (line[length - 1] == '\n'){
    line[length - 1] = '\0';
  }

//next, parse the line and place each seperate piece in the args array
  char *token;
  token = strtok(line," ");
  int i=0;
  while(token!=NULL){
      args[i]=token;
      token = strtok(NULL," ");
      i++;
 }
 //assign the last args index to null and assign argcount the number of commands in args
 args[i]=NULL;
 argcount = i;

  if(strcmp(args[0], "exit")==0){
    exit(0);
  }
  //check if argument is a built in function
  int builtIn;
  builtIn = checkBuiltinFunc(args, argcount);
  if(builtIn == 0){ //not a builtin function
  char curpath[500];
  strcpy(curpath, args[0]);
  int execd;
  //add argument to end of path and see if it is executable
  for(size_t i = 0; i < 10; i++){
    strcat(curpath, PATHDIR[i]);
    execd = execArgs(args);
    if(execd == 0){
      break;
    }
  }
  //if it is not executable, print error
  if(execd == 1){
    fprintf(stderr, "%s\n", "Err: exec not found");
  }
  }
}
}
