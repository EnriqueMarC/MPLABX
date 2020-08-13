void ADC_config(void){
//Paso 1, configuración de ADCON1
    ADCON1bits.VCFG = 0; //Referencias de voltaje
    ADCON1bits.PCFG = 0b1010; //Los pines como I analógica e I/O Digitales
    //Paso 2. Configuración de tiempos de adquisición
    ADCON2bits.ACQT = 0b001;//2TAD de 1.3uS ACQT de 2.66uS
    ADCON2bits.ADCS = 0b110;//La frecuencia de entrada es dividida entre 64 1TAD de 1.3uS
    ADCON2bits.ADFM = 1;
    // Paso 3. Encender el módulo ADC
    ADCON0bits.ADON = 1;
    //Fin de la configuración
     //RA0 como entrada
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 1;
    TRISAbits.RA3 = 1;
    TRISAbits.RA5 = 1;
}