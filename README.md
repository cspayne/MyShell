README

This is the MyShell program written by Cypress Payne 
for CSC 3350

MyShell Built in commands: \n

// [] are optional \n
<> are required \n

help: lists all built-in commands\n
exit : terminates shell process\n
pwd : prints working directory\n
cd [dir] : changes directories to main or to specified directory\n
setpath <dir> [dir] ... [dir] : overwrites current paths and saves all\n
	\t paths specified\n
new_cat [.txt] : copies text from input or from file into output stream.\n
	\t Can be exited using command "close"\n
new_echo [arg] ... [arg] : copies arguments into output stream and exits.\n
new_tail [.txt] [-n num] : takes num end lines from .txt file or input stream\n 
	\t and copies them into output stream. If no file is selected, the input\n
	\t stream ends once CTRL-D is pressed.\n
new_wc [.txt] [-l] OR [-c] : gives wordcount of chosen text or input after CTRL-D\n
	\t is pressed. -l displays the line count and word count. -c displays\n
	\t the character count and wordcount.\n
portman <arg> <arg> : takes to strings and combines them to create a string\n 
	\t made from the first half of the first string and the second half\n
	\t of the second string.\n
sleepykitten : contains a sleepykitten short adventure game with prompts\n
	\t provided. \n

MyShell also allows executables to be run if they can be found in the PATH like
other shells do. 