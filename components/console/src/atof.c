#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

#define MAXE 308

// flags
#define SIGN    0x01
#define ESIGN   0x02
#define DECP    0x04


// MACROS

#define Ise(c)          	((c == 'e') || (c == 'E'))
#define Issign(c)       	((c == '-') || (c == '+'))
#define Val(c)          	((c - '0'))

// FUNCTIONS

/*
 * [atw] multiply 64 bit accumulator by 10 and add digit.
 * The KA/CA way to do this should be to use
 * a 64-bit integer internally and use "adjust" to
 * convert it to float at the end of processing.
 */
static int
ten_mul (double *acc, int digit)
{
  /* [atw] Crude, but effective (at least on a KB)...
   */
  *acc *= 10;
  *acc += digit;

  return 0;			/* no overflow */
}				// ten_mul()


/*
 * compute 10**x by successive squaring.
 */

static double
_exp10 (unsigned x)
{
  static double powtab[] = { 1.0,
    10.0,
    100.0,
    1000.0,
    10000.0
  };

  if (x < (sizeof (powtab) / sizeof (double)))
    return powtab[x];
  else if (x & 1)
    return 10.0 * _exp10 (x - 1);
  else
    return _exp10 (x / 2) * _exp10 (x / 2);
}				// _exp10()


/*
 * return (*acc) scaled by 10**dexp.
 */
static double
adjust (double *acc, int dexp, int sign)
	/* *acc    the 64 bit accumulator */
	/* dexp    decimal exponent       */
	/* sign    sign flag              */
{
  double r, res;

  r = *acc;
  if (sign)
    r = -r;
  if (dexp == 0)
    return r;

  if (dexp < 0)
    {
      res = r / _exp10 (abs (dexp));
    }
  else
    {
      res = r * _exp10 (abs (dexp));
    }
  if ((res == 0 && r != 0) || res == HUGE_VAL || res == -HUGE_VAL)
    {
      errno = ERANGE;
    }
  return res;
}				// adjust()

double
atof (const char *nptr)
{

  double accum = 0.0;
  int flags = 0;
  int texp = 0;
  int e = 0;


  double retval;

  while (isspace (*(const unsigned char *)nptr))
    nptr++;
  if (*nptr == '\0')
    {				/* just leading spaces */
      return 0.0;
    }


  if (Issign (*nptr))
    {
      if (*nptr == '-')
	flags = SIGN;
      if (*++nptr == '\0')
	{			/* "+|-" : should be an error ? */
	  return 0.0;
	}
    }

  for (; (isdigit (*(const unsigned char *)nptr) || (*nptr == '.')); nptr++)
    {
      if (*nptr == '.')
	flags |= DECP;
      else
	{
	  if (ten_mul (&accum, Val (*nptr)))
	    texp++;
	  if (flags & DECP)
	    texp--;
	}
    }

  if (Ise (*nptr))
    {
      if (*++nptr != '\0')	/* skip e|E */
	{			/* ! ([nptr]xxx[.[yyy]]e)  */

	  while (isspace (*(const unsigned char *)nptr))
	    nptr++;		/* Ansi allows spaces after e */
	  if (*nptr != '\0')
	    {			/*  ! ([nptr]xxx[.[yyy]]e[space])  */

	      if (Issign (*nptr))
		if (*nptr++ == '-')
		  flags |= ESIGN;

	      if (*nptr != '\0')
		{		/*  ! ([nptr]xxx[.[yyy]]e[nptr])  -- error?? */

		  for (; isdigit (*(const unsigned char *)nptr); nptr++)
		    if (e < MAXE)	/* prevent from grossly overflowing */
		      e = e * 10 + Val (*nptr);

		  /* dont care what comes after this */
		  if (flags & ESIGN)
		    texp -= e;
		  else
		    texp += e;
		}
	    }
	}
    }

  retval = adjust (&accum, (int) texp, (int) (flags & SIGN));

  return retval;
}

