/******************************************************************************\
# C - INT_TO_REAL                                #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2025/06/04 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GNU       :: Update - 2025/06/05 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/* *********************** [v] TI CGT CCS (PUSH) [v] ************************ */
#ifdef __TI_COMPILER_VERSION__
#	pragma diag_push /* TI CGT CCS COMPILER DIRECTIVES */
#	pragma CHECK_MISRA("5.4") /* TAG NAMES SHALL BE A UNIQUE IDENTIFIER */
#	pragma CHECK_MISRA("19.3") /*
#		THE #INCLUDE DIRECTIVE SHALL BE FOLLOWED BY EITHER A <FILENAME> OR
#		"FILENAME" SEQUENCE
#	*/
#endif /* __TI_COMPILER_VERSION__ */
/* *********************** [^] TI CGT CCS (PUSH) [^] ************************ */

/* *************************** [v] C++ (PUSH) [v] *************************** */
#ifdef __cplusplus /* C++ */
extern "C" {
#endif /* __cplusplus */
/* *************************** [^] C++ (PUSH) [^] *************************** */

/* *************************** [v] MVS LINKER [v] *************************** */
/* **** MVS LINKER DOES NOT SUPPORT EXTERNAL NAMES LARGER THAN 8 BYTES!! **** */
// NOTE: TARGETING IBM MAINFRAME SYSTEMS (Z/OS)
#ifdef __MVS__
#	pragma map(INT_TO_REAL, "ITORMT01")
#	pragma map(int_to_real, "ITORMT02")
#endif /* __MVS__ */
/* *************************** [^] MVS LINKER [^] *************************** */

/* **************************** [v] INCLUDES [v] **************************** */
#include "../../REAL.h" /*
# define __REAL_SIZE__
#typedef REAL;
#typedef real;
#        */
#include "../../LIBCMT/ENVIRONMENTS/KNR_STYLE.h" /*
# define KNR_STYLE
#        */
#include "../../LIBCMT/ATTRIBUTES/REGPARM.h" /*
# define REGPARM()
#        */
#include "../../LIBCMT/KEYWORDS/LOCAL.h" /*
# define LOCAL
#        */
#include <stdlib.h> /*
#typedef size_t;
#   void *malloc(size_t);
#        */
/* **************************** [^] INCLUDES [^] **************************** */

#ifndef KNR_STYLE /* STANDARD C */
char REGPARM(2)
	NUM_INDEX(register long NUMBER, register unsigned short INDEX)
#else /* K&R */
char REGPARM(2)
	NUM_INDEX(NUMBER, INDEX)
	register long	NUMBER;
	register short	INDEX;
#endif /* !KNR_STYLE */
{
	register unsigned short		NUMBER_SIZE;
	static LOCAL const long		TEN_POWERS[19] = {
		(long)1, /* GENERALLY IGNORED */
		(long)10,
		(long)100,
		(long)1000,
		(long)10000,
		(long)100000,
		(long)1000000,
		(long)10000000,
		(long)100000000,
		(long)1000000000,
		(long)10000000000,
		(long)100000000000,
		(long)1000000000000,
		(long)10000000000000,
		(long)100000000000000,
		(long)1000000000000000,
		(long)10000000000000000,
		(long)100000000000000000,
		(long)1000000000000000000 /* MAX LONG */
	};

	if (NUMBER < (long)0)
		NUMBER = -NUMBER;

	if (!NUMBER)
		return (-!!INDEX);

	{
		register long	TEMP_NUMBER;

		TEMP_NUMBER = NUMBER;

		if (TEMP_NUMBER < (long)0)
			TEMP_NUMBER = -TEMP_NUMBER;

		if (TEMP_NUMBER < (long)10)
			NUMBER_SIZE = 1;
		else
		{
			NUMBER_SIZE = 0;

			while (TEMP_NUMBER)
			{
				TEMP_NUMBER = TEMP_NUMBER / (long)10;
				++NUMBER_SIZE;
			}
		}
	}

	if (INDEX >= NUMBER_SIZE)
		return (-1);

	NUMBER = NUMBER / TEN_POWERS[(NUMBER_SIZE - INDEX) - 1];
	return ((char)(NUMBER - (NUMBER / 10) * 10));
}

REAL REGPARM(1)
	INT_TO_REAL(register long NUMBER)
{
	REAL				RESULT;
	register int		NEGATIVE;

	if (NUMBER == 0)
	{
		RESULT = (REAL)malloc(1 * sizeof(char));

		if (!RESULT)
			return ((REAL)0);

		(*RESULT) = 15;
		return (RESULT);
	}

	if (NUMBER < 0)
	{
		NEGATIVE = 1;
		NUMBER = -NUMBER;
	}
	else
		NEGATIVE = 0;

	{ // ALLOCATE REAL
		register int	NIBBLE;
		register char	DIGIT;
		register char	NUMBER_INDEX;
		register size_t	SIZE;

		SIZE = NEGATIVE + 1;
		NIBBLE = NEGATIVE;
		NUMBER_INDEX = 0;
		DIGIT = NUM_INDEX(NUMBER, NUMBER_INDEX);

		while (DIGIT != -1)
		{
			{
				register size_t	DIGIT_COUNTER;

				DIGIT_COUNTER = 1;

				while (DIGIT_COUNTER != 65535)
				{
					register char	ITERATED_DIGIT;

					ITERATED_DIGIT =
						NUM_INDEX(NUMBER, DIGIT_COUNTER + NUMBER_INDEX);

					if (
						ITERATED_DIGIT != -1 &&
						DIGIT == ITERATED_DIGIT
					)
						++DIGIT_COUNTER;
					else
						break ;
				}

				if (DIGIT_COUNTER > 15) // 8 + 8 BITS
				{
					SIZE += 2;
					NUMBER_INDEX += DIGIT_COUNTER;
					continue ;
				}
				else if (DIGIT_COUNTER >= 4) // 4 + 8 BITS
				{
					if (NIBBLE)
					{
						NIBBLE = 0;
						++SIZE;
					}
					else
					{
						NIBBLE = 1;
						SIZE += 2;
					}

					NUMBER_INDEX += DIGIT_COUNTER;
					DIGIT = NUM_INDEX(NUMBER, NUMBER_INDEX);
					continue ;
				}
			}

			if (NIBBLE)
			{
				NIBBLE = 0;
				++SIZE;
			}
			else
				NIBBLE = 1;

			++NUMBER_INDEX;
			DIGIT = NUM_INDEX(NUMBER, NUMBER_INDEX);
		}

		RESULT = (REAL)malloc(SIZE * __REAL_SIZE__);

		if (!RESULT)
			return ((REAL)0);

		while (SIZE)
		{
			RESULT[SIZE] = 0;
			--SIZE;
		}

		(*RESULT) = 0;
	} // ALLOCATE REAL

	{ // PREPARE REAL
		register int	NIBBLE;
		register size_t	INDEX;
		register char	NUMBER_INDEX;
		register char	DIGIT;

		NIBBLE = 0;
		INDEX = 0;
		NUMBER_INDEX = 0;

		if (NEGATIVE)
		{
			(*RESULT) = -16;
			NIBBLE = 1;
		}

		DIGIT = NUM_INDEX(NUMBER, NUMBER_INDEX);

		while (DIGIT != -1)
		{
			register size_t	DIGIT_COUNTER;

			DIGIT_COUNTER = 1;

			while (DIGIT_COUNTER != 65535)
			{
				register char	ITERATED_DIGIT;

				ITERATED_DIGIT =
					NUM_INDEX(NUMBER, DIGIT_COUNTER + NUMBER_INDEX);

				if (
					ITERATED_DIGIT != -1 &&
					DIGIT == ITERATED_DIGIT
				)
					++DIGIT_COUNTER;
				else
					break ;
			}

			if (DIGIT_COUNTER > 15) // 8 + 8 BITS
			{
				if (NIBBLE)
				{
					RESULT[INDEX] += 12;
					++INDEX;
					RESULT[INDEX] = DIGIT << 4;
					RESULT[INDEX] += ((DIGIT_COUNTER >> 4) & 0X0F);
					++INDEX;
					RESULT[INDEX] = (DIGIT_COUNTER & 0X0F);
				}
				else
				{
					RESULT[INDEX] = -64;
					RESULT[INDEX] += DIGIT;
					++INDEX;
					RESULT[INDEX] = ((DIGIT_COUNTER >> 4) & 0X0F) << 4;
					RESULT[INDEX] += (DIGIT_COUNTER & 0X0F) << 4;
					++INDEX;
				}

				NUMBER_INDEX += DIGIT_COUNTER;
				DIGIT = NUM_INDEX(NUMBER, NUMBER_INDEX);
				continue ;
			}
			else if (DIGIT_COUNTER >= 4) // 4 + 8 BITS
			{
				if (NIBBLE)
				{
					RESULT[INDEX] += 11;
					++INDEX;
					RESULT[INDEX] = DIGIT << 4;
					RESULT[INDEX] += DIGIT_COUNTER;
					++INDEX;
					NIBBLE = 0;
				}
				else
				{
					RESULT[INDEX] = -80;
					RESULT[INDEX] += DIGIT;
					++INDEX;
					RESULT[INDEX] = DIGIT_COUNTER << 4;
					NIBBLE = 1;
				}

				NUMBER_INDEX += DIGIT_COUNTER;
				DIGIT = NUM_INDEX(NUMBER, NUMBER_INDEX);
				continue ;
			}

			if (DIGIT_COUNTER == 1)
			{
				if (NIBBLE)
				{
					RESULT[INDEX] += DIGIT;
					++INDEX;
					NIBBLE = 0;
				}
				else
				{
					RESULT[INDEX] = DIGIT << 4;
					NIBBLE = 1;
				}
			}
			else if (DIGIT_COUNTER == 2)
			{
				if (NIBBLE)
				{
					RESULT[INDEX] += DIGIT;
					++INDEX;
					RESULT[INDEX] = DIGIT << 4;
				}
				else
				{
					RESULT[INDEX] = DIGIT << 4;
					RESULT[INDEX] += DIGIT;
					++INDEX;
				}
			}
			else if (DIGIT_COUNTER == 3)
			{
				if (NIBBLE)
				{
					RESULT[INDEX] += DIGIT;
					++INDEX;
					RESULT[INDEX] = DIGIT << 4;
					RESULT[INDEX] += DIGIT;
					++INDEX;
					NIBBLE = 0;
				}
				else
				{
					RESULT[INDEX] = DIGIT << 4;
					RESULT[INDEX] += DIGIT;
					++INDEX;
					RESULT[INDEX] = DIGIT << 4;
					NIBBLE = 1;
				}
			}

			NUMBER_INDEX += DIGIT_COUNTER;
			DIGIT = NUM_INDEX(NUMBER, NUMBER_INDEX);
		}

		if (NIBBLE)
			RESULT[INDEX] += 15;
		else
			RESULT[INDEX] = -16;
	} // PREPARE REAL

	return (RESULT);
}

/* *************************** [v] LOWER CASE [v] *************************** */
#ifndef KNR_STYLE /* STANDARD C */
real REGPARM(1)
	*int_to_real(register long number)
#else /* K&R */
real REGPARM(1)
	*int_to_real(number)
	register long	number;
#endif /* !KNR_STYLE */
{
	return ((real)INT_TO_REAL(number));
}
/* *************************** [^] LOWER CASE [^] *************************** */

/* *************************** [v] C++ (POP) [v] **************************** */
#ifdef __cplusplus /* C++ */
}
#endif /* __cplusplus */
/* *************************** [^] C++ (POP) [^] **************************** */

/* ************************ [v] TI CGT CCS (POP) [v] ************************ */
#ifdef __TI_COMPILER_VERSION__
#	pragma diag_pop /* TI CGT CCS COMPILER DIRECTIVES */
#endif /* __TI_COMPILER_VERSION__ */
/* ************************ [^] TI CGT CCS (POP) [^] ************************ */
