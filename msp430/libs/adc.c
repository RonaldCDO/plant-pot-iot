#include "adc.h"
#include "timers.h"
#include "gpio.h"

void adc12Config(){
    
    ADC12CTL0 &= ~ADC12ENC;					// clear ENC bit

	ADC12CTL0 = ADC12ON;         			// turn on ADC12

	ADC12CTL1 = ADC12CSTARTADD_0 	|		// Stores the result in ADC12MEM0 
			    ADC12SHS_1       	|   	// Select TA0.1 for trigger source
			    ADC12SSEL_3   		| 		// Use SMCLK - 1MHz as the sampling clock
			    ADC12CONSEQ_3;      		// multi channel, sequence without repeat

	ADC12CTL2 =  ADC12RES_2;                // 12BITS mode

	pinMode(6,0, module);
	pinMode(6,1, module);

	ADC12MCTL0 = ADC12SREF_0      	|  		// Use AVSS and AVCC as reference
			     ADC12INCH_0;         		// Input in P6.0
	ADC12MCTL1 = ADC12SREF_0      	|  		// Use AVSS and AVCC as reference
				 ADC12INCH_1        |  		// Input in P6.1
				 ADC12EOS;

	ADC12IE |= ADC12IE1;                	// Enable ADC12IFG.1

	ADC12CTL0 |= ADC12ENC;            		// Enable conversions   
}   


#pragma vector = ADC12_VECTOR
__interrupt void ADC_RESULT(void){
    adcResult[0] = ADC12MEM0;
    adcResult[1] = ADC12MEM1;
}
