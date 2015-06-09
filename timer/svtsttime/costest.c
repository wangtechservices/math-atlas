#include "atlas_misc.h"
#include <assert.h>

int FAx=0, MAx=0, FAy=0, MAy=0;
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct FA_allocs
{
   void *mem, *memA;
   struct FA_allocs *next;
} *allocQ=NULL;

struct FA_allocs *NewAlloc(size_t size, struct FA_allocs *next,
                           int align, int misalign)
/*
 * Allocates size allocation that is aligned to [align], but not aligned
 * to [misalign].  Therefore, misalign > align.  Align must minimally be sizeof
 * while misalign may be 0 if we don't need to avoid a particular alignment.
 */
{
   void *vp;
   char *cp;
   struct FA_allocs *ap;
   int n, i;
   const int malign = align >= misalign ? align : misalign;

   n = size + align + align + malign;
   i = (n >> 3)<<3;
   if (n != i)
      n += n - i;
   cp = malloc(n + sizeof(struct FA_allocs));
   assert(cp);
   ap = (struct FA_allocs *) (cp + n);
   ap->mem = cp;
/*
 * Align to min alignment
 */
   ap->memA = align ? (void*) ((((size_t) cp)/align)*align + align) : cp;
/*
 * Misalign to misalign
 * We often need to make sure to unaligned addresses share the same modulo
 * so that they have the *same* degree of misalignment (so that their alignment
 * can be fixed by simple peeling), and so in this case force the address
 * modulo the misalign to be the exact align value.
 */
   if (misalign)
      ap->memA = (void*)((((size_t)ap->memA)/malign)*malign + malign + align);
   ap->next = next;
   return(ap);
}

/*
 * no-align malloc free retaining system default behavior
 */
void *NA_malloc(size_t size)
{
   return(malloc(size));
}
void *NA_calloc(size_t n, size_t size)
{
   return(calloc(n, size));
}
void NA_free(void *ptr)
{
   free(ptr);
}


/*
 * malloc/free pair that aligns data to align, but not to misalign
 */
void *FA_malloc(size_t size, int align, int misalign)
{
   if ((!misalign && align <= 8) || !size)
      return(malloc(size));
   else
   {
      allocQ = NewAlloc(size, allocQ, align, misalign);
      return(allocQ->memA);
   }
}
void *FA_calloc(size_t n, size_t size, int align, int misalign)
{
   char *cp;
   int *ip;
   double *dp;
   size_t i;
   size_t tsize;
   tsize = n * size;
   cp = FA_malloc(tsize, align, misalign);
   if (size == sizeof(int))
      for (ip=(int*)cp,i=0; i < n; i++)
        ip[i] = 0;
   else if (size == sizeof(double))
      for (dp=(double*)cp,i=0; i < n; i++)
        dp[i] = 0.0;
   else
      for (i=0; i < tsize; i++)
        cp[i] = 0;
   return(cp);
}

void FA_free(void *ptr, int align, int misalign)
/*
 * Part of malloc/free pair that aligns data to FALIGN
 */
{
   struct FA_allocs *ap, *prev;
   if (ptr)
   {
      if ((!misalign && align <= 8))
         free(ptr);
      else
      {
         for (ap=allocQ; ap && ap->memA != ptr; ap = ap->next) prev = ap;
         if (!ap)
         {
            fprintf(stderr, "Couldn't find mem=%ld\nmemQ=\n", (size_t)ptr);
            for (ap=allocQ; ap; ap = ap->next)
               fprintf(stderr, "   %ld, %ld\n", (size_t)ap->memA,
                       (size_t)ap->mem);
         }
         assert(ap);
         if (ap == allocQ)
            allocQ = allocQ->next;
         else
            prev->next = ap->next;
         free(ap->mem);
      }
   }
}

#define dumb_seed(iseed_) srand(iseed_)
#ifndef RAND_MAX  /* rather dangerous non-ansi workaround */
   #define RAND_MAX ((unsigned long)(1<<30))
#endif
#define dumb_rand() ( 0.5 - ((double)rand())/((double)RAND_MAX) )

#ifndef TEST_KERNEL_COS
   #define TEST_KERNEL_COS  FKO_COSINE 
#endif

#ifndef TRUSTED_KERNEL_COS
   #define TRUSTED_KERNEL_COS good_kernel_cos
#endif

void TEST_KERNEL_COS(const int N, const TYPE *X,  TYPE *Y);

