## CSR bit definitions
A Pixie-16 module has several control registers that are used to activate runs, download DSP code 
or reset DSP, control pullup resistor for the trigger lines on the backplane, and/or indicate 
module status to the host computer. Software on the host computer can set and/or read bits in 
these registers to control the operation of the Pixie-16 module or monitor its internal status.

| Bit name | Bit # | Direction | Description |
|---|---|---|---|
| RUNENABLE | 0 | Read/Write | Enable run:<br><ul><li>1: start a run</li><li>0: stop a run</li></ul> |
| Download DSP code | 1 | Read/Write | Enable DSP code download:<br><ul><li>1: Download DSP code</li><li>0: DSP code is running</li></ul> |
| PCI Active read or write | 2 | Read/Write | Indicate PCI I/O is active:<br><ul><li>1: PCI is reading or writing memory</li><li>0: no PCI read or write</li></ul> |
| Pull-up resistor control | 3 | Read/Write | Control pull-up for the SYNC lines:<br><ul><li>1: wired-OR trigger lines on the backplane connect to a pull-up resistor</li><li>0: not connected</li></ul> |
| Reset DSP | 4 | Read/Write | Generate a pulse to reset DSP:<br><ol><li>Call Pixie_ReadCSR to read the value of this control register</li><li>Set this bit to 1</li><li> Write the control register back to the System FPGA by calling function Pixie_WrtCSR</li></ol> |
| Reserved | 5 |  |  |
| EXTFIFO_WML | 6 | Read only | External FIFO watermark level indicator:<br><ul><li>1: number of data words in the external FIFO exceeds the watermark level, so the external FIFO is ready to be read out</li><li>0: number of data words in the external FIFO is still below the watermark level, so the external FIFO is not yet ready to be read out</li></ul> |
| Reserved | [12:7] |  |  |
| RUNACTIVE | 13 | Read only | Run active indicator:<br><ul><li>1: run is active</li><li>0: run has ended</li></ul> |
| Reserved | 14 |  |  |
| CLREXTMEM_ACTIVE   | 15 | Read only | Clearing external memory active indicator:<br><ul><li>1: clearing external memory is still ongoing</li><li>0: clearing external memory has completed</li></ul> |