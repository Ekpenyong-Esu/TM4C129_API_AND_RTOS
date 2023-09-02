

/**
 * main.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <tm4c129encpdt.h>

/* calculating the Fin = Fxtal/[ (Q+1) * (N+1) ]
 *                      25MHz / [ (0+1) * (4+1) ] = 5MHz   check page 245 data sheet
 *                      calculating MDIV
 *                      MDIV = MINT + (MFRAC/1024)
 *                      MDIV = MINT
 *                      MDIV = 96
 *
 *                      Fvco = Fin * MDIV
 *                            = 5*96 = 480
 *
 *                   and system clock = SysClk = fVCO/ (PSYSDIV + 1)
 *                                             = 480/ (3+1) = 120j
 */






#define MINT  96

#define MFRAC  0

#define N      4

#define PSYDIV 3

#define Q      0


#define FBCHT   0x6
#define EBCHT   0x6
#define FBCE    0
#define EBCE    0
#define FWS     0x5
#define EWS     0x5
#define MB1     0x00100010






void Sysclkconfig()
{



   // setting OSCRNG TO 1 GIVES HIGH SPEED CRYSTAL OF FREQUENCY
   SYSCTL_MOSCCTL_R |= SYSCTL_MOSCCTL_OSCRNG;

   // WE CLEAR THE  NOXTAL TO 0 WHICH ENABLE THE EXTERNAL OSCILATOR
   SYSCTL_MOSCCTL_R &= ~ SYSCTL_MOSCCTL_NOXTAL;

   // WE THEN POWER UP THE MAIN OSCILATOR BY CLEARING THE PWRDN OF THE MOSCCTL
   SYSCTL_MOSCCTL_R &= ~SYSCTL_MOSCCTL_PWRDN;

   // WE THE WAIT FOR THE OSILLATOR TO SETTLE BY POOLING THE RAW INTERRUPT STATUS
    while (!(SYSCTL_RIS_R & SYSCTL_RIS_MOSCPUPRIS)){};

    // WE THEN CONFIGURE THE PLL CLOCK SOURCE BY RSCLKCFG
    // WE FIRST CLEAR THE PLL SRC REGISTER AND OSCSRC REGISTER (0xff)
    SYSCTL_RSCLKCFG_R &= ~(SYSCTL_RSCLKCFG_PLLSRC_M|SYSCTL_RSCLKCFG_OSCSRC_M);

    // SET MOSC AS OSCILLATOR SOURCE BY USIG 0X3
    SYSCTL_RSCLKCFG_R |= SYSCTL_RSCLKCFG_OSCSRC_MOSC;

    // WE NOW SET MOSC AS PLL INPUT BY WRITIN 0X3
    SYSCTL_RSCLKCFG_R |= SYSCTL_RSCLKCFG_PLLSRC_MOSC;


    // WE NOW SET UP PLL M INTEGER (MINT) AND M FRACTIONAL (MFRAC)
    SYSCTL_PLLFREQ0_R |= (MFRAC << 10) | MINT;
    SYSCTL_PLLFREQ1_R |= (Q << 8) | N ;

    //WE NOW PWER UP THE PLL BY USING PLLFREQ0 PLLPWR REGISTER
    SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;

    // WE NOW SET THE MEMORY TIMING PARAMETER
    SYSCTL_MEMTIM0_R = (EBCHT<<22)|(EBCE<<21)|(EWS<<16)|(FBCHT<<6)|(FBCE<<5)|(FWS<<0);
    SYSCTL_MEMTIM0_R|=(1<<20)|(1<<4);

    // WAIT FOR PLL TO LOCK BY POOLIN PLLSTAT
   while (!(SYSCTL_PLLSTAT_R &SYSCTL_PLLSTAT_LOCK)) {};

   // ENABLE MEMTIM UPDATE BY WRITING TO RSCLKCFG AND SETTING
   //NEW FREQUENCY AND SPECIFY PLL CLOCK SOURCE

   SYSCTL_RSCLKCFG_R |= SYSCTL_RSCLKCFG_MEMTIMU|SYSCTL_RSCLKCFG_NEWFREQ|SYSCTL_RSCLKCFG_USEPLL|PSYDIV;

}
int main(void){

    Sysclkconfig();
}

