#!/usr/bin/python3
# -*- coding: utf-8 -*-
from scripts.verify_cocotb.RunFlow import RunFLow, CocotbArgs

import argparse

parser = argparse.ArgumentParser(description="Run cocotb tests")
parser.add_argument(
    "-test",
    "-t",
    nargs="+",
    help="name of test or tests.if no --sim provided RTL will be run <takes list as input>",
)
parser.add_argument(
    "-sim",
    nargs="+",
    help="Simulation typerun RTL,GL & GL_SDF provided only when run -test<takes list as input>",
)
parser.add_argument("-testlist", "-tl", nargs="+",
        help="path of testlist to be run"
)
parser.add_argument(
    "-tag",
    help="provide tag of the run default would be regression name and if no regression is provided would be run_<random float>_<timestamp>_"
)
parser.add_argument(
    "-maxerr",
    help="max number of errors for every test before simulation breaks default = 3",
)
parser.add_argument(
    "-vcs",
    "-v",
    action="store_true",
    help="use vcs as compiler if not used iverilog would be used",
)
parser.add_argument(
    "-corner",
    "-c",
    nargs="+",
    help="Corner type in case of GL_SDF run has to be provided",
)
parser.add_argument(
    "-zip_passed",
    action="store_true",
    help="zip the waves and logs of passed tests. by default if the run has more than 7 tests pass tests results would be zipped automatically",
)
parser.add_argument(
    "-emailto", "-mail", nargs="+", help="mails to send results to when results finish"
)
parser.add_argument("-seed", help="run with specific seed")
parser.add_argument("-no_wave", action="store_true", help="disable dumping waves")
parser.add_argument(
    "-sdf_setup",
    action="store_true",
    help="targeting setup violations by taking the sdf mamximum values",
)
parser.add_argument(
    "-clk", help="define the clock period in ns default defined at design_info.yaml"
)
parser.add_argument(
    "-lint", action="store_true", help="generate lint log vcs must be used"
)
parser.add_argument(
    "-macros", nargs="+", help="Add addtional verilog macros for the design "
)
parser.add_argument(
    "-sim_path",
    help='directory where simulation result directory "sim" would be created if None it would be created under cocotb folder',
)
parser.add_argument(
    "-verbosity",
    help='verbosity of the console output it can have one of 3 value debug, normal or quiet the default value is normal',
)
args = parser.parse_args()
# Arguments = namedtuple("Arguments","regression test sim corner testlist tag maxerr vcs cov checker_en  zip_passed caravan emailto seed no_wave clk lint arm sdf_setup")
# arg = Arguments(args.regression ,args.test ,args.sim ,args.corner ,args.testlist ,args.tag ,args.maxerr ,args.vcs ,args.cov ,args.checkers_en  ,args.zip_passed ,args.caravan ,args.emailto ,args.seed ,args.no_wave ,args.clk ,args.lint ,args.arm ,args.sdf_setup)
# print(args)
# print(
#     f"test:{args.test}, testlist:{args.testlist} sim: {args.sim}"
# )
cocotb_args = CocotbArgs()
cocotb_args.argparse_to_CocotbArgs(args)
RunFLow(cocotb_args)

"""
verilator_command = (f"verilator {macros} --vpi --public-flat-rw --prefix Vtop"
                            f" -LDFLAGS \"-Wl,-rpath,$(cocotb-config --prefix)/cocotb/libs"
                            f"-L$(cocotb-config --prefix)/cocotb/libs -lcocotbvpi_verilator -lgpi -lcocotb -lgpilog -lcocotbutils \" $(cocotb-config --share)/lib/verilator/verilator.cpp "
                            f"-y {VERILOG_PATH}/includes/includes.rtl.caravel  --cc -o sim_build/sim.vvp caravel_top.sv")




"""
