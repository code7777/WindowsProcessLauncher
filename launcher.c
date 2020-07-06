/*
Course: CS302 Operating systems
Author: Preston Porter
Assignment: Homework2 
Description:

Uses the windows  process structure to open 
1: notepad
2: Wordpad
3: command window
4: calculator
5: File Explorer
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// function prototype
void printError(char* functionName);
void listChoices();//lists choices for user
void launchProcess(int x);//launches Processes using Windows function CreateProcess
char process[6][256]; //an array of characters to hold the different windows processes
void getEnv();//associates each process with appropriate path
int main(void)
{

	
	int x=7; //initialized variable to store user input
	
	getEnv();
	do{
		 listChoices();
		scanf("%d", &x);
     launchProcess(x);
		
       
   putenv("prompt="); // Reset to the default command shell prompt


	}while( x != 0);




   return 0;
}
void listChoices(){
      printf("Enter a number 0 through 5 for the following \n");
		printf("  0: Quit \n");
		printf("  1: Run Notepad \n");
		printf("  2: Run Wordpad \n");
		printf(" *3: Run cmd prompt \n");
		printf("  4: Run Calculator \n");
		printf("  5: Run explorer \n");
		printf("Type your number then press enter on your keyboard: ");

}
void getEnv(){

 char *environment_var = getenv( "SystemRoot");

	sprintf(process[1], "%s\\system32\\Notepad.exe", environment_var);

	environment_var = getenv( "ProgramFiles");
	
	sprintf(process[2], "%s\\Windows NT\\Accessories\\Wordpad.exe", environment_var);

	environment_var = getenv( "COMSPEC");
	sprintf(process[3], "%s /T:F9", environment_var);

	environment_var = getenv( "SystemRoot");
	sprintf(process[4], "%s\\system32\\calc.exe", environment_var);

	environment_var = getenv("SystemRoot");
	sprintf(process[5], "%s\\explorer.exe", environment_var);
	
}
void launchProcess(int x){

   PROCESS_INFORMATION processInfo;
   /*If CreateProcess succeeds, it returns a PROCESS_INFORMATION structure containing handles and identifiers for the new 
   processand its primary thread */
   
	STARTUPINFO startInfo; 
   /*Specifies the window station, desktop, standard handles, and appearance of the main window for a process at creation time.*/
   
   
	ZeroMemory(&startInfo, sizeof(startInfo));//fills startInfo with zeros to initialize
	startInfo.cb = sizeof(startInfo); // Set structure size
  
   char processName[6][256];

   switch( x)
   {
	
         case 1: case 2: case 4: case 5: 
      
         switch(x){
         case 1 :  sprintf(processName[1],"Notepad"); break;
         case 2 :  sprintf(processName[2],"Wordpad"); break;
         case 3 :  sprintf(processName[3],"Cmd Prompt"); break;
         case 4 :  sprintf(processName[4],"Calculator"); break;
         case 5 :  sprintf(processName[5],"explorer"); break;
         break;
         }
          /*
          https://docs.microsoft.com/en-us/windows/desktop/api/processthreadsapi/nf-processthreadsapi-createprocessa
          Function Create process         
          @param  LPCSTR lpApplicationName,
          @param  LPSTR  pCommandLine,
          @param  LPSECURITY_ATTRIBUTES lpProcessAttributes,
          @param  BOOL bInheritHandles,
          @param  DWORD dwCreationFlags,
          @param  LPVOID lpEnvironment,
          @param  LPCSTR lpCurrentDirectory,
          @param  LPSTARTUPINFOA lpStartupInfo,
          @param  LPPROCESS_INFORMATION lpProcessInformation
          @return BOOL 
          */
			if(!CreateProcess(NULL, process[x], NULL, NULL, FALSE,
				HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
				NULL, NULL, &startInfo, &processInfo))
			{
				printError("CreateProcess");
			}
      
			else
			{
         
          
				printf("Starting %s with pid = %d\n\n", processName[x], (int)processInfo.dwProcessId); //prints process ID to screen
            CloseHandle(processInfo.hThread); //closes thread Handle
           CloseHandle(processInfo.hProcess); //closes process Handle
			}
      break;
      
			case 3: //if cmd prompt is opened
		
			 startInfo.lpTitle = "What is your command?"; //title of new window
         
			 _putenv("prompt=Speak to me$G"); //output to command prompt
           /*
          https://docs.microsoft.com/en-us/windows/desktop/api/processthreadsapi/nf-processthreadsapi-createprocessa
          Function Create process         
          @param  LPCSTR lpApplicationName,
          @param  LPSTR  pCommandLine,
          @param  LPSECURITY_ATTRIBUTES lpProcessAttributes,
          @param  BOOL bInheritHandles,
          @param  DWORD dwCreationFlags,
          @param  LPVOID lpEnvironment,
          @param  LPCSTR lpCurrentDirectory,
          @param  LPSTARTUPINFOA lpStartupInfo,
          @param  LPPROCESS_INFORMATION lpProcessInformation
          @return BOOL 
          */
         if(!CreateProcess(NULL, process[x], NULL, NULL, FALSE,
				HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
				NULL, NULL, &startInfo, &processInfo))
			{
				printError("CreateProcess"); //error
			}
			else
			{
         
        
            LPDWORD *ExitValue=0; //exit code intialized 
				
				printf("Starting Cmd Prompt with pid = %d\n\n",  (int)processInfo.dwProcessId);  //prints process ID to screen
            
            printf("  waiting for Cmd Prompt to terminate...\n\n");
                /*
                Waits until the specified object is in the signaled state or the time-out interval elapses*/
                WaitForSingleObject(processInfo.hProcess, INFINITE); 
               
              
                GetExitCodeProcess(processInfo.hProcess, &ExitValue); //Retrieves the termination status of the specified process
            printf(" Cmd Prompt exited with return value %d\n\n",   (int) ExitValue);
            
            CloseHandle(processInfo.hThread); //close handle thread
				CloseHandle(processInfo.hProcess); //close handle process
         }
         break;
		
      default: break;
   }
}
/****************************************************************
   The following function can be used to print out "meaningful"
   error messages. If you call a Win32 function and it returns
   with an error condition, then call this function right away and
   pass it a string containing the name of the Win32 function that
   failed. This function will print out a reasonable text message
   explaining the error and then (if chosen) terminate the program.
*/

void printError(char* functionName)
{
   LPVOID lpMsgBuf;
   int error_no;
   error_no = GetLastError();
   FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
         NULL,
         error_no,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf,
         0,
         NULL
   );
   // Display the string.
   fprintf(stderr, "\n%s failed on error %d: ", functionName, error_no);
   fprintf(stderr, (const char*)lpMsgBuf);
   // Free the buffer.
   LocalFree( lpMsgBuf );
   //ExitProcess(1);  // terminate the program
}//printError
