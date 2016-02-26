def b(x, n=8):
    return ("{:0" + str(n) + "b}").format(x)

def w(*args):
    return " | ".join(map(b, args))

def f(s):
    return " | ".join((s[i:i+8] for i in range(0, len(s), 8)))
