# The method we used was brute force. We swapped sticks until a delightful order is found.

stickInputNumber = int(input()) # Technically not needed as we never use it
stickInputString = input() # Take the line of sticks
sticks = stickInputString.split() # Split into string array

for i in range(len(sticks)): # Loop to turn string -> int
    sticks[i] = int(sticks[i])

swap = False # Variable to show if swapped occurred(indicating not confirmed delightful order)

for i in range(len(sticks)):
    if i < len(sticks)-2: # If there is still three sticks in the array(i, i+1, i+2)
        # Another way to express this if statement is: (sticks[i] < sticks[i+1]) + (sticks[i+1] < sticks[i+2]) != 1
        if sticks[i] < sticks[i+1] < sticks[i+2] or sticks[i] > sticks[i+1] > sticks[i+2]: # Not delightful pair
            sticks[i+1], sticks[i+2] = sticks[i+2], sticks[i+1] # Swap second pairs
            swap = False
    else:
        if swap == True: # No swaps occured
            print(' '.join(map(str, sticks)))
            break
        swap = True # Reset swap
        i = 0
