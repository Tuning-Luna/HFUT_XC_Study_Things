
def compute(add):
    add(1, 2)

def add(x, y):
    print(x + y)

compute(lambda x, y: print(x + y))