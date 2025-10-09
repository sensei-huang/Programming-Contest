# 左边右边
import math

n, k = input().split()
n, k = int(n), int(k)
viralscore = [int(a)-k for a in input().split()]
s = sum(viralscore)
for i in range(n):
    if s < 0:
        if viralscore[0] < viralscore[-1]:
            s -= viralscore[0]
            viralscore.pop(0)
        elif viralscore[0] > viralscore[-1]:
            s -= viralscore[-1]
            viralscore.pop(-1)
        else:
            for j in range(1, math.floor(i/2)):
                if viralscore[j] < viralscore[-1-j]:
                    s -= viralscore[0]
                    viralscore.pop(0)
                    break
                elif viralscore[j] > viralscore[-1-j]:
                    s -= viralscore[-1]
                    viralscore.pop(-1)
                    break
    else:
        break

if len(viralscore) == 1:
    if s < 0:
        print(0)
    else:
        print(1)
else:
    print(len(viralscore))