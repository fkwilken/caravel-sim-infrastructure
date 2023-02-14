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

void main()
{

    enable_debug();
    configure_gpio(6,GPIO_MODE_MGMT_STD_OUTPUT);
    configure_gpio(5,GPIO_MODE_MGMT_STD_INPUT_NOPULL);
    configure_gpio(0,GPIO_MODE_MGMT_STD_INPUT_NOPULL);
    gpio_config_load(); 
    #ifndef ARM 

    (*(volatile uint32_t*) CSR_DEBUG_MODE_OUT_ADDR ) = 1; // enable debug mode
    #endif

    // start of the test
    set_debug_reg1(0xAA);

    // very long wait
    dummy_delay(1500);


}
