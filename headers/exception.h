//
// Created by maxim on 22/01/2023.
//

#ifndef JUNGLEGAME_EXCEPTION_H
#define JUNGLEGAME_EXCEPTION_H
#include <stdio.h>
#include <setjmp.h>


// Preprocessor macros for creating basic exception support
#define TRY do{ jmp_buf excepBuf; if( !setjmp(excepBuf) ){    // This part creates the varable excepBuf, used for storing stack state
#define CATCH } else {
#define END_TRY } }while(0)                   // We have to have a macro of ending the try loop
#define THROW longjmp(excepBuf, 1)
#endif //JUNGLEGAME_EXCEPTION_H
