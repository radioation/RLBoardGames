SIZE = 4

def to_index(x, y, z):
    return z * SIZE * SIZE + y * SIZE + x

# single layer checks
print("' Offsets for row cells in layer")
print('DATA WINLINE_ROWS_IN_LAYER() BYTE = ', end="")
z = 0 # just compute one layer
for y in range(SIZE):
    for x in range(SIZE):
        print( f"{ to_index(x,y,z)},", end= "")
    print(" ", end="")


print("\n' Offsets for col cells in layer")
print('DATA WINLINE_COLS_IN_LAYER  = ', end="")
z = 0 # just compute one layer
for x in range(SIZE):
    for y in range(SIZE):
        print( f"{ to_index(x,y,z)},", end= "")
    print(" ", end="")


print("\n' offsets for diagonal cells in a layer")
print('DATA WINLINE_MULTILAYER() BYTE  = ', end="")
z = 0
for i in range(SIZE):
    print( f"{ to_index(i,i,z)},", end= "")
print(" ", end="")
for i in range(SIZE):
    print( f"{ to_index(SIZE-i-1,i,z )},", end= "")  



# Multilayer checks

# straight down X,Y ( don' really ned to enumerate?, just 0 through 15 for xy
# and then Z in (0,16,32,48)


# corner to corner
print("\n' Diagonal cells to cube corners")
print('DATA BYTE = ', end="")
for i in range(SIZE):
    print( f"{ to_index(i,i,i)},", end= "")
print(" ", end="")
for i in range(SIZE):
    print( f"{ to_index(SIZE - 1 -i,i,i)},", end= "")
print(" ", end="")
for i in range(SIZE):
    print( f"{ to_index(i, SIZE - 1 -i, i)},", end= "")
print(" ", end="")
for i in range(SIZE):
    print( f"{ to_index(SIZE-1 -i, SIZE - 1 -i, i)},", end= "")
print(" ", end="")



print("\n' Diagonal cells in col")
print('DATA BYTE = ', end="")
for x in range(SIZE):
    for i in range(SIZE):
        print( f"{ to_index(x, i, i)},", end= "")
    print(" ", end="")
for x in range(SIZE):
    for i in range(SIZE):
        print( f"{ to_index(x, SIZE-1-i, i)},", end= "")
    print(" ", end="")


print("\n' Diagonal cells in row")
print('DATA BYTE = ', end="")
for y in range(SIZE):
    for i in range(SIZE):
        print( f"{ to_index(i, y, i)},", end= "")
    print(" ", end="")
for y in range(SIZE):
    for i in range(SIZE):
        print( f"{ to_index(SIZE-1-i, y, i)},", end= "")
    print(" ", end="")





