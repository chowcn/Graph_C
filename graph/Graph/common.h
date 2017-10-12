/*
 * Document    : common.h
 * Auther      : Josh.Chow
 * Date        : 2017-5-21
 * Description :
 * Cautions    :
 * -----------------------------------------------------------------------
   Modify History
  -----------------------------------------------------------------------
   Date		| Auther	 | Description
  -----------------------------------------------------------------------
  2017-5-28    
  -----------------------------------------------------------------------  	 
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C"{
#endif


#define DEBUG
#undef  DEBUG

#ifdef  DEBUG
#define DEBUG_PRINT(fmt, str)	\
	printf(fmt, str)
#else
#define DEBUG_PRINT(fmt, str)
#endif


/* Macro of const value */
#define NULL ((void *)0)       /* Repleace NULL defination in stdio.h */

#define ERROR_SUCCESS     0
#define ERROR_FAILED      1
#define ERROR_RESOURCE    2
#define ERROR_PARAMETER   3
#define ERROR_DFSCYCLE    4


#define BOOL_TRUE   1
#define BOOL_FALSE  0

#define NONE_USE_VALUE (~0)


/* common user variable type defination */
#define INT          signed int
#define UINT         unsigned int
#define LONG         signed long
#define ULONG        unsigned long
#define DOUBLE       double
#define FLOAT        float
#define CHAR         char
#define VOID         void
#define BOOL_T       unsigned short

/* 
 * Decorate the parameter, 
 * IN for only read, 
 * OUT only for write, 
 * INOUT for both of IN and OUT
 */
#define IN 
#define OUT
#define INOUT

/*
 * Decorate the function 
 */
#define STATIC static
#define EXTERN extern


#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H__*/
