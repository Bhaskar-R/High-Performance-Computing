n = [2, 4, 6, 8, 12, 16, 20, 24, 32, 64, 128]
s = [
    0.3125,
    0.7143,
    0.1786,
    0.2632,
    0.1163,
    0.122,
    0.1163,
    0.119,
    0.0481,
    0.004,
    0.0008

]
o = []
for i in range(len(n)):
    o.append(((1 - (1 / s[i])) / (1 - (1 / n[i])))*100)
for i in o:
    print(round(i, 4))
