#include "inttab.h"


int main (int argc, char *argv[]) {
  // Initialize all configurables to defaults
  uint lowr_bnd  = DEFAULT_LOWER_BOUND,
       highr_bnd = DEFAULT_HIGHER_BOUND,
       presc     = DEFAULT_PRESCISION,
       col_width = DEFAULT_COLUMN_WIDTH;
  char included  = DEFAULT_INCLUDED;
  
  // Initialize interest rate storage
  uint rate_count = 0;
  double *int_rate = calloc(1, sizeof(double));
  double *tmp_rate;

  char incl_flag = 0; // Flag for the selection of table values
  
  // Check for arguments provided
  if (argc < 2) {
    printf("No arguments provided. Run \'%s -h\' for details.\n", argv[0]);
    return 0;
  }
  else
    for (int i = 1; i < argc; ++i) { // Iterate through non-name arguments
      if (!strncmp (argv[i], "[", 1)) { // Bounds Specification 
        char *subs, *tmp;
        uint len = strlen(argv[i]); // Save argument length before tokens
        subs = strtok (argv[i], ",");
        
        // The length of token is the same for the single value argument
        if (strlen(subs) == len) {
          // Extract the upper bound from argument
          tmp = (char *) malloc(strlen(argv[i] - 1));
          strncpy(tmp, argv[i] + 1, strlen(argv[i]) -2);
          highr_bnd = (uint) strtoul(tmp, NULL, 10);
          free(tmp);
        }
        else {
          // Extract both bounds from argument
          tmp = (char *) malloc(strlen(subs));
          strncpy (tmp, subs+1, strlen(subs)-1);
          lowr_bnd = (uint) strtoul(tmp, NULL, 10);
          free(tmp);
          
          subs=strtok(NULL, ",");
          tmp = (char *) malloc(strlen(subs));
          strncpy (tmp, subs, strlen(subs)-1);
          highr_bnd = (uint) strtoul(tmp, NULL, 10);
          free(tmp);
        }
        incl_flag = 0; // Reset inclusion selection flag
      }
      else if (!strncmp(argv[i]+strlen(argv[i])-1 ,"%",1)) { // Interest Rate
        // Duplicate previous list with new value appended
        tmp_rate = calloc(rate_count + 1, sizeof(double));
        memmove (tmp_rate, int_rate, rate_count*sizeof(double));
        tmp_rate[rate_count] = (double) strtod(argv[i], NULL);
        
        if (tmp_rate[rate_count] <= 0) { // Ensure interest rate is valid
          printf("Please enter a valid interest rate. Run \'%s -h\' for details.\n", argv[0]);
          return 0;
        }
        
        free(int_rate); // Free and replace previous list
        int_rate = tmp_rate;
        ++rate_count; // Increment rate counter
        incl_flag = 0; // Reset inclusion selection flag
      }
      else if (!strncmp(argv[i], "-", 1)) { // Other configuration
        switch ((char)*(argv[i]+1)) { // Use first character after (-) to determine behaviour
          case 'h': // Help
            printf("Interest Table Generation\n");
            printf("© Carson Hall 2020\n\n");
            printf("Basic Usage: %s ([(First_Value,)Last_Value]) Interest_Rate( %% ) (OPTIONS)\n\n", argv[0]);
            printf("%-10s\n", "Arguments");
            printf("%5s%s\n", "", "Interest_Rate: Interest rate for a period. This is required to run this program. It must end in %%.");
            printf("%5s%s\n", "", "[(First_Value,)Last_Value]: Bounds for the rates to appear in the table (both inclusive). Neither are required, but if only one is present it will be interpereted as [1,Last_Value].");
            printf("%-10s\n", "Options");
            printf("%5s%s\n", "", "-e (OPTIONS)    -> Exclude: Exclude a value from the table.");
            printf("%5s%s\n", "", "-E (OPTIONS)    -> Strict Exclude: From all available options, only exclude those specified.");
            printf("%5s%s\n", "", "-i (OPTIONS)    -> Include: Include a value in the table.");
            printf("%5s%s\n", "", "-I (OPTIONS)    -> Strict Include: Only include values specified.");
            printf("%5s%s\n", "", "-w <width>      -> Width: Set the width of columns in the table.");
            printf("%5s%s\n", "", "-p <prescision> -> Prescision: Set the prescision of the table values.");
            printf("%5s%s\n\n", "", "-h              -> Get help.");
            printf("%-10s\n", "Table Values");
            printf("%5s%s\n", "", "fp -> Compound Amount Factor");
            printf("%5s%s\n", "", "pf -> Present Worth Factor");
            printf("%5s%s\n", "", "fa -> Compound Amount Factor");
            printf("%5s%s\n", "", "af -> Sinking Fund Factor");
            printf("%5s%s\n", "", "pa -> Present Worth Factor");
            printf("%5s%s\n", "", "ap -> Capital Recovery Factor");
            printf("%5s%s\n", "", "pg -> Gradient Present Worth");
            printf("%5s%s\n", "", "ag -> Gradient Uniform Series");
            return 0;
          case 'w': // Column Width
            ++i; // Take next argument (corresponding value)
            col_width = strtoul(argv[i], NULL, 10);
            incl_flag = 0; // Reset inclusion selection flag
            break;
          case 'p': // Prescision
            ++i; // Take next argument (corresponding value)
            presc = strtoul(argv[i], NULL, 10);
            incl_flag = 0; // Reset inclusion selection flag
            break;
          case 'e': // Exclude
            incl_flag = -1; // Set inclusion flag to exclude
            break;
          case 'E': // Strict exclude
            included = 0xFF; // Reset
            incl_flag = -1; // Set inclusion flag to exclude
            break;
          case 'i': // Include
            incl_flag = 1; // Set inclusion flag to include
            break;
          case 'I': // Strict include
            included = 0x00; // Reset
            incl_flag = 1; // Set inclusion flag to include
            break;
        }
      }
      else {
        if (incl_flag < 0 && strlen(argv[i]) == 2) { // Exclusions from table
          char mask = 0x00;
          if      (!strcmp(argv[i], "fp") || !strcmp(argv[i], "FP"))
            mask += INCLUDE_FP;
          else if (!strcmp(argv[i], "pf") || !strcmp(argv[i], "PF"))
            mask += INCLUDE_PF;
          else if (!strcmp(argv[i], "fa") || !strcmp(argv[i], "FA"))
            mask += INCLUDE_FA;
          else if (!strcmp(argv[i], "af") || !strcmp(argv[i], "AF"))
            mask += INCLUDE_AF;
          else if (!strcmp(argv[i], "pa") || !strcmp(argv[i], "PA"))
            mask += INCLUDE_PA;
          else if (!strcmp(argv[i], "ap") || !strcmp(argv[i], "AP"))
            mask += INCLUDE_AP;
          else if (!strcmp(argv[i], "pg") || !strcmp(argv[i], "PG"))
            mask += INCLUDE_PG;
          else if (!strcmp(argv[i], "ag") || !strcmp(argv[i], "AG"))
            mask += INCLUDE_AG;
          else
            goto operator_except; // Exception if not a valid value
          included &= ~mask;
          continue; // Skip to next argument
        }
        if (incl_flag > 0 && strlen(argv[i]) == 2) { // Inclusions to table
          char mask = 0x00;
          if      (!strcmp(argv[i], "fp") || !strcmp(argv[i], "FP"))
            mask += INCLUDE_FP;
          else if (!strcmp(argv[i], "pf") || !strcmp(argv[i], "PF"))
            mask += INCLUDE_PF;
          else if (!strcmp(argv[i], "fa") || !strcmp(argv[i], "FA"))
            mask += INCLUDE_FA;
          else if (!strcmp(argv[i], "af") || !strcmp(argv[i], "AF"))
            mask += INCLUDE_AF;
          else if (!strcmp(argv[i], "pa") || !strcmp(argv[i], "PA"))
            mask += INCLUDE_PA;
          else if (!strcmp(argv[i], "ap") || !strcmp(argv[i], "AP"))
            mask += INCLUDE_AP;
          else if (!strcmp(argv[i], "pg") || !strcmp(argv[i], "PG"))
            mask += INCLUDE_PG;
          else if (!strcmp(argv[i], "ag") || !strcmp(argv[i], "AG"))
            mask += INCLUDE_AG;
          else
            goto operator_except; // Exception if not a valid value
          included |= mask;
          continue; // Skip to next argument
        }
        operator_except:
        printf("Invalid operator. Run \'%s -h\' for details\n", argv[0]);
        return 0;
      }
    }

  // Write interest(ing) table
  for (uint i = 0; i < rate_count; ++i) {
    printf("Table for the interest rate of: %.*lf\n", presc, int_rate[i]);
    double ref_int = int_rate[i]/100.0; // Convert from percentage

    // Write appropriate table headers
    printf("%6s ", "Period");
    if (INCLUDE_FP & included)
      printf("%*s ", col_width, "(F|P, i, N)");
    if (INCLUDE_PF & included)
      printf("%*s ", col_width, "(P|F, i, N)");
    if (INCLUDE_FA & included)
      printf("%*s ", col_width, "(F|A, i, N)");
    if (INCLUDE_AF & included)
      printf("%*s ", col_width, "(A|F, i, N)");
    if (INCLUDE_PA & included)
      printf("%*s ", col_width, "(P|A, i, N)");
    if (INCLUDE_AP & included)
      printf("%*s ", col_width, "(A|P, i, N)");
    if (INCLUDE_PG & included)
      printf("%*s ", col_width, "(P|G, i, N)");
    if (INCLUDE_AG & included)
      printf("%*s ", col_width, "(A|G, i, N)");
    printf("\n");
    
    // Write appropriate values between bounds
    for (uint N = lowr_bnd; N <= highr_bnd; ++N) {
      printf("%6d ", N);
      if (INCLUDE_FP & included)
        printf("%*.*lf ", col_width, presc, f_p(ref_int, N));
      if (INCLUDE_PF & included)
        printf("%*.*lf ", col_width, presc, p_f(ref_int, N));
      if (INCLUDE_FA & included)
        printf("%*.*lf ", col_width, presc, f_a(ref_int, N));
      if (INCLUDE_AF & included)
        printf("%*.*lf ", col_width, presc, a_f(ref_int, N));
      if (INCLUDE_PA & included)
        printf("%*.*lf ", col_width, presc, p_a(ref_int, N));
      if (INCLUDE_AP & included)
        printf("%*.*lf ", col_width, presc, a_p(ref_int, N));
      if (INCLUDE_PG & included)
        printf("%*.*lf ", col_width, presc, p_g(ref_int, N));
      if (INCLUDE_AG & included)
        printf("%*.*lf ", col_width, presc, a_g(ref_int, N));
      printf("\n");
    }
  }
  return 0;
}
