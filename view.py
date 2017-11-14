import pandas as pd
from pylab import *

data = pd.read_csv("hoge.csv")
atime = data["acc_time"]
asm = data["asm"]
emu = data["emu"]

p1 = plot(atime,asm,label="assembler")
p2 = plot(atime,emu,label="C emulation")
#gca().legend()
legend()

show()

