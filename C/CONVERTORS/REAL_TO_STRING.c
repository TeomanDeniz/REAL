/******************************************************************************\
# C - REAL_TO_STRING                             #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2025/05/31 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GNU       :: Update - 2025/06/07 : #    ::::!!!1!!1!!!1!!!::     #
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
#	pragma map(REAL_TO_STRING, "RTOSMT01")
#	pragma map(real_to_string, "RTOSMT02")
#endif /* __MVS__ */
/* *************************** [^] MVS LINKER [^] *************************** */

/* **************************** [v] INCLUDES [v] **************************** */
#include "../../REAL.h" /*
# define REAL__RECURRING_DECIMAL_LIMIT
# define REAL__FRACTION_LIMIT
#typedef REAL;
#typedef real;
#        */
#include "../../LIBCMT/ENVIRONMENTS/KNR_STYLE.h" /*
# define KNR_STYLE
#        */
#include <stdlib.h> /*
#typedef size_t;
#   void *malloc(size_t);
#        */
/* **************************** [^] INCLUDES [^] **************************** */

#ifndef KNR_STYLE /* STANDARD C */
char
	*REAL_TO_STRING(REAL NUMBER)
#else /* K&R */
char
	*REAL_TO_STRING(NUMBER)
	REAL	NUMBER;
