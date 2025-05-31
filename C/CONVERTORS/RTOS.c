/******************************************************************************\
# C - RTOS                                       #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2025/05/31 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License -           :: Update - 2025/05/31 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/* **************************** [v] INCLUDES [v] **************************** */
#include "../../REAL.h" /*
# define REAL_SIZE
#typedef REAL;
#        */
#include "../../LIBCMT/ENVIRONMENTS/KNR_STYLE.h" /*
# define KNR_STYLE
#        */
#include <stdlib.h> /*
#typedef size_t;
#   void *malloc(size_t);
#   void free(void *);
#        */
/* **************************** [^] INCLUDES [^] **************************** */

#ifndef KNR_STYLE /* STANDARD C */
char
	*RTOS(REAL NUMBER)
#else /* K&R */
char
	*RTOS(NUMBER)
	REAL	NUMBER;
#endif /* !KNR_STYLE */
{
	char			*RESULT;
	register int	NEGATIVE;

	if (!NUMBER)
		return ((char *)0);

	NEGATIVE = (((((*NUMBER) & -16) >> 4) & 15) == 15);
	RESULT = (char *)0;

	if ((*NUMBER) == 15 || (*NUMBER) == -1)
	{
		RESULT = (char *)malloc(2 * REAL_SIZE);

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
			RESULT = (char *)malloc(4 * REAL_SIZE);

			if (!RESULT)
				return ((char *)0);

			RESULT[0] = 'E';
			RESULT[1] = 'R';
			RESULT[2] = 'R';
			RESULT[3] = 0;
			return (RESULT);
		}

		if (NEGATIVE)
		{
			if (LOW_NIBBLE == 0)
			{ // -INF
				RESULT = (char *)malloc(5 * REAL_SIZE);

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
				RESULT = (char *)malloc(4 * REAL_SIZE);

				if (!RESULT)
					return ((char *)0);

				RESULT[0] = 'N';
				RESULT[1] = 'A';
				RESULT[2] = 'N';
				RESULT[3] = 0;
				return (RESULT);
			} // NAN
		}
		else if (HIGH_NIBBLE == 0 || LOW_NIBBLE == 0)
		{ // INF
			RESULT = (char *)malloc(4 * REAL_SIZE);

			if (!RESULT)
				return ((char *)0);

			RESULT[0] = 'I';
			RESULT[1] = 'N';
			RESULT[2] = 'F';
			RESULT[3] = 0;
			return (RESULT);
		} // INF
	} // FIRST BYTE SPECIAL COMMANDS

	{ // ALLOCATE RESULT
		register size_t	SIZE;
		register char	PACK;
		REAL			ORIGINAL_NUMBER;

		ORIGINAL_NUMBER = NUMBER;
		SIZE = 1;
		PACK = 0;

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
			register int	EOR;
			register int	NIBBLE;
			register char	PACK_VALUE;
			register size_t	PACK_SIZE;

			NIBBLE = 0;
			EOR = 0;
			PACK_VALUE = 10;
			PACK_SIZE = 0;

			while (!EOR)
			{
				register char	CURRENT_NIBBLE;

				if (NIBBLE)
					CURRENT_NIBBLE = (*NUMBER) & 15;
				else
					CURRENT_NIBBLE = (((*NUMBER) & -16) >> 4) & 15;

				if (CURRENT_NIBBLE == 15 && PACK == 0)
					EOR = 1;

				if (PACK == 11)
				{
					if (PACK_VALUE == 10)
						PACK_VALUE = CURRENT_NIBBLE;
					else
					{
						PACK_SIZE = CURRENT_NIBBLE;

						if (PACK_SIZE != 0) // IF NOT STILL 0
						{
							SIZE += PACK_SIZE;
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
								SIZE += PACK_SIZE;
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
								SIZE += PACK_SIZE;
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
								SIZE += PACK_SIZE;
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
				else
					++SIZE;

				if (!NIBBLE)
					NIBBLE = 1;
				else
				{
					NIBBLE = 0;
					++NUMBER;
				}
			}
		}

		RESULT = (char *)malloc((SIZE + 1) * sizeof(char *));

		if (!RESULT)
			return ((char *)0);

		NUMBER = ORIGINAL_NUMBER;
	} // ALLOCATE RESULT

	{ // PREPARE RESULT
		register int	NIBBLE;
		register char	PACK_VALUE;
		register size_t	PACK_SIZE;
		register size_t	INDEX;
		register char	PACK;

		NIBBLE = 0;
		PACK_VALUE = 10;
		PACK_SIZE = 0;
		INDEX = 0;
		PACK = 0;

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
						RESULT[INDEX] = '.';
						++INDEX;
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
						RESULT[INDEX] = 0;
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

#ifndef KNR_STYLE /* STANDARD C */
char
	*rtos(real number)
#else /* K&R */
char
	*rtos(number)
	real	number;
#endif /* !KNR_STYLE */
{
	return (RTOS(number));
}