#ifdef TREAL
#ifdef SREAL
void good_kernel_cos(const int N, const TYPE *X, TYPE *Y)
{
   int i;
   float r, z, x, y, x1, qx, hz, a;
   const float one =  1.0000000000e+0, C1  =  4.1666667908e-2, 
               C2  = -1.3888889225e-3, C3  =  2.4801587642e-5,   
               C4  = -2.7557314297e-7, C5  =  2.0875723372e-9,     
               C6  = -1.1359647598e-11,C7  =  2.8125e-1, 
               C8  =  4.0e+0,          lmark = 7.4505806e-9,   
               l1  = 3.0e-1,           l2 = 7.8125e-1,
               half = 5.0e-1;     

   for (i=0; i < N; i++, X++, Y++)
   {
      x = *X;
      y = *Y;
      x1 = (x < 0.0) ? -x : x;
      if (x1 < lmark)
      {
         r = one;
      }
      else
      {
         z = x * x;
         r  = z*(C1+z*(C2+z*(C3+z*(C4+z*(C5+z*C6)))));
         if (x1 > l1)
         {
            r = one - (0.5*z - (z*r - x*y));
         }
         else
         {
            if (x1 > l2) 
            {
               qx = C7;
            }
            else
            {
               qx = x / C8;
            }
            hz = 0.5*z-qx;
            a  = one-qx;
            r = a - (hz - (z*r-x*y));
         }
      }
      *Y = r;
   }
}
#else /* DREAL */
void good_kernel_cos(const int N, const TYPE *X, TYPE *Y)
{
   int i;
   TYPE r, z, x, y, x1, qx, hz, a;
   const double one =  1.00000000000000000000e+00, /* 0x3FF00000, 0x00000000 */
         C1  =  4.16666666666666019037e-02, /* 0x3FA55555, 0x5555554C */
         C2  = -1.38888888888741095749e-03, /* 0xBF56C16C, 0x16C15177 */
         C3  =  2.48015872894767294178e-05, /* 0x3EFA01A0, 0x19CB1590 */
         C4  = -2.75573143513906633035e-07, /* 0xBE927E4F, 0x809C52AD */
         C5  =  2.08757232129817482790e-09, /* 0x3E21EE9E, 0xBDB4B1C4 */
         C6  = -1.13596475577881948265e-11, /* 0xBDA8FAE9, 0xBE8838D4 */
         C7  =  2.8125e-1, 
         C8  =  4.0e+0,          
         lmark = 7.450580596923828125e-9,   
         l1  = 3.0e-1,           
         l2 = 7.8125e-1,
         half = 5.0e-1;     

   for (i=0; i < N; i++, X++, Y++)
   {
      x = *X;
      y = *Y;
      x1 = (x < 0.0) ? -x : x;
      if (x1 < lmark)
      {
         r = one;
      }
      else
      {
         z = x * x;
         r  = z*(C1+z*(C2+z*(C3+z*(C4+z*(C5+z*C6)))));
         if (x1 > l1)
         {
            r = one - (0.5*z - (z*r - x*y));
         }
         else
         {
            if (x1 > l2) 
            {
               qx = C7;
            }
            else
            {
               qx = x / C8;
            }
            hz = 0.5*z-qx;
            a  = one-qx;
            r = a - (hz - (z*r-x*y));
         }
      }
      *Y = r;
   }
}
#endif

int checkY(int N, TYPE *Yg, int incYg, TYPE *Yc, int incYc)
{
   int i, iret=0;
   TYPE eps, diff;
   TYPE Mjoin(PATL, epsilon)(void);
   eps = Mjoin(PATL,epsilon)();

   for (i=0; i < N; i++, Yg += incYg, Yc += incYc)
   {
      diff = *Yg - *Yc;
      diff = Mabs(diff);
      if (diff > 2.0*eps) /* FIXME: need to check appropriate mul factor */
      {
         iret = i;
         fprintf(stderr, "ERROR: Y[%d], correct=%f, computed=%f\n",
                 i, *Yg, *Yc);
      }
   }
   return(iret);
}


int CheckPad(int npad, TYPE padval, int N, TYPE *Y, int incY)
{
   int i, iret=0;

   incY = Mabs(incY);
   for (i=0; i < npad; i++)
   {
      if (Y[i] != padval)
      {
         iret = i;
         fprintf(stderr, "OVERWRITE %f IN PREPAD %d before beginning of Y!!\n",
                 Y[i], npad-i);
      }
   }
   Y += npad;
   if (incY != 1)
   {
      for (i=0; i < N*incY; i++)
      {
         if (i%incY)
         {
            if (Y[i] != padval)
            {
               iret = i;
               fprintf(stderr, "INTERNAL OVERWRITE %f AT POSITION %d!!\n",
                       Y[i], i);
            }
         }
      }
   }
   Y += 1 + (N-1)*incY;
   for (i=0; i < npad; i++)
   {
      if (Y[i] != padval)
      {
         iret = i;
         fprintf(stderr, "OVERWRITE %f IN POSTPAD %d past end of Y!!\n",
                 Y[i], i+1);
      }
   }
   return(iret);
}
#else
/*
 * Majedul: we haven't consider for complex number yet. If need to add, look
 * into other atlas's tester.
 */

