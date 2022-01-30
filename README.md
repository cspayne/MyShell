README

This is the MyShell program written by Cypress Payne 
for CSC 3350

MyShell Built in commands:

[] are optional
<> are required

help: lists all built-in commands
exit : terminates shell process
pwd : prints working directory
cd [dir] : changes directories to main or to specified 	directory
setpath <dir> [dir] ... [dir] : overwrites current paths and 		saves all paths specified
new_cat [.txt] : copies text from input or from file into 		output stream. Can be exited using command "close"
new_echo [arg] ... [arg] : copies arguments into output 	stream and exits.
new_tail [.txt] [-n num] : takes num end lines from .txt file 	or input stream and copies them into output stream. 		If no file is selected, the input stream ends once 	CTRL-D is pressed.
new_wc [.txt] [-l] OR [-c] : gives wordcount of chosen text 		or input after CTRL-D is pressed. 
	-l displays the line count and word count. 
	-c displays the character count and wordcount.
portman <arg> <arg> : takes to strings and combines them to 		create a string made from the first half of the first 	string and the second half of the second string.
sleepykitten : contains a sleepykitten short adventure game 	with prompts provided.

MyShell also allows executables to be run if they can be found in the PATH like other shells do. 