/* This file generated by /home/whaley/TEST/ATLAS3.9.39.0/obj64/..//tune/blas/gemm/txover.c
 */
#ifndef ATL_TXOVER_H
   #define ATL_TXOVER_H 1

   #define ATL_PDIM 2
static const int ATL_tmmNN_SQmnk_XO[2] = 
   {109,118};
static const int ATL_tmmNT_SQmnk_XO[2] = 
   {109,118};
static const int ATL_tmmTN_SQmnk_XO[2] = 
   {109,118};
#define ATL_tmmTT_SQmnk_XO ATL_tmmNN_SQmnk_XO
static const int *ATL_tmm_SQmnk_XO[4] =
{ATL_tmmNN_SQmnk_XO, ATL_tmmNT_SQmnk_XO,
 ATL_tmmTN_SQmnk_XO, ATL_tmmTT_SQmnk_XO};
static const int ATL_tmmNN_SmnLk_XO[18] = 
   {0,0,0,0,0,0,6249,6249,0,2669,742,787,307,563,52,59,0,52};
static const int ATL_tmmNT_SmnLk_XO[18] = 
   {0,0,0,0,0,0,4511,5396,2255,2485,647,787,291,508,0,75,0,52};
static const int ATL_tmmTN_SmnLk_XO[18] = 
   {0,0,0,0,7124,0,2332,6035,1098,1722,0,435,284,362,52,94,0,52};
#define ATL_tmmTT_SmnLk_XO ATL_tmmNN_SmnLk_XO
static const int *ATL_tmm_SmnLk_XO[4] =
{ATL_tmmNN_SmnLk_XO, ATL_tmmNT_SmnLk_XO,
 ATL_tmmTN_SmnLk_XO, ATL_tmmTT_SmnLk_XO};
static const int ATL_tmmNN_SmkLn_XO[18] = 
   {0,0,0,0,0,0,4673,4673,2072,2072,0,691,402,575,52,104,0,52};
static const int ATL_tmmNT_SmkLn_XO[18] = 
   {0,0,0,0,0,0,4226,4815,0,2459,719,952,420,565,52,104,0,52};
#define ATL_tmmTN_SmkLn_XO ATL_tmmNN_SmkLn_XO
#define ATL_tmmTT_SmkLn_XO ATL_tmmNT_SmkLn_XO
static const int *ATL_tmm_SmkLn_XO[4] =
{ATL_tmmNN_SmkLn_XO, ATL_tmmNT_SmkLn_XO,
 ATL_tmmTN_SmkLn_XO, ATL_tmmTT_SmkLn_XO};
static const int ATL_tmmNN_SnkLm_XO[18] = 
   {0,0,0,0,6598,0,2224,4345,952,2630,473,1071,274,497,52,104,0,52};
#define ATL_tmmNT_SnkLm_XO ATL_tmmNN_SnkLm_XO
static const int ATL_tmmTN_SnkLm_XO[18] = 
   {0,0,0,0,0,0,5820,6311,2386,2644,586,774,399,497,52,104,0,52};
#define ATL_tmmTT_SnkLm_XO ATL_tmmTN_SnkLm_XO
static const int *ATL_tmm_SnkLm_XO[4] =
{ATL_tmmNN_SnkLm_XO, ATL_tmmNT_SnkLm_XO,
 ATL_tmmTN_SnkLm_XO, ATL_tmmTT_SnkLm_XO};
static const int ATL_tmmNN_SmLnk_XO[18] = 
   {403,424,403,457,0,403,315,358,223,342,184,232,106,217,91,0,63,0};
static const int ATL_tmmNT_SmLnk_XO[18] = 
   {0,423,423,455,423,454,333,366,197,343,197,225,169,217,68,0,57,0};
static const int ATL_tmmTN_SmLnk_XO[18] = 
   {429,562,421,527,340,406,340,368,0,257,166,301,106,217,91,0,63,0};
static const int ATL_tmmTT_SmLnk_XO[18] = 
   {403,511,403,424,403,469,337,383,261,346,177,300,105,218,80,0,57,6000};
static const int *ATL_tmm_SmLnk_XO[4] =
{ATL_tmmNN_SmLnk_XO, ATL_tmmNT_SmLnk_XO,
 ATL_tmmTN_SmLnk_XO, ATL_tmmTT_SmLnk_XO};
static const int ATL_tmmNN_SnLmk_XO[18] = 
   {420,517,420,526,339,502,302,362,227,354,185,228,160,217,68,0,57,0};
static const int ATL_tmmNT_SnLmk_XO[18] = 
   {393,524,393,513,393,499,311,358,235,345,199,231,105,218,80,0,57,0};
static const int ATL_tmmTN_SnLmk_XO[18] = 
   {0,399,399,584,0,399,299,382,236,236,191,225,157,218,57,126,57,0};
static const int ATL_tmmTT_SnLmk_XO[18] = 
   {348,491,348,414,348,390,335,581,208,259,192,326,132,217,68,0,57,0};
static const int *ATL_tmm_SnLmk_XO[4] =
{ATL_tmmNN_SnLmk_XO, ATL_tmmNT_SnLmk_XO,
 ATL_tmmTN_SnLmk_XO, ATL_tmmTT_SnLmk_XO};
static const int ATL_tmmNN_SkLmn_XO[18] = 
   {511,606,511,587,436,494,340,499,218,253,162,205,137,137,80,0,69,0};
static const int ATL_tmmNT_SkLmn_XO[18] = 
   {0,622,622,1149,432,526,339,502,302,315,214,227,125,125,68,0,57,0};
static const int ATL_tmmTN_SkLmn_XO[18] = 
   {0,526,526,526,418,495,341,341,241,297,185,228,114,0,80,0,63,0};
#define ATL_tmmTT_SkLmn_XO ATL_tmmNN_SkLmn_XO
static const int *ATL_tmm_SkLmn_XO[4] =
{ATL_tmmNN_SkLmn_XO, ATL_tmmNT_SkLmn_XO,
 ATL_tmmTN_SkLmn_XO, ATL_tmmTT_SkLmn_XO};

#endif /* end ifndef ATL_TXOVER_H */