#endif

static int CheckY(int npad, TYPE padval, int N, TYPE *Yg, int incYg,
                  TYPE *Yt, int incYt)
{
   int i0, i1;
   incYg = Mabs(incYg);
   incYt = Mabs(incYt);
   i0 = checkY(N, Yg+(npad SHIFT), incYg, Yt+(npad SHIFT), incYt);
   i1 = CheckPad(npad, padval, N, Yt, incYt);
   if (!i0 && !i1) return(0);
   return(1);
}
static void vecset(int N, TYPE alpha, TYPE *X)
{
   int i;
   #ifdef TCPLX
      N *= 2;
   #endif
   for (i=0; i < N; i++) X[i] = alpha;
}

static TYPE *getvec(int npad, TYPE padval, int N, int incX, int FA, int MA)
{
   TYPE *X, *x;
   int i, n;

   if (N <= 0) return(NULL);
   incX = Mabs(incX);
   n = 2*npad + 1+(N-1)*incX;
   X = FA_malloc(ATL_sizeof*n, FA, MA);
   assert(X);
   vecset(n, padval, X);
   #ifdef TCPLX
      npad *= 2;
      incX *= 2;
   #endif
   x = X + npad;
   for (i=0; i < N; i++, x += incX)
   {
      #ifdef TREAL
         *x = dumb_rand();
      #else
         fprintf(stderr, "Complex number not considered yet");
         assert(0); /* blocked code for complex */
         *x   = dumb_rand();
         x[1] = dumb_rand();
      #endif
   }
   return(X);
}

static void copyvec(int N, const TYPE *X, int incX, TYPE *Y, int incY)
{
   int i;
   #ifdef TREAL
      for (i=0; i < N; i++, X += incX, Y += incY) *Y = *X;
   #else
      incX *= 2; incY *= 2;
      for (i=0; i < N; i++, X += incX, Y += incY) { *Y = *X; Y[1] = X[1]; }
   #endif
}

static TYPE *dupvec(int npad, int N, TYPE *X, int incX, int FA, int MA)
{
   int i, n;
   TYPE *y;

   incX = Mabs(incX);
   n = 1+(N-1)*incX + 2*npad;
   y = FA_malloc(ATL_sizeof*n, FA, MA);
   assert(y);
   #ifdef TCPLX
      n *= 2;
   #endif
   for (i=0; i < n; i++) y[i] = X[i];
   return(y);
}

static TYPE *gen_dupvec(int N, TYPE padval, int npadX, TYPE *X, int incX,
                        int npadY, int incY, int FA, int MA)
{
   int i, n;
   TYPE *y, *yy, *xx=X+(npadX SHIFT);

   y = getvec(npadY, padval, N, incY, FA, MA);
   yy = y + (npadY SHIFT);
   if (incY < 1) yy -= ((N-1)SHIFT) * incY;
   if (incX < 1) xx -= ((N-1)SHIFT) * incX;
   copyvec(N, xx, incX, yy, incY);
   return(y);
}

int DoTest(int N, int incX, int incY)
{
   int iret;
   const int npad=Mmax(4*Mabs(incY), 16);
   const TYPE padval=(-2271.0);
   TYPE *Yg, *Yt, *X, *x, *y;

   X = getvec(0, padval, N, incX, FAx, MAx);  /* no padding for read-only X */
   
   Yg = getvec(npad, padval, N, incY, FAy, MAy);
   Yt = dupvec(npad, N, Yg, incY, FAy, MAy);

   x = X;
   y = Yg + (npad SHIFT);
   if (incX < 1) x -= ((N-1)SHIFT) * incX;
   if (incY < 1) y -= ((N-1)SHIFT) * incY;
   
   TRUSTED_KERNEL_COS(N, x, y);
   
   y = Yt + (npad SHIFT);
   if (incY < 1) y -= ((N-1)SHIFT) * incY;

   TEST_KERNEL_COS(N, x, y);

   iret = CheckY(npad, padval, N, Yg, incY, Yt, incY);

   FA_free(X, FAx, MAx);
   FA_free(Yg, FAy, MAy);
   FA_free(Yt, FAy, MAy);
   return(iret);
}

