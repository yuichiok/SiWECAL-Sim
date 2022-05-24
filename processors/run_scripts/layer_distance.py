#!/usr/bin/env python3

import numpy as np, sys
from confs import W_Confs

tbconf = sys.argv[1]
offset = float(sys.argv[2])

Si_z = np.array([z[2]/1000 for z in W_Confs[tbconf]])

result = [offset]
for i in range(1, len(Si_z)):
    this = offset - Si_z[i-1] + (Si_z[i-1] + Si_z[i])/2 + result[-1] + 7.54
    result.append(this)
result = np.array(result)
print("Add to processor:\n")
print("<parameter name=\"FixedPosZ\" type=\"string\">")
print(np.array2string(result, precision=3, separator=" ")[2:-1])
print("</parameter>")