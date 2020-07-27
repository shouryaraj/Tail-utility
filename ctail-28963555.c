/**
NAME: SHOURYA RAJ
STUDENT_ID: 28963555
START-DATE; 25-08-2019
DATE-OF-MODIFICATION; 30-08-2019
This program print the last part of the file either from the provided file or the default file. 
Last part can be sepcified in the terms of the characters and the number of lines.
**/



#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//#include <stdio.h>
int character(char *argumnet, int end_point, int infile);
int print(int end_point, int file_pointer, int infile);
int line(char *arg2, char *arg3, int infile);
int check_digit_argument(char *digit_argument, int infile);
int main (int argc, char *argv[])
{
         
          int i, infile, file_pointer, end_point, read_bytes, number_of_lines;      
          char *buffer;
          char *fileread_error = "Error in opening the file.\n";
          char *third_argument_error = "There is no positive number argument for the line shift after -n.\n";
          char *valid_input = "There is no valid argument after -n, it should be a positive integer.\n";
          char *wrong_syntax = "Wrong input syntax used like -L to lines and -n for the number  -n\n";
          char *invalid_order = "Invalid order of arguments";
          char *temp;
          char *command_error = "Invalid command";
          char *filename;
          char *char_value = "200";
          char *too_many_argument = "There are too many arguments";
          char *argument1, *argument2, *argument3, *argument4; // here argument1 is used for to store the command value
          // argumnet2 will either use for store secondary command, -n, or digit argument. argument3 will use for the digit argument only.
         
         // Checking for the 5 argument possibitie, it means only one possible value would be there.
         // ./ctail filename -L -n number
         if(argc > 5){
             write(1, too_many_argument, strlen(too_many_argument));
             exit(1);
         }
         if(argc == 5){
                  filename =  argv[1];  
                  argument1 = argv[2];     
                  argument2 = argv[3];
                  argument3 = argv[4];
          
               
               
           }
         // For the 4 argumnets there are two possibilites, 
         // first is ./ctail -L -n number
         // second is ./ctail filename -n number
         else if(argc == 4){
              // for the -L command 
              if(strcmp(argv[1], "-L")  == 0){
                   filename ="logfile.txt";
                   //printf("%s\n", filename);
                  argument1 = argv[1];     // command
                  argument2 = argv[2];
                  argument3 = argv[3];
               } 
               // -n command with the specified filename
              else{
              
                  filename =  argv[1];
                  argument1 = argv[2];
                  argument2 = argv[3];
                 
              }
         }
         // For the 3 arguments there are two possibilites
         // first one is ./ctail -n number
         // second one is ./ctail filename -L
         else if(argc == 3){
              // handing the -n command
              if(strcmp(argv[1], "-n") == 0){
                 filename = "logfile.txt";
                 argument1 = argv[1];
                // printf("%s\n", argument1);
                 argument2 = argv[2];
                 
              }
              // handing the -L command with the specified filename
              else{
                 filename = argv[1];
                 argument1 = argv[2];
                 argument2 = argv[3];
                 argument3 = argv[4];
              }
         }
         // For the 2 argument there are two possibilites
         // first one is ./ctail -L 
         // second one is ./ctail filename (Only with the filename)
         else if(argc == 2){
             // Handing only -L command on the default filename
             if(strcmp(argv[1], "-L") == 0){
                filename ="logfile.txt";
                argument1 = argv[1];
                argument2 = argv[2];
                argument3 = argv[3];
                
                }
             else if(strcmp(argv[1], "-n") == 0){
                 filename ="logfile.txt";
                 argument1 = argv[1];
                 argument2 = argv[2];
                 argument3 = argv[3];
             
             }
                // Handling the command ./ctail to read the character with the specified filename
              else{
                 filename = argv[1];
                 argument1 = "-n";        // making it default -n for the charachter argument for the function
                 argument2 = char_value;  // default last character for the 200
              }
            }
        // One argument by default read the logfile.txt file 
        else if (argc == 1){
           filename ="logfile.txt";
           argument1 = "-n";             // making it default -n for the charachter argument for the function
           argument2 = char_value;       // default last character for the 200
        }
          
         // Opening the file either default one or user_provided one
         if ((infile = open(filename, O_RDONLY)) < 0 ){
                // if it is an error to opening the error through the error if the first argument is the command
                if((strcmp(argv[1], "-n") == 0) ||(strcmp(argv[1], "-L") == 0)){
                   write(1, invalid_order, strlen(invalid_order));
                   close(infile);
                   exit(2);
                }
                // Throwing the error because of opening the file
               else{
                    write(1, fileread_error, strlen(fileread_error));
                    close(infile);
                    exit(2);
                }
         }
         
         // end_point in the file
         end_point = lseek(infile, 0, SEEK_END);   
         
         //checking condition for either line or character command 
         if(strcmp(argument1, "-L")  == 0){
                   
              file_pointer = line(argument2, argument3, infile);
                   
            }
         //  execute the 
         else if(strcmp(argument1, "-n")  == 0){
              file_pointer = character(argument2, end_point, infile);
         }
         
         else{
             write(1, command_error, strlen(command_error));
             close(infile);
             exit(2);  
         
         }
     
       // printing function to write on the screen because normal byte doesn't 
       // work for the larger character, my computer chuck it off          
       print(end_point, file_pointer, infile);
                                   
        
      
         // initalizing all the elements again 
        
         close(infile);
         // closing the file 
         exit(0);
         
}
/**
This function works find the pointer from the end till it reaches the given number of character by the user.
param: argument is the pointet for the character which is provider by the user either a valid positive interger or any string.
param: end_point is the file last byte location.
return: file_pointer is the interger value which points the character in the file. 
**/
int character(char *argument, int end_point, int infile){
          int number_of_character;
          int file_pointer;
          char *third_argument_error = "There is no argument after the -n\n";
         // char *valid_input = "There is no valid input line number after the -n, it should be a positive integer.\n";
          char *wrong_syntax = "Wrong input syntax used like to count the number of character  -n\n";
         
          //checking for the postive digit argument for after -n
          number_of_character = check_digit_argument(argument, infile);
         
          // checking for the number of number should be greater than to print last 200 charachter
          if(end_point > number_of_character){
              //shifting the point in file at the last 200th charachter in the file
              file_pointer = (end_point - number_of_character);
          
              //printf("%d\n", k);
          }
          else{
              file_pointer = 0;
          }
         
          return file_pointer;
        


}
/**
This function works find the pointer from the end till it reaches the given number of lines by the user.
param: dash_h is the pointer for the character which is command provided by the user.
param: digit_argument is the pointet for the character which is provider by the user either a valid positive interger or any string.
param: infile is the file opening code.
return: file_pointer is the interger value which points the character in the file. 
**/