int DoAllTests(int nN, int *Ns, int nX, int *Xs, int nY, int *Ys)
{
   int in, ix, iy, ia, ib, iret=0, i=0, j, k;
   char *passfail;

   char *t1="  ITST         N  incX  incY    TEST";
   char *t2="======  ========  ====  ====  ======";
   fprintf(stdout, "%s\n", t1);
   fprintf(stdout, "%s\n", t2);
   for (in=0; in < nN; in++)
   {
      for (ix=0; ix < nX; ix++)
      {
         for (iy=0; iy < nY; iy++)
         {
            j = DoTest(Ns[in], Xs[ix], Ys[iy]);
            iret += j;
            if (j == 0) passfail = "PASSED";
            else passfail = "FAILED";
            fprintf(stdout, "%6d %9d %5d %5d  %s\n",
                    i, Ns[in], Xs[ix], Ys[iy], passfail);
            i++;
         }
      }
   }
   if (iret == 0) fprintf(stdout, "ALL COPY SANITY TESTS PASSED.\n\n");
   else fprintf(stdout, "%d of %d COPY TESTS FAILED!!\n\n", iret, i);
   return(iret);
}

void PrintUsage(char *nam)
{
   fprintf(stderr,
           "USAGE: %s -N # n1 ... n# -n <n> -X # x1 ... x# -Y # y1 ... y#\n",
           nam);
   exit(-1);
}

void GetFlags(int nargs, char **args, int *nN, int **Ns, int *nX, int **incXs,
              int *nY, int **incYs)
{
   int i, j, k, ig;

   *nY = *nX = *nN = -1;

   for (i=1; i < nargs; i++)
   {
      if (args[i][0] != '-') PrintUsage(args[0]);
      if (i == nargs-1) PrintUsage(args[0]);
      switch(args[i][1])
      {
      case 'F':
         j = args[i][2] != 'y';
         k = atoi(args[++i]);
         if (j)
         {
            if (k < 0)
               MAx = -k;
            else
               FAx = k;
         }
         else
         {
            if (k < 0)
               MAy = -k;
            else
               FAy = k;
         }
         break;
      case 'b':
      case 'a':
         ig = atoi(args[++i]);
         if (ig > nargs-i) PrintUsage(args[0]);
         i += ig SHIFT;
         break;
      case 'Y':
         *nY = atoi(args[++i]);
         if (*nY > nargs-i) PrintUsage(args[0]);
         *incYs = malloc((*nY)*sizeof(int));
         assert(*incYs);
         for (j=0; j < *nY; j++) (*incYs)[j] = atoi(args[++i]);
         break;
      case 'X':
         *nX = atoi(args[++i]);
         if (*nX > nargs-i) PrintUsage(args[0]);
         *incXs = malloc((*nX)*sizeof(int));
         assert(*incXs);
         for (j=0; j < *nX; j++) (*incXs)[j] = atoi(args[++i]);
         break;
      case 'N':
         *nN = atoi(args[++i]);
         if (*nN > nargs-i) PrintUsage(args[0]);
         *Ns = malloc((*nN)*sizeof(int));
         assert(*Ns);
         for (j=0; j < *nN; j++) (*Ns)[j] = atoi(args[++i]);
         break;
      case 'y':
         *nY = 1;
         *incYs = malloc(sizeof(int));
         assert(*incYs);
         **incYs = atoi(args[++i]);
         break;
      case 'x':
         *nX = 1;
         *incXs = malloc(sizeof(int));
         assert(*incXs);
         **incXs = atoi(args[++i]);
         break;
      case 'n':
         *nN = 1;
         *Ns = malloc(sizeof(int));
         assert(*Ns);
         **Ns = atoi(args[++i]);
         break;
      default:
         PrintUsage(args[0]);
      }
   }
   if (*nN < 0)
   {
      *nN = 4;
      *Ns = malloc((*nN)*sizeof(int));
      assert(*Ns);
      **Ns = 777;
      (*Ns)[1] = 1;
      (*Ns)[2] = 3;
      (*Ns)[3] = 7;
   }
   if (*nX < 0)
   {
      *nX = 1;
      *incXs = malloc((*nX)*sizeof(int));
      assert(*incXs);
      **incXs = 1;
   }
   if (*nY < 0)
   {
      *nY = 1;
      *incYs = malloc((*nY)*sizeof(int));
      assert(*incYs);
      **incYs = 1;
   }
   if (FAx < sizeof(TYPE))
      FAx = sizeof(TYPE);
   if (FAy < sizeof(TYPE))
      FAy = sizeof(TYPE);
}

int main(int nargs, char **args)
{
   int nN, nX, nY, nal, nbe;
   int *Ns, *incXs, *incYs;
   int ierr;
   GetFlags(nargs, args, &nN, &Ns, &nX, &incXs, &nY, &incYs);
   ierr = DoAllTests(nN, Ns, nX, incXs, nY, incYs);
   free(incXs);
   free(incYs);
   return(ierr);
}