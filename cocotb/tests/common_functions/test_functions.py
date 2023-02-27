
import random
import cocotb
from cocotb.clock import Clock
import cocotb.log
import interfaces.caravel as caravel
from interfaces.logic_analyzer import LA
from wb_models.housekeepingWB.housekeepingWB import HK_whiteBox
from wb_models.gpio_controlWB.GPIO_ctrlWB import GPIOs_ctrlWB
import interfaces.common as common
import logging
from interfaces.cpu import RiskV
from cocotb.log import SimTimeContextFilter
from cocotb.log import SimLogFormatter
from tests.common_functions.Timeout import Timeout
import os
from cocotb.triggers import FallingEdge,RisingEdge,ClockCycles
from cocotb_coverage.coverage import *
from interfaces.common import Macros
from importlib import import_module
 
"""configure the test log file location and log verbosity 
   configure the test clock 
   configure the test timeout 
   configure whitbox models
   start up the test connecting power vdd to the design then reset and disable the CSB bit 
   return the caravel environmnet with clock and start up
"""
import yaml
def read_config_file():
    cocotb.plusargs["TAG"]
    config_file = f"{cocotb.plusargs['MAIN_PATH']}/sim/{cocotb.plusargs['TAG']}/configs.yaml".replace('"', '')

    with open(config_file) as file:
        # The FullLoader parameter handles the conversion from YAML
        # scalar values to Python the dictionary format
        configs = yaml.load(file, Loader=yaml.FullLoader)

        print(configs)
        return configs


active_gpios_num = 37 # number of active gpios
async def test_configure(dut:cocotb.handle.SimHandle,timeout_cycles=1000000,clk=25,timeout_precision=0.2,num_error=3)-> caravel.Caravel_env:
    """
    Configure caravel power, clock, and reset and setup the timeout watchdog then return object of caravel environment.

    :param SimHandle dut: dut handle
    :param int timeout_cycles: Number of cycles before reporting timeout and exit the test default = 1000000 cycles 
    :param int clk: The clock period to be used in the design in ``'ns'`` default 12.5 ``'ns'``
    :param int timeout_precision: Precision of logging how many cycle left until the timeout default is 0.2 meaning if time is 100 cycle every 20 cycles there would be a warning message for timeout 
    :param int num_error: Maximum number of errors reported before terminate the test  
    :return: Object of type Caravel_env (caravel environment)
    """
    caravelEnv = caravel.Caravel_env(dut)
    Timeout(caravelEnv.clk,timeout_cycles,timeout_precision)
    cocotb.scheduler.add(max_num_error(num_error,caravelEnv.clk))
    caravelEnv.setup_clock(clk)
    await caravelEnv.start_up()
    await ClockCycles(caravelEnv.clk, 10)
    coverage = Macros['COVERAGE']
    checker = Macros['CHECKERS']
    if checker:
        HK_whiteBox(dut,checkers=True)
        GPIOs_ctrlWB(dut,checkers=True)
    elif coverage: 
        HK_whiteBox(dut)
        GPIOs_ctrlWB(dut)
    if  Macros['ARM']:
        global active_gpios_num
        active_gpios_num = 34 # with ARM the last 3 gpios are not configurable
    return caravelEnv
    
class CallCounted:
    """Decorator to determine number of calls for a method"""

    def __init__(self,method):
        self.method=method
        self.counter=0

    def __call__(self,*args,**kwargs):
        self.counter+=1
        return self.method(*args,**kwargs)


def repot_test(func):
    async def wrapper_func(*args, **kwargs):
        ## configure logging 
        COCOTB_ANSI_OUTPUT=0
        TESTFULLNAME = cocotb.plusargs['FTESTNAME']
        sim_dir = f"{cocotb.plusargs['SIM_PATH']}/{cocotb.plusargs['TAG']}"
        TestName = func.__name__
        logger_file= f"{sim_dir}/{TESTFULLNAME}/{TestName}.log".replace('"', '')
        cocotb.log.setLevel(logging.INFO)
        cocotb.log.error = CallCounted(cocotb.log.error)
        cocotb.log.critical = CallCounted(cocotb.log.critical)
        cocotb.log.warning = CallCounted(cocotb.log.warning)
        handler = logging.FileHandler(logger_file,mode='w')
        handler.addFilter(SimTimeContextFilter())
        handler.setFormatter(SimLogFormatter())
        cocotb.log.addHandler(handler) 
        ## call test 
        await func(*args, **kwargs)
        if Macros['COVERAGE'] or Macros['CHECKERS']:
            coverage_db.export_to_yaml(filename=f"{sim_dir}/{TESTFULLNAME}/coverage.ylm")
        ## report after finish simulation
        msg = f'with ({cocotb.log.critical.counter})criticals ({cocotb.log.error.counter})errors ({cocotb.log.warning.counter})warnings '
        if cocotb.log.error.counter > 0 or cocotb.log.critical.counter >0:
            raise cocotb.result.TestComplete(f'Test failed {msg}')
        else: 
            cocotb.log.info(f'Test passed {msg}')
    return wrapper_func

async def max_num_error(num_error,clk):
    while True:
        await ClockCycles(clk,1)
        if cocotb.log.error.counter + cocotb.log.critical.counter > num_error:
            msg = f'Test failed with max number of errors {num_error} ({cocotb.log.critical.counter})criticals ({cocotb.log.error.counter})errors ({cocotb.log.warning.counter})warnings '
            raise cocotb.result.TestFailure(msg)
        

async def wait_reg1(cpu,caravelEnv,data):
    while (True):
        if cpu.read_debug_reg1() == data: 
            return
        await ClockCycles(caravelEnv.clk,1)
        
            
async def wait_reg2(cpu,caravelEnv,data):
    while (True):
        if cpu.read_debug_reg2() == data: 
            return
        await ClockCycles(caravelEnv.clk,1)
        