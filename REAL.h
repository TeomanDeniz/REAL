/******************************************************************************\
# H - REAL                                       #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2025/05/31 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GNU       :: Update - 2025/06/05 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

#ifndef REAL_H
#	define REAL_H 202506 /* VERSION */

/* **************************** [v] INCLUDES [v] **************************** */
#	include "LIBCMT/ENVIRONMENTS/KNR_STYLE.h" /*
#	 define KNR_STYLE
#	        */
#	include "LIBCMT/ATTRIBUTES/REGPARM.h" /*
#	 define REGPARM()
#	        */
/* **************************** [^] INCLUDES [^] **************************** */

/* ***************************** [v] MACROS [v] ***************************** */
#	ifndef REAL__RECURRING_DECIMAL_LIMIT
#		define REAL__RECURRING_DECIMAL_LIMIT 8
#	endif /* REAL__FRACTION_LIMIT */

#	ifndef REAL__FRACTION_LIMIT
#		define REAL__FRACTION_LIMIT 256
#	endif /* REAL__FRACTION_LIMIT */

#	define __REAL_SIZE__ sizeof(char)
/* ***************************** [^] MACROS [^] ***************************** */

/* **************************** [v] TYPEDEFS [v] **************************** */
typedef char	*REAL;
typedef char	*real;
/* **************************** [^] TYPEDEFS [^] **************************** */

/* *************************** [v] PROTOTYPES [v] *************************** */
#	ifndef KNR_STYLE /* STANDARD C */
/* [v] CONVERTORS *********************************************************** */
extern char				*REAL_TO_STRING(REAL);
extern char				*real_to_string(real);
extern REAL REGPARM(1)	INT_TO_REAL(register long);
extern real REGPARM(1)	int_to_real(register long);
/* [^] CONVERTORS *********************************************************** */
#	else /* K&R */
/* [v] CONVERTORS *********************************************************** */
extern char				*REAL_TO_STRING();
extern char				*real_to_string();
extern REAL REGPARM(1)	INT_TO_REAL();
extern real REGPARM(1)	int_to_real();
/* [^] CONVERTORS *********************************************************** */
#	endif /* !KNR_STYLE */
/* *************************** [^] PROTOTYPES [^] *************************** */

#endif /* !REAL_H */
