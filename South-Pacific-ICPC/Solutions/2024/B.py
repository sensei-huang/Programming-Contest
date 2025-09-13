def candles(num, base):
    total = 0
    i = 0
    while base**i < num:
        i += 1
    i+=1
    for i2 in range(i, -1, -1):
        if num//(base**i2) > 0:
            total += num//(base**i2)
            num -= num//(base**i2)*(base**i2)
    return total

num = int(input())
small = num
for i in range(2, min(11, num+1)):
    candle = candles(num, i)
    if candle < small:
        small = candle
print(small)
