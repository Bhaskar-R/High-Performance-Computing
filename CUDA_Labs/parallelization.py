n = [
    10,
    20,
    30,
    40,
    50,
    1024,
    10,
    10,
    10,
    10,
    10,
    1024,
    1024,
    1024,
    128,
    256,
    512,
    1024
]

s = [
    1.3675,
    1.3792,
    1.2995,
    1.2906,
    1.3653,
    1.2816,
    1.2468,
    1.4036,
    1.2664,
    1.3315,
    1.4073,
    1.2969,
    1.2073,
    0.9966,
    1.2167,
    1.1688,
    1.0222,
    0.8564


]
o = []
for i in range(len(n)):
    o.append(((1 - (1 / s[i])) / (1 - (1 / n[i])))*100)
for i in o:
    print(round(i, 4))
