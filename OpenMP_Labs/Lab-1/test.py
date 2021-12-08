import matplotlib.pyplot as plt
execution_time = [26.773438,
                  13.974609,
                  7.917969,
                  8.867188,
                  8.998047,
                  10.419922,
                  10.099609,
                  9.855469,
                  10.281250,
                  9.134766,
                  9.085938,
                  9.611328,
                  8.851562
                  ]
Threads = [1,
           2,
           4,
           6,
           8,
           10,
           12,
           16,
           20,
           32,
           64,
           128,
           150
           ]

plt.plot(Threads, execution_time)
