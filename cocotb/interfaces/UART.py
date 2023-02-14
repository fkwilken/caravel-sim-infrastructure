import cocotb
from cocotb.triggers import FallingEdge,RisingEdge,ClockCycles,Timer,Edge
from interfaces.common import Macros

class UART():
    def __init__(self,caravelEnv) -> None:
        self.caravelEnv = caravelEnv
        clock = caravelEnv.get_clock_obj()
        self.period = clock.period/1000
        self.bit_time_ns = round( 1.01 * 10**5 * self.period / (96))  # 10% factor of safety
        cocotb.log.info (f"[UART] configure uart bit_time_ns = {self.bit_time_ns}ns")

    async def get_line(self):
        line =''
        while(True):
            new_char = await self.get_char()
            cocotb.log.info (f"[UART] new char = {new_char}")
            if new_char == "\n":
                break
            line += new_char 
            cocotb.log.info (f"[UART] new char again = {new_char}")
            cocotb.log.info (f"[UART] line recieved = {line}")
        cocotb.log.info (f"[UART] line recieved = {line}")
        return line

    async def get_char(self):
        await self.start_of_tx()
        char =''
        for i in range(8):
            char = self.caravelEnv.monitor_gpio((6,6)).binstr + char
            await Timer(self.bit_time_ns, units='ns')
        return chr(int(char,2))

    async def start_of_tx(self):
        while (True): # wait for the start of the transimission it 1 then 0
            if (self.caravelEnv.monitor_gpio((6,6)).integer == 0):
                break
            await Timer(round(self.bit_time_ns/100), units='ns')
        await Timer(self.bit_time_ns, units='ns') 

    
    async def uart_send_char(self,char):
        char_bits = [int(x) for x in '{:08b}'.format(ord(char))]
        cocotb.log.info (f"[TEST] start sending on uart {char}")
        #send start bit
        self.caravelEnv.drive_gpio_in((5,5),0)
        extra_time = 0
        if Macros['ARM']:
            extra_time= -479 * self.period  # there is state 1 which takes 11975 ns and this time isn't in ARM only
        cocotb.log.info (f"[TEST] extra_time = {extra_time}ns")

        await Timer(self.bit_time_ns + extra_time, units='ns')
        #send bits 
        for i in reversed(range(8)):
            self.caravelEnv.drive_gpio_in((5,5),char_bits[i])
            await Timer(self.bit_time_ns, units='ns')

        # stop of frame
        self.caravelEnv.drive_gpio_in((5,5),1)
        await Timer(self.bit_time_ns, units='ns')
        await Timer(self.bit_time_ns, units='ns')
        # insert 4 bit delay just for debugging
        await Timer(self.bit_time_ns, units='ns')
        await Timer(self.bit_time_ns, units='ns')
        await Timer(self.bit_time_ns, units='ns')
        await Timer(self.bit_time_ns, units='ns')

    async def uart_send_line(self,line):
        for char in line: 
            await self.uart_send_char(char)

        # end of line \n 
        await self.uart_send_char("\n")
        