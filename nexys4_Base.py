#!/usr/bin/env python3

#
# This file is part of LiteX-Boards.
#
# Copyright (c) 2018-2019 Florent Kermarrec <florent@enjoy-digital.fr>
# SPDX-License-Identifier: BSD-2-Clause

import os

from migen import *
from migen.genlib.io import CRG

#from litex_boards.platforms import nexys4ddr
import nexys4_tarjeta as nexys4

from litex.soc.cores.clock import *
from litex.soc.integration.soc_core import *
from litex.soc.integration.soc_sdram import *
from litex.soc.integration.builder import *
from litex.soc.cores.led import LedChaser

from litedram.modules import MT47H64M16
from litedram.phy import s7ddrphy

from liteeth.phy.rmii import LiteEthPHYRMII

from litex.soc.cores import uart

# BaseSoC ------------------------------------------------------------------------------------------

class BaseSoC(SoCCore):
    def __init__(self, sys_clk_freq=int(100e6) ):
        platform = nexys4.Platform()

        # SoCCore ----------------------------------_-----------------------------------------------
        SoCCore.__init__(self, platform, sys_clk_freq,
            cpu_type       = "vexriscv", # or picorv32
            ident          = "LiteX SoC on Nexys4",
            ident_version  = True,
            integrated_rom_size      = 0x8000,
            integrated_main_ram_size = 0x4000,
            )

        # CRG --------------------------------------------------------------------------------------
        self.submodules.crg = CRG(platform.request("clk100"), ~platform.request("cpu_reset"))

        # Leds -------------------------------------------------------------------------------------
        self.submodules.leds = LedChaser(
            pads         = platform.request_all("user_led"),
            sys_clk_freq = sys_clk_freq)
        self.add_csr("leds")
        
        # Uart Adicional
        #  Classical UART. En /litex/soc/integration/soc.py

        self.submodules.uart1_phy = uart.UARTPHY(
            pads     = platform.request("uart1"),
            clk_freq = self.sys_clk_freq,
            baudrate = 9600) # crea un objeto uart RS232PHY. Este despues se pasa a UART
        self.submodules.uart1 = ResetInserter()(uart.UART(self.uart1_phy, #paso a UART. ResetInserter no idea
            tx_fifo_depth = 16,
            rx_fifo_depth = 16) )
            
        self.csr.add("uart1_phy", use_loc_if_exists=True)
        self.csr.add("uart1", use_loc_if_exists=True)
        
        if hasattr(self.cpu, "interrupt"):
            self.irq.add("uart1", use_loc_if_exists=True)
        else:
            self.add_constant("UART_POLLING")
        
        
        

# Build --------------------------------------------------------------------------------------------

def main():
    soc = BaseSoC(
        sys_clk_freq  = int(100e6)
    )
    
    builder = Builder(soc, output_dir="build", compile_gateware = True, csr_csv="mapaMemoria_csr.csv")
    builder.build(build_name="top")

    

if __name__ == "__main__":
    main()
