#ifndef SINTSTACK_H
#define SINTSTACK_H

/*
 * SIntStack.h
 *
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to SORCERER -- SORCERER is in the public
 * domain.  An individual or company may do whatever they wish with
 * source code distributed with SORCERER or the code generated by
 * SORCERER, including the incorporation of SORCERER, or its output, into
 * commerical software.
 *
 * We encourage users to develop software with SORCERER.  However, we do
 * ask that credit is given to us for developing SORCERER.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like SORCERER and have developed a nice tool with the
 * output, please mention that you developed it using SORCERER.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * SORCERER 1.00B
 * Terence Parr
 * AHPCRC, University of Minnesota
 * 1992-1994
 */

#include "sorcerer.h"

typedef struct _istack {
    int *data;
    int sp;
    int  size;
  } SIntStack;

#ifdef __USE_PROTOS
extern SIntStack *sint_newstack(int size);
extern void sint_freestack(SIntStack *st);
extern void sint_push(SIntStack *st, int i);
extern int sint_pop(SIntStack *st);
extern int sint_stacksize(SIntStack *st);
extern void sint_stackreset(SIntStack *st);
extern int sint_stackempty(SIntStack *st);
extern int sint_top(SIntStack *st);
#else
extern SIntStack *sint_newstack();
extern void sint_freestack();
extern void sint_push();
extern int sint_pop();
extern int sint_stacksize();
extern void sint_stackreset();
extern int sint_stackempty();
extern int sint_top();
#endif

#endif