int line(char *dash_n, char *digit_argument, int infile){
           
              int i, file_pointer, end_point, read_bytes;     
              char *fileread_error = "Error in opening the file\n";
              char *wrong_syntax = "Wrong input syntax used, it has to be ' -n '\n";
              char temp[1024];
              int number_of_lines = 10;  // Setting default number of line read if there is no input for -n
              end_point = lseek(infile, 0, SEEK_END);
             
              file_pointer = lseek(infile,  0 , SEEK_END);
         
              if(dash_n!= 0){   // checking the -n command
                       if (strcmp(dash_n, "-n") == 0){ // checking the valid command after the -L
                               number_of_lines = check_digit_argument(digit_argument , infile);
                         } 
                        else{
                               write(1, wrong_syntax, strlen(wrong_syntax));
                               close(infile);
                               exit(2);
                          }
               }
                 // counter 1 because 0 would lead to end there is nothing to read after that
               i = 1;
                 //printf("%d", point_in_file);
                 // looping through the 
               while(end_point - i >= 0 && number_of_lines > 0){
                      
                   file_pointer = lseek(infile,  (long)-i , SEEK_END);
                          
                   read_bytes = read(infile, &temp, 1);
                        // printf("%d\n", point_in_file); 
                       
                   if(*temp == '\n'){
                        number_of_lines--;
                    }
                    i++ ;
               }
                     
    // because of loop ends at the point of last \n hence we don't have to consider that last character better to shift it by one.
               if (number_of_lines == 0){
                    file_pointer++;
               }
               return file_pointer;
} 
/**
Checking the valid digit argument after the -n command
param: digit_argument is the pointet for the character which is provider by the user either a valid positive interger or any string.
return: number is the valid positive integer which includes zero.
**/                  
int check_digit_argument(char *digit_argument, int infile){
       char *third_argument_error = "There is no argument after -n\n";
       char *valid_input = "There is no valid argument after the -n, it should be a positive integer.\n";
       int number;
       // Checking for the digit argument
       if(digit_argument != 0){
                     // converting the argument into ascii value
                     number = atoi(digit_argument);
                    
                     // checking if the value is positive digit and zero is the special case.
                     if (number <= 0 && strcmp(digit_argument, "0") != 0 ){
                        write(1, valid_input, strlen(valid_input));
                        close(infile);
                        exit(2);
                     }
                     
               }
        // Throwing error if there is no argument 
        else{
                    write(1, third_argument_error, strlen(third_argument_error));
                    close(infile);
                    exit(1);  
               }
  
        return number;
}                
/**
// print function 1024 bytes at a time. 
// It is because for the larger file the computer might break total number of the bytes and print it last read bytes.  
param: end_point is the file last byte location.
param: infile is the file opening code.
return: file_pointer is the interger value which points the character in the file. 

**/            

int print(int end_point, int file_pointer, int infile){
     char temp[end_point];
     int number, k;
     int shift_value = 1024;         // by default 1024 is good choice to take value at a time
    
     while( (end_point - file_pointer) > 0) {
          int k = lseek(infile,  file_pointer , SEEK_SET);  // pre- pointing the 
          
          if(shift_value > end_point - file_pointer){
                  shift_value = end_point - file_pointer;    
           }
           number = read(infile, &temp, shift_value);
           write(1, &temp, number);
          
           file_pointer = file_pointer + shift_value;  
       }
}
