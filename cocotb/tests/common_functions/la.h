/**
 \file
*/
#ifndef LA_C_HEADER_FILE
#define LA_C_HEADER_FILE
/*! \enum la_reg_number
 * Logic analyzers registers
 */
enum la_reg_number {
                LA_REG_0=0/*!< First LA register probs [31:0]*/,
                LA_REG_1=1/*!< Second LA register probs [63:32]*/,
                LA_REG_2=2/*!< Third LA register probs [95:64]*/,
                LA_REG_3=3/*!< First LA register probs [127:96]*/
                };

/**
 * Setting logic analyzer input enable 
 * 
 * Enable as input to the user project. Software sends to user prject
 *   
 * @param reg_num logic analyzer register to write to. 
 * Usually not all caravel versions has the same numbers of LA registers 
 * They might have 4 registers (128 probs between software and user project)
 * or  registers (64 probs between software and user project)
 * 
 * @param is_enable 32 bits each bit indicate if the corresponding prob enabled as input 
 *  
 */
void set_la_ien(enum la_reg_number reg_num , unsigned int is_enable){
    switch(reg_num){
        #if LA_SIZE >= 64
        case 0 : reg_la0_iena = is_enable; break;
        case 1 : reg_la1_iena = is_enable; break;
        #endif 
        #if LA_SIZE >= 128
        case 2 : reg_la2_iena = is_enable; break;
        case 3 : reg_la3_iena = is_enable; break;
        #endif
        default: break;
    }
}
/**
 * Setting logic analyzer output enable 
 * 
 * Enable as output from the user project. Software receieves from user prject
 * 
 *  
 * @param reg_num logic analyzer register to write to. 
 * Usually not all caravel versions has the same numbers of LA registers 
 * They might have 4 registers (128 probs between software and user project)
 * or  registers (64 probs between software and user project)
 * 
 * @param is_enable 32 bits each bit indicate if the corresponding prob enabled as output 
 *  
 */
void set_la_oen(enum la_reg_number reg_num , unsigned int is_enable){
    switch(reg_num){
        #if LA_SIZE >= 64
        case 0 : reg_la0_oenb = ~is_enable; break;
        case 1 : reg_la1_oenb = ~is_enable; break;
        #endif 
        #if LA_SIZE >= 128
        case 2 : reg_la2_oenb = ~is_enable; break;
        case 3 : reg_la3_oenb = ~is_enable; break;
        #endif
        default: break;
    }
}
/**
 * Write data through logic analyers from software to user project
 * 
 * \note 
 * For this to work correctly prob should be configured as output 
 * 
 * @param reg_num logic analyzer register to write to. 
 * Usually not all caravel versions has the same numbers of LA registers 
 * They might have 4 registers (128 probs between software and user project)
 * or  registers (64 probs between software and user project)
 * 
 * @param data data to write through logic analyzers 
 *  
 */
void set_la_reg(enum la_reg_number reg_num , unsigned int data){
    switch(reg_num){
        #if LA_SIZE >= 64
        case 0 : reg_la0_data = data; break;
        case 1 : reg_la1_data = data; break;
        #endif 
        #if LA_SIZE >= 128
        case 2 : reg_la2_data = data; break;
        case 3 : reg_la3_data = data; break;
        #endif
        default: break;
    }
}
/**
 * Read data through logic analyers from user projectt to software
 * 
 * \note 
 * For this to work correctly prob should be configured as output 
 * 
 * @param reg_num logic analyzer register to read from. 
 * Usually not all caravel versions has the same numbers of LA registers 
 * They might have 4 registers (128 probs between software and user project)
 * or  registers (64 probs between software and user project)
 * 
 *  
 */
unsigned int get_la_reg(enum la_reg_number reg_num){
    switch(reg_num){
        #if LA_SIZE >= 64
        case 0 : return reg_la0_data_in;
        case 1 : return reg_la1_data_in;
        #endif 
        #if LA_SIZE >= 128
        case 2 : return reg_la2_data_in;
        case 3 : return reg_la3_data_in;
        #endif
        default: break;
    }
}

#endif // LA_C_HEADER_FILE
