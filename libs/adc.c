#include "adc.h"
#include "timers.h"

uint16_t adcResult[2];

void adc12Config(){
    
    ADC12CTL0 &= ~ADC12ENC;					// clear ENC bit

	ADC12CTL0 = ADC12SHT0_2      	|		// SHT = 16 ADC12CLKs
				ADC12ON;         			// turn on ADC12

	ADC12CTL1 = ADC12CSTARTADD_0 	|		// Stores the result in ADC12MEM0 
			ADC12SHS_0       		|   	// Use ADC12SC bit to trigger sampling       
			ADC12SHP         		|		// Use ADC12 intern timer to sample
			ADC12SSEL_0   		   	| 		// Use MODCLK - 5MHz as the sampling clock  
			ADC12CONSEQ_1;      			// Single channel, single conversion

	ADC12CTL2 = ADC12TCOFF       	|		// Turn off the temperature sensor
				ADC12RES_2       	|   		// 12-bit resolution
				ADC12SR;           			// FS 50KSPS (removes to reach to 200KSPS)

	ADC12MCTL0 = ADC12SREF_0      	|  		// Use AVSS and AVCC as reference
			     ADC12INCH_0;         		// Input in P6.0
	ADC12MCTL1 = ADC12SREF_0      	|  		// Use AVSS and AVCC as reference
				 ADC12INCH_1        |  		// Input in P6.1
				 ADC12EOS;

	ADC12IE = ADC12IE0;                		// Enable ADC12IFG.0    

	ADC12CTL0 |= ADC12ENC;            		// Enable conversions   
}   

#pragma vector = TIMER0_A0_VECTOR
__interrupt void ADC_TRIGGER(void){
  
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;

}

#pragma vector = ADC12_VECTOR
__interrupt void ADC_RESULT(void){
    adcResult[0] = ADC12MEM0;
    adcResult[1] = ADC12MEM1;
}