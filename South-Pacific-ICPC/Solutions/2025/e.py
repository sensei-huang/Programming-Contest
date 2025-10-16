import math
from unittest.mock import NonCallableMagicMock

set = [int(a) for a in input().split()]
num = set.pop(0) # Number of primes

def ssRec(oset, sset:list, aset, n):
    if n == 0:
        if sset == None and aset == None:
            return math.prod(oset)+1
        a = 1 if (math.prod(sset) == 0) else math.prod(sset)
        b = 1 if (math.prod(aset) == 0) else math.prod(aset)
        return a+b
    else:
        if sset == None and aset == None:
            sset = oset
            aset = []
        sets = []
        for i in range(len(sset)):
            # Append one to aset
            a = sset.pop(i)
            aset.append(a)
            if n == 1:
                sets.append(ssRec(oset, sset, aset, n-1))
            else:
                sets.extend(ssRec(oset, sset, aset, n-1))
            sset.insert(i, aset.pop())
        return sets

def checkp(n):
    pr = True
    for j in range(2, math.floor(math.sqrt(n))+1):
        if j != n and i%j == 0:
            pr = False
            break
    return pr

primelist = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127]
primes = []

# for i in range(2, 128):
#     a = True
#     for j in range(2, math.floor(math.sqrt(i))+1):
#         if j != i and i%j == 0:
#             a = False
#             break
#     if a:
#         print(i, end=", ")

for i in range(num+1):
    arr = ssRec(set, None, None, i)
    if type(arr) != list:
        arr = [arr]
    for j in arr:
        if j not in primelist:
            for p in primelist:
                if j%p == 0:
                    while j%p == 0:
                        j //= p
                    if p not in primes:
                        primes.append(p)
                if checkp(j):
                    if j not in primes:
                        primes.append(j)
            if not checkp(j) and j != 1:
                for k in range(128, j):
                    if checkp(k):
                        if j%k == 0:
                            while j%k == 0:
                                j //= k
                            if k not in primes:
                                primes.append(k)
                        if checkp(j):
                            if j not in primes:
                                primes.append(j)
        elif j not in primes:
            primes.append(j)

primes.sort()
print(len(primes), end=" ")
for p in primes:
    if p != primes[-1]:
        print(p, end=" ")
    else:
        print(p)