#endif /* !KNR_STYLE */
{
	char			*RESULT;
	register int	NEGATIVE;

	if (!NUMBER)
		return ((char *)0);

	NEGATIVE = (((((*NUMBER) & -16) >> 4) & 15) == 15);

	if ((*NUMBER) == 15)
	{
		RESULT = (char *)malloc(2 * sizeof(char));

		if (!RESULT)
			return ((char *)0);

		RESULT[0] = '0';
		RESULT[1] = 0;
		return (RESULT);
	}

	{ // FIRST BYTE SPECIAL COMMANDS
		register char	HIGH_NIBBLE;
		register char	LOW_NIBBLE;

		HIGH_NIBBLE = ((*NUMBER & -16) >> 4) & 15;
		LOW_NIBBLE = *NUMBER & 15;
		NEGATIVE = (HIGH_NIBBLE == 15);

		if (
			(HIGH_NIBBLE >= 11 && HIGH_NIBBLE <= 14) &&
			!(LOW_NIBBLE >= 0 && LOW_NIBBLE <= 9)
		)
		{
			RESULT = (char *)malloc(5 * sizeof(char));

			if (!RESULT)
				return ((char *)0);

			RESULT[0] = 'E';
			RESULT[1] = 'R';
			RESULT[2] = 'R';
			RESULT[3] = '0';
			RESULT[4] = 0;
			return (RESULT);
		}

		if (NEGATIVE)
		{
			if (LOW_NIBBLE == 0)
			{ // -INF
				RESULT = (char *)malloc(5 * sizeof(char));

				if (!RESULT)
					return ((char *)0);

				RESULT[0] = '-';
				RESULT[1] = 'I';
				RESULT[2] = 'N';
				RESULT[3] = 'F';
				RESULT[4] = 0;
				return (RESULT);
			} // -INF

			if (LOW_NIBBLE == 15)
			{ // NAN
				RESULT = (char *)malloc(4 * sizeof(char));

				if (!RESULT)
					return ((char *)0);

				RESULT[0] = 'N';
				RESULT[1] = 'A';
				RESULT[2] = 'N';
				RESULT[3] = 0;
				return (RESULT);
			} // NAN
		}
		else if (HIGH_NIBBLE == 0 && LOW_NIBBLE == 0)
		{ // INF
			RESULT = (char *)malloc(4 * sizeof(char));

			if (!RESULT)
				return ((char *)0);

			RESULT[0] = 'I';
			RESULT[1] = 'N';
			RESULT[2] = 'F';
			RESULT[3] = 0;
			return (RESULT);
		} // INF

		if (
			(HIGH_NIBBLE == 15 && LOW_NIBBLE == 0) ||
			(HIGH_NIBBLE == 10) ||
			(HIGH_NIBBLE == 15 && LOW_NIBBLE == 10)
		)
		{
			RESULT = (char *)malloc(5 * sizeof(char));

			if (!RESULT)
				return ((char *)0);

			RESULT[0] = 'E';
			RESULT[1] = 'R';
			RESULT[2] = 'R';
			RESULT[3] = '1';
			RESULT[4] = 0;
			return (RESULT);
		}
	} // FIRST BYTE SPECIAL COMMANDS

	{ // ALLOCATE RESULT
		register size_t	SIZE;
		register size_t	SIZE_RDN;
		register char	PACK;
		REAL			ORIGINAL_NUMBER;

		ORIGINAL_NUMBER = NUMBER;
		SIZE = 1;
		PACK = 0;
		SIZE_RDN = 0;

		if (NEGATIVE)
		{
			register char	LOW_NIBBLE;

			LOW_NIBBLE = (*NUMBER) & 15;
			++SIZE;

			if (!(LOW_NIBBLE >= 1 && LOW_NIBBLE <= 9)) // PACK
				PACK = LOW_NIBBLE;
			else
				++SIZE;

			++NUMBER;
		}

		{
			register int	NIBBLE;
			register int	_1010; // FRACTION OR RDN
			register char	PACK_VALUE;
			register size_t	PACK_SIZE;

			NIBBLE = 0;
			PACK_VALUE = 10;
			PACK_SIZE = 0;
			_1010 = 0;

			while (1)
			{
				register char	CURRENT_NIBBLE;

				if (NIBBLE)
					CURRENT_NIBBLE = (*NUMBER) & 15;
				else
					CURRENT_NIBBLE = (((*NUMBER) & -16) >> 4) & 15;

				if (CURRENT_NIBBLE == 15 && PACK == 0)
					break ;

				if (PACK == 11)
				{
					if (PACK_VALUE == 10)
						PACK_VALUE = CURRENT_NIBBLE;
					else
					{
						PACK_SIZE = CURRENT_NIBBLE;

						if (PACK_SIZE != 0) // IF NOT STILL 0
						{
							if (_1010 != 2)
								SIZE += PACK_SIZE;
							else
								SIZE_RDN += PACK_SIZE;

							PACK_SIZE = 0;
						}

						PACK_VALUE = 10;
						PACK = 0;
					}
				}
				else if (PACK == 12 || PACK == 22)
				{
					if (PACK_VALUE == 10)
						PACK_VALUE = CURRENT_NIBBLE;
					else
					{
						if (PACK == 22)
						{
							PACK_SIZE += CURRENT_NIBBLE;

							if (PACK_SIZE != 0) // IF NOT STILL 0
							{
								if (_1010 != 2)
									SIZE += PACK_SIZE;
								else
									SIZE_RDN += PACK_SIZE;

								PACK_SIZE = 0;
							}

							PACK_VALUE = 10;
							PACK = 0;
						}
						else /* PACK == 12 */
						{
							PACK_SIZE = CURRENT_NIBBLE << 4;
							PACK += 10;
						}
					}
				}
				else if (PACK == 13 || PACK == 23 || PACK == 33)
				{
					if (PACK_VALUE == 10)
						PACK_VALUE = CURRENT_NIBBLE;
					else
					{
						if (PACK == 33)
						{
							PACK_SIZE += CURRENT_NIBBLE;

							if (PACK_SIZE != 0) // IF NOT STILL 0
							{
								if (_1010 != 2)
									SIZE += PACK_SIZE;
								else
									SIZE_RDN += PACK_SIZE;

								PACK_SIZE = 0;
							}

							PACK_VALUE = 10;
							PACK = 0;
						}
						else if (PACK == 23)
						{
							PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 4;
							PACK += 10;
						}
						else /* PACK == 13 */
						{
							PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 8;
							PACK += 10;
						}
					}
				}
				else if (PACK == 14 || PACK == 24 || PACK == 34 || PACK == 44)
				{
					if (PACK_VALUE == 10)
						PACK_VALUE = CURRENT_NIBBLE;
					else
					{
						if (PACK == 44)
						{
							PACK_SIZE += CURRENT_NIBBLE;

							if (PACK_SIZE != 0) // IF NOT STILL 0
							{
								if (_1010 != 2)
									SIZE += PACK_SIZE;
								else
									SIZE_RDN += PACK_SIZE;

								PACK_SIZE = 0;
							}

							PACK_VALUE = 10;
							PACK = 0;
						}
						else if (PACK == 34)
						{
							PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 4;
							PACK += 10;
						}
						else if (PACK == 24)
						{
							PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 8;
							PACK += 10;
						}
						else /* PACK == 14 */
						{
							PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 12;
							PACK += 10;
						}
					}
				}
				else if (
					PACK == 0 &&
					(
						CURRENT_NIBBLE == 11 ||
						CURRENT_NIBBLE == 12 ||
						CURRENT_NIBBLE == 13 ||
						CURRENT_NIBBLE == 14
					)
				)
					PACK = CURRENT_NIBBLE;
				else if (CURRENT_NIBBLE == 10)
				{
					if (_1010 == 0)
					{
						++SIZE;
						_1010 = 1;
					}
					else if (_1010 == 1)
						_1010 = 2;
					else
					{
						RESULT = (char *)malloc(5 * sizeof(char));

						if (!RESULT)
							return ((char *)0);

						RESULT[0] = 'E';
						RESULT[1] = 'R';
						RESULT[2] = 'R';
						RESULT[3] = '2';
						RESULT[4] = 0;
						return (RESULT);
					}
				}
				else
				{
					if (_1010 != 2)
						++SIZE;
					else
						++SIZE_RDN;
				}

				if (!NIBBLE)
					NIBBLE = 1;
				else
				{
					NIBBLE = 0;
					++NUMBER;
				}
			}
		}

		if (SIZE_RDN != 0)
		{
			SIZE_RDN += 3; // FOR "..."

			if (SIZE_RDN > REAL__RECURRING_DECIMAL_LIMIT)
				SIZE_RDN = SIZE_RDN * 2;
			else
				SIZE_RDN = SIZE_RDN * REAL__RECURRING_DECIMAL_LIMIT;

			SIZE += SIZE_RDN;
		}

		RESULT = (char *)malloc((SIZE + 1) * sizeof(char *));

		if (!RESULT)
			return ((char *)0);

		NUMBER = ORIGINAL_NUMBER;
	} // ALLOCATE RESULT

	{ // PREPARE RESULT
		register int	NIBBLE;
		register int	_1010; // FRACTION OR RDN
		register size_t	SIZE_RDN;
		register char	PACK_VALUE;
		register size_t	PACK_SIZE;
		register size_t	INDEX;
		register char	PACK;

		NIBBLE = 0;
		PACK_VALUE = 10;
		PACK_SIZE = 0;
		INDEX = 0;
		PACK = 0;
		_1010 = 0;
		SIZE_RDN = 0;

		if (NEGATIVE)
		{
			register char	LOW_NIBBLE;

			LOW_NIBBLE = (*NUMBER) & 15;
			(*RESULT) = '-';
			++INDEX;

			if (!(LOW_NIBBLE >= 1 && LOW_NIBBLE <= 9)) // PACK
				PACK = LOW_NIBBLE;
			else
			{
				switch (LOW_NIBBLE)
				{
					case (0):
					{
						RESULT[INDEX] = '0';
						++INDEX;
					}
					break ;
					case (1):
					{
						RESULT[INDEX] = '1';
						++INDEX;
					}
					break ;

					case (2):
					{
						RESULT[INDEX] = '2';
						++INDEX;
					}
					break ;
					case (3):
					{
						RESULT[INDEX] = '3';
						++INDEX;
					}
					break ;
					case (4):
					{
						RESULT[INDEX] = '4';
						++INDEX;
					}
					break ;
					case (5):
					{
						RESULT[INDEX] = '5';
						++INDEX;
					}
					break ;
					case (6):
					{
						RESULT[INDEX] = '6';
						++INDEX;
					}
					break ;
					case (7):
					{
						RESULT[INDEX] = '7';
						++INDEX;
					}
					break ;
					case (8):
					{
						RESULT[INDEX] = '8';
						++INDEX;
					}
					break ;
					case (9):
					{
						RESULT[INDEX] = '9';
						++INDEX;
					}
					break ;
					case (10):
					{
						RESULT[INDEX] = '.';
						++INDEX;
						_1010 = 1;
					}
					break ;
					case (11):
					{
						PACK = LOW_NIBBLE;
					}
					break ;
					case (12):
					{
						PACK = LOW_NIBBLE;
					}
					break ;
					case (13):
					{
						PACK = LOW_NIBBLE;
					}
					break ;
					case (14):
					{
						PACK = LOW_NIBBLE;
					}
					break ;
				}
			}

			++NUMBER;
		}

		while (1)
		{
			register char	CURRENT_NIBBLE;

			if (NIBBLE)
				CURRENT_NIBBLE = (*NUMBER) & 15;
			else
				CURRENT_NIBBLE = (((*NUMBER) & -16) >> 4) & 15;

			if (PACK == 0)
			{
				switch (CURRENT_NIBBLE)
				{
					case (0):
					{
						RESULT[INDEX] = '0';
						++INDEX;
					}
					break ;
					case (1):
					{
						RESULT[INDEX] = '1';
						++INDEX;
					}
					break ;

					case (2):
					{
						RESULT[INDEX] = '2';
						++INDEX;
					}
					break ;
					case (3):
					{
						RESULT[INDEX] = '3';
						++INDEX;
					}
					break ;
					case (4):
					{
						RESULT[INDEX] = '4';
						++INDEX;
					}
					break ;
					case (5):
					{
						RESULT[INDEX] = '5';
						++INDEX;
					}
					break ;
					case (6):
					{
						RESULT[INDEX] = '6';
						++INDEX;
					}
					break ;
					case (7):
					{
						RESULT[INDEX] = '7';
						++INDEX;
					}
					break ;
					case (8):
					{
						RESULT[INDEX] = '8';
						++INDEX;
					}
					break ;
					case (9):
					{
						RESULT[INDEX] = '9';
						++INDEX;
					}
					break ;
					case (10):
					{
						if (_1010 == 0)
						{
							RESULT[INDEX] = '.';
							++INDEX;
							_1010 = 1;
						}
						else if (_1010 == 1)
							_1010 = INDEX;
					}
					break ;
					case (11):
					{
						PACK = CURRENT_NIBBLE;
					}
					break ;
					case (12):
					{
						PACK = CURRENT_NIBBLE;
					}
					break ;
					case (13):
					{
						PACK = CURRENT_NIBBLE;
					}
					break ;
					case (14):
					{
						PACK = CURRENT_NIBBLE;
					}
					break ;
					case (15):
					{
						if (_1010 > 1)
						{
							register size_t	FINISH_INDEX;
							register size_t	REPEAT_ITERATE;
							register size_t	SIZE_OF_RDN;
							register size_t	RDN_INDEX;

							FINISH_INDEX = INDEX;
							SIZE_OF_RDN = FINISH_INDEX - _1010;
							RDN_INDEX = _1010;

							{
								register size_t RDN_ORIGINAL_SIZE;

								RDN_ORIGINAL_SIZE = SIZE_OF_RDN;

								if (SIZE_OF_RDN > REAL__RECURRING_DECIMAL_LIMIT)
									SIZE_OF_RDN = SIZE_OF_RDN * 2;
								else
									SIZE_OF_RDN = SIZE_OF_RDN *
										REAL__RECURRING_DECIMAL_LIMIT;

								if (SIZE_OF_RDN)
									SIZE_OF_RDN -= RDN_ORIGINAL_SIZE;
							}

							while (SIZE_OF_RDN)
							{
								RESULT[INDEX] = RESULT[RDN_INDEX];
								++RDN_INDEX;
								++INDEX;
								--SIZE_OF_RDN;

								if (RDN_INDEX >= FINISH_INDEX)
									RDN_INDEX = _1010;
							}
						}

						RESULT[INDEX] = '.';
						RESULT[INDEX + 1] = '.';
						RESULT[INDEX + 2] = '.';
						RESULT[INDEX + 3] = 0;
						return (RESULT);
					}
					break ;
				}
			}
			else if (PACK == 11)
			{
				if (PACK_VALUE == 10)
					PACK_VALUE = CURRENT_NIBBLE;
				else
				{
					PACK_SIZE = CURRENT_NIBBLE;

					if (PACK_SIZE == 0) // IF STILL 0
					{ // SKIP
						PACK_VALUE = 10;
						PACK = 0;
					}
					else
					{
						while (PACK_SIZE)
						{
							RESULT[INDEX] = PACK_VALUE + 48;
							--PACK_SIZE;
							++INDEX;
						}

						PACK_VALUE = 10;
						PACK = 0;
					}
				}
			}
			else if (PACK == 12 || PACK == 22)
			{
				if (PACK_VALUE == 10)
					PACK_VALUE = CURRENT_NIBBLE;
				else
				{
					if (PACK == 22)
					{
						PACK_SIZE += CURRENT_NIBBLE;

						while (PACK_SIZE)
						{
							RESULT[INDEX] = PACK_VALUE + 48;
							--PACK_SIZE;
							++INDEX;
						}

						PACK_VALUE = 10;
						PACK = 0;
					}
					else /* PACK == 12 */
					{
						PACK_SIZE = CURRENT_NIBBLE << 4;
						PACK += 10;
					}
				}
			}
			else if (PACK == 13 || PACK == 23 || PACK == 33)
			{
				if (PACK_VALUE == 10)
					PACK_VALUE = CURRENT_NIBBLE;
				else
				{
					if (PACK == 33)
					{
						PACK_SIZE += CURRENT_NIBBLE;

						while (PACK_SIZE)
						{
							RESULT[INDEX] = PACK_VALUE + 48;
							--PACK_SIZE;
							++INDEX;
						}

						PACK_VALUE = 10;
						PACK = 0;
					}
					else if (PACK == 23)
					{
						PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 4;
						PACK += 10;
					}
					else /* PACK == 13 */
					{
						PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 8;
						PACK += 10;
					}
				}
			}
			else if (PACK == 14 || PACK == 24 || PACK == 34 || PACK == 44)
			{
				if (PACK_VALUE == 10)
					PACK_VALUE = CURRENT_NIBBLE;
				else
				{
					if (PACK == 44)
					{
						PACK_SIZE += CURRENT_NIBBLE;

						while (PACK_SIZE)
						{
							RESULT[INDEX] = PACK_VALUE + 48;
							--PACK_SIZE;
							++INDEX;
						}

						PACK_VALUE = 10;
						PACK = 0;
					}
					else if (PACK == 34)
					{
						PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 4;
						PACK += 10;
					}
					else if (PACK == 24)
					{
						PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 8;
						PACK += 10;
					}
					else /* PACK == 14 */
					{
						PACK_SIZE = ((size_t)CURRENT_NIBBLE) << 12;
						PACK += 10;
					}
				}
			}

			if (!NIBBLE)
				NIBBLE = 1;
			else
			{
				NIBBLE = 0;
				++NUMBER;
			}
		}
	} // PREPARE RESULT

	return (RESULT);
}

/* *************************** [v] LOWER CASE [v] *************************** */
#ifndef KNR_STYLE /* STANDARD C */
char
	*real_to_string(real number)
#else /* K&R */
char
	*real_to_string(number)
	real	number;
#endif /* !KNR_STYLE */
{
	return (REAL_TO_STRING(number));
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
