
/**
 \file
*/
#ifndef IRQ_API_C_HEADER_FILE
#define IRQ_API_C_HEADER_FILE
// IRQ 
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifndef ARM
extern unsigned int flag;
#else 
unsigned int flag;
void HK_IRQ0_Handler(void){flag = 1;}
void HK_IRQ1_Handler(void){flag = 1;}
void HK_IRQ2_Handler(void){flag = 1;}
void TMR0_Handler(void){flag = 1;clear_TMR0_Handler();}
void UART0_Handler(void){flag = 1;clear_UART0_Handler();}
void clear_TMR0_Handler(){
reg_timer0_irq =1;
}
void clear_UART0_Handler(){
reg_uart_isc =0x3;
}
#endif
char get_flag(){
    #ifndef ARM
    return flag;
    #else 
    dummy_delay(1);
    return flag;
    #endif
}

void clear_flag(){
    #ifndef ARM
    flag=0;
    #else 
    flag=0;
    
    #endif
}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
/**
 * Enable or disable external1 interrupt GPIO[7] 
 * 
 *  
 * @param is_enable when 1 (true) interrupt is active and software would detect if happened, 0 (false) interrupt is disabled and software would not detect if happened
 */
void enable_external1_irq(bool is_enable){
    if (is_enable){
        #ifndef ARM
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << USER_IRQ_4_INTERRUPT));
        reg_user4_irq_en =1;
        #else
        NVIC_EnableIRQ(HK_IRQ1);
        __enable_irq();
        #endif
    }else{
        #ifndef ARM
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << USER_IRQ_4_INTERRUPT));
        reg_user4_irq_en =0;
        #else
        NVIC_DisableIRQ(HK_IRQ1);
        __enable_irq();
        #endif
    }
}
/**
 * Enable or disable external2 interrupt GPIO[12] 
 * 
 *  
 * @param is_enable when 1 (true) interrupt is active and software would detect if happened, 0 (false) interrupt is disabled and software would not detect if happened
 */
void enable_external2_irq(bool is_enable){
    if (is_enable){
        #ifndef ARM
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << USER_IRQ_5_INTERRUPT));
        reg_user5_irq_en =1;
        #else
        NVIC_EnableIRQ(HK_IRQ2);
        __enable_irq();
        #endif
    }else{
        #ifndef ARM
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << USER_IRQ_5_INTERRUPT));
        reg_user5_irq_en =0;
        #else
        NVIC_DisableIRQ(HK_IRQ2);
        __enable_irq();
        #endif
    }    
}
/**
 * Enable or disable timer0 interrupt 
 * 
 *  
 * @param is_enable when 1 (true) interrupt is active and software would detect if happened, 0 (false) interrupt is disabled and software would not detect if happened
 */
void enable_timer0_irq(bool is_enable){
    if (is_enable){
        #ifndef ARM
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << TIMER0_INTERRUPT));
        reg_timer0_irq_en = 1;
        #else
        NVIC_EnableIRQ(TMR0_IRQn);
        reg_timer0_config = reg_timer0_config | 0x8; // enable irq
        __enable_irq();
        #endif
    }else{
        #ifndef ARM
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << TIMER0_INTERRUPT));
        reg_timer0_irq_en = 0;
        #else
        NVIC_DisableIRQ(TMR0_IRQn);
        reg_timer0_config = reg_timer0_config | 0x8; // enable irq
        __enable_irq();
        #endif
    }
}
/**
 * Enable or disable UART tx interrupt 
 * 
 *  
 * @param is_enable when 1 (true) interrupt is active and software would detect if happened, 0 (false) interrupt is disabled and software would not detect if happened
 */
void enable_uart_tx_irq(bool is_enable){
    if (is_enable){
        #ifndef ARM
        reg_uart_enable = 1;
        reg_uart_irq_en =1;
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << UART_INTERRUPT));
        #else
        NVIC_EnableIRQ(UART0_IRQn);
        reg_uart_ctrl = reg_uart_ctrl | 0x5; // enable irq TX 
        __enable_irq();
        #endif
    }else{
        #ifndef ARM
        reg_uart_irq_en =0;
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << UART_INTERRUPT));
        #else
        NVIC_DisableIRQ(UART0_IRQn);
        reg_uart_ctrl = reg_uart_ctrl | 0x5; // enable irq TX 
        __enable_irq();
        #endif
    }
}
/**
 * Enable or disable UART rx interrupt 
 * 
 *  
 * @param is_enable when 1 (true) interrupt is active and software would detect if happened, 0 (false) interrupt is disabled and software would not detect if happened
 */
void enable_uart_rx_irq(bool is_enable){
    if (is_enable){
        #ifndef ARM
        reg_uart_enable = 1;
        reg_uart_irq_en =1;
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << UART_INTERRUPT));
        #else
        NVIC_EnableIRQ(UART0_IRQn);
        reg_uart_ctrl = reg_uart_ctrl | 0xA; // enable irq rx 
        __enable_irq();
        #endif
    }else{
        #ifndef ARM
        reg_uart_enable = 1;
        reg_uart_irq_en =1;
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << UART_INTERRUPT));
        #else
        reg_uart_ctrl = reg_uart_ctrl & 0xF7; // enable irq rx 
        __enable_irq();
        #endif
    }
}
/**
 * Enable or disable SPI interrupt 
 * 
 *  
 * @param is_enable when 1 (true) interrupt is active and software would detect if happened, 0 (false) interrupt is disabled and software would not detect if happened
 */
void enable_hk_spi_irq(bool is_enable){
    if (is_enable){
        #ifndef ARM
        reg_user3_irq_en =1;
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << USER_IRQ_3_INTERRUPT));
        #else
        NVIC_EnableIRQ(HK_IRQ0);
        __enable_irq();
        #endif
    }else{
        #ifndef ARM
        irq_setmask(0);
        irq_setie(1);
        irq_setmask(irq_getmask() | (1 << USER_IRQ_3_INTERRUPT));
        reg_user3_irq_en =0;
        #else
        NVIC_DisableIRQ(HK_IRQ0);
        __enable_irq();
        #endif
    }
}

#endif // IRQ_API_C_HEADER_FILE
