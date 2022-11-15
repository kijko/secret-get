#include <windows.h>
#include <tchar.h> // required for _tmain
#include <strsafe.h> // for printf

#define PW_BUFFER 64
#define PW_PROMPT_MSG "Password: \n"

#include "BitwardenList.h"

int cleanUpBwOutput(TCHAR *, size_t);

int bwList(char *buff, int buffSize, char *secretName) {
   TCHAR szCmdline[64];
   ZeroMemory(szCmdline, 64);
   if (FAILED(StringCbCopy(szCmdline, 64, TEXT("bw list items --search ")))) {
      printf("Failed to copy base command\n");

      return 10;
   }

   if (FAILED(StringCbCat(szCmdline, 64, secretName))) {
      printf("Failed to copy base command\n");

      return 11;
   }

//   printf("debug: szCmdline '%s'\n", szCmdline);

   STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
    sa.bInheritHandle = TRUE; 
    sa.lpSecurityDescriptor = NULL; 

    HANDLE childStdOutPipeWrite = NULL;
    HANDLE childStdOutPipeRead = NULL;
    if (!CreatePipe(&childStdOutPipeRead, &childStdOutPipeWrite, &sa, 0)) {
       printf("Create child std out pipe failed (%d).\n", GetLastError());
       return 12;
    }

    if (!SetHandleInformation(childStdOutPipeRead, HANDLE_FLAG_INHERIT, 0)) {
       printf("Failed setting out read not inheritable (%d).\n", GetLastError());
       return 13;
    }

    HANDLE stdInput = GetStdHandle(STD_INPUT_HANDLE);

    si.hStdError = childStdOutPipeWrite;
    si.hStdOutput = childStdOutPipeWrite;
    si.hStdInput = stdInput;
    si.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line)
        szCmdline,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return 14;
    }


   HANDLE stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
   WriteFile(stdOutput, TEXT(PW_PROMPT_MSG), sizeof(TCHAR) * lstrlen(PW_PROMPT_MSG), NULL, NULL);

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    HANDLE childReader = childStdOutPipeRead;
    TCHAR bwOutput[4096];
    ZeroMemory(bwOutput, 4096);
    ReadFile(childReader, bwOutput, (sizeof(TCHAR) * 4096) - 1, NULL, NULL);
    
    int finalResult = 0;
    int cleanUpRes = cleanUpBwOutput(bwOutput, 4096);
    if (cleanUpRes) {

       if (cleanUpRes == 4 || cleanUpRes == 5 || cleanUpRes == 9) {
          printf("Non json response (%d): '%s'\n", cleanUpRes, bwOutput);
       }

       printf("Unexpected output from 'bw' process (%d)\n", cleanUpRes);

       finalResult = 15;
    }

   ZeroMemory(buff, buffSize);
   if (finalResult == 0 && FAILED(StringCbCopy(buff, buffSize, bwOutput))) {
      printf("Failed to copy output json to result buffer\n");

      return 16;
   }

    // todo close it if error
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    CloseHandle(childStdOutPipeWrite);
//    CloseHandle(childStdInPipeRead);

   return finalResult;
}

int cleanUpBwOutput(TCHAR *output, size_t maxlen) {
   size_t actualLen = 0;

   if (SUCCEEDED(StringCbLength(output, maxlen, &actualLen))) {

      if (actualLen > 0) {
         int newline = -1;

         for (int i = actualLen; i >= 0; i--) {
            if (output[i] == '\n') {
               newline = i;
               break;
            }
         }

         if (newline > -1) {
            int jsonStart = -1, jsonEnd = -1;
            for (int i = newline+1; i < (int) actualLen; i++) {
               if (output[i] == '[') {
                  jsonStart = i;
                  break;
               }
            }

            for (int i = actualLen; i >= 0; i--) {
               if (output[i] == ']') {
                  jsonEnd = i;
                  break;
               }
            }

            TCHAR tempBuffer[actualLen];
            ZeroMemory(tempBuffer, actualLen);
            int t = 0, status = 0;
            if (jsonStart > -1 && jsonEnd > -1) {

               for (int i = jsonStart; i <= jsonEnd; i++) {
                  tempBuffer[t] = output[i];
                  t++;
               }

            } else {
               
               for (int i = newline+1; i < (int) actualLen; i++) {
                  tempBuffer[t] = output[i];
                  t++;
               }

               if (jsonStart == -1) status += 4;
               if (jsonEnd == -1) status += 5;
            }

            ZeroMemory(output, actualLen);
            for (int i = 0; i < t; i++) output[i] = tempBuffer[i];

            return status;
         }

         return 3;
      }

      return 2;
   }

   return 1;
}


