/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../common_functions/common.c"
#include "../common_functions/gpios.c"

// --------------------------------------------------------

/*
 *	SPI master Test
 *	- Enables SPI master
 *	- Uses SPI master to talk to external SPI module
 */

void main(){
    enable_debug();
    hk_spi_disable();

    configure_gpio(34,GPIO_MODE_MGMT_STD_INPUT_NOPULL); // SDI
    configure_gpio(35,GPIO_MODE_MGMT_STD_OUTPUT);       // SDO
    configure_gpio(33,GPIO_MODE_MGMT_STD_OUTPUT);       // CSB
    configure_gpio(32,GPIO_MODE_MGMT_STD_OUTPUT);       // SCK

    // Now, apply the configuration
    gpio_load();

    set_debug_reg2(0xAA);

    spi_en();


    // For SPI operation, GPIO 1 should be an input, and GPIOs 2 to 4
    // should be outputs.

    // Start test

    // Enable SPI master
    // SPI master configuration bits:
    // bits 7-0:	Clock prescaler value (default 2)
    // bit  8:		MSB/LSB first (0 = MSB first, 1 = LSB first)
    // bit  9:		CSB sense (0 = inverted, 1 = noninverted)
    // bit 10:		SCK sense (0 = noninverted, 1 = inverted)
    // bit 11:		mode (0 = read/write opposite edges, 1 = same edges)
    // bit 12:		stream (1 = CSB ends transmission)
    // bit 13:		enable (1 = enabled)
    // bit 14:		IRQ enable (1 = enabled)
    // bit 15:		(unused)


    CS_en();  // sel=0, manual CS

    spi_write(0x03);        // Write 0x03 (read mode)
    spi_write(0x00);        // Write 0x00 (start address high byte)
    spi_write(0x00);        // Write 0x00 (start address middle byte)
    spi_write(0x04);        // Write 0x04 (start address low byte)

    unsigned int value = spi_read(); // 0x93
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value


    value = spi_read(); // 0x01
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value

    value = spi_read(); // 0x00
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value

    CS_dis();  // release CS
    CS_en();  // sel=0, manual CS

    spi_write(0x03);        // Write 0x03 (read mode)
    spi_write(0x00);        // Write 0x00 (start address high byte)
    spi_write(0x00);        // Write 0x00 (start address middle byte)
    spi_write(0x08);        // Write 0x08 (start address low byte)

    value = spi_read(); // 0x13
    if (value == 0x13)
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value

    value = spi_read(); // 0x02
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value

    CS_dis();  // release CS
    CS_en();  // sel=0, manual CS

    spi_write(0x03);        // Write 0x03 (read mode)
    spi_write(0x00);        // Write 0x00 (start address high byte)
    spi_write(0x00);        // Write 0x00 (start address middle byte)
    spi_write(0x0a);        // Write 0x0a (start address low byte)

    value = spi_read(); // 0x63
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value

    value = spi_read(); // 0x57
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value

    value = spi_read(); // 0xb5
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value

    value = spi_read(); // 0x00
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value
    
    value = spi_read();// 0x23
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value


    value = spi_read(); // 0x20
    set_debug_reg1(value);
    // set_debug_reg2(0x55); // value is ready to be read
    // wait_debug_reg2(0xCC) // testbench has read the value

    CS_dis();  // release CS
    CS_en();  // sel=0, manual CS

    dummy_delay(100000000);
}

