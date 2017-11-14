import pandas as pd
from pylab import *


p1 = pd.read_csv("plan1.csv")
asm = p1["asm"]
emu = p1["emu"]
rmse1 = sqrt(sum((asm-emu)**2))

p2 = pd.read_csv("plan2.csv")
asm = p2["asm"]
emu = p2["emu"]
rmse2 = sqrt(sum((asm-emu)**2))

print("rmse1: ",rmse1)
print("rmse2: ",rmse2)

if ([rmse1,rmse2].index((min([rmse1,rmse2]))))==0:
    print("plan1 is better")
else:
    print("plan2 is better")

