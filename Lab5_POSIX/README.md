mpicc -pthread main.c -o main -lm
mpirun -np N ./main


out:
mpirun -np 8 ./main
=================================================
Iteration number: 0
Disbalance time: 2.214536
Disbalance percentage: 16.089492
----------------------------------------------
		Current proc is: 0
Amount of executed tasks: 5101
Result of calculating is: 2481.652625
Time per iteration: 12.924681 seconds
		Current proc is: 1
Amount of executed tasks: 3007
Result of calculating is: 2328.463012
Time per iteration: 11.619424 seconds
		Current proc is: 2
Amount of executed tasks: 2536
Result of calculating is: 1733.287797
Time per iteration: 12.207244 seconds
		Current proc is: 3
Amount of executed tasks: 2199
Result of calculating is: 1581.992993
Time per iteration: 13.763866 seconds
		Current proc is: 4
Amount of executed tasks: 1768
Result of calculating is: 1446.802559
Time per iteration: 11.776573 seconds
		Current proc is: 5
Amount of executed tasks: 1422
Result of calculating is: 1195.912072
Time per iteration: 12.452507 seconds
		Current proc is: 6
Amount of executed tasks: 2044
Result of calculating is: 911.088810
Time per iteration: 11.549330 seconds
		Current proc is: 7
Amount of executed tasks: 1123
Result of calculating is: 899.308761
Time per iteration: 12.869735 seconds
=================================================
Iteration number: 1
Disbalance time: 2.238480
Disbalance percentage: 16.266336
----------------------------------------------
		Current proc is: 0
Amount of executed tasks: 5104
Result of calculating is: 4960.698790
Time per iteration: 12.943465 seconds
		Current proc is: 1
Amount of executed tasks: 3013
Result of calculating is: 4660.071400
Time per iteration: 11.564189 seconds
		Current proc is: 2
Amount of executed tasks: 2543
Result of calculating is: 3472.407938
Time per iteration: 12.223636 seconds
		Current proc is: 3
Amount of executed tasks: 2195
Result of calculating is: 3158.657782
Time per iteration: 13.761428 seconds
		Current proc is: 4
Amount of executed tasks: 1766
Result of calculating is: 2890.530048
Time per iteration: 11.798706 seconds
		Current proc is: 5
Amount of executed tasks: 1421
Result of calculating is: 2400.706949
Time per iteration: 12.458197 seconds
		Current proc is: 6
Amount of executed tasks: 2042
Result of calculating is: 1826.507458
Time per iteration: 11.522948 seconds
		Current proc is: 7
Amount of executed tasks: 1116
Result of calculating is: 1803.318415
Time per iteration: 12.919902 seconds
=================================================
Iteration number: 2
Disbalance time: 2.258551
Disbalance percentage: 16.345051
----------------------------------------------
		Current proc is: 0
Amount of executed tasks: 5106
Result of calculating is: 7444.399497
Time per iteration: 13.008965 seconds
		Current proc is: 1
Amount of executed tasks: 3013
Result of calculating is: 6990.047964
Time per iteration: 11.590857 seconds
		Current proc is: 2
Amount of executed tasks: 2542
Result of calculating is: 5209.516041
Time per iteration: 12.291980 seconds
		Current proc is: 3
Amount of executed tasks: 2197
Result of calculating is: 4738.321445
Time per iteration: 13.817951 seconds
		Current proc is: 4
Amount of executed tasks: 1767
Result of calculating is: 4336.562627
Time per iteration: 11.811844 seconds
		Current proc is: 5
Amount of executed tasks: 1419
Result of calculating is: 3606.552055
Time per iteration: 12.476437 seconds
		Current proc is: 6
Amount of executed tasks: 2050
Result of calculating is: 2748.112420
Time per iteration: 11.559400 seconds
		Current proc is: 7
Amount of executed tasks: 1106
Result of calculating is: 2702.715024
Time per iteration: 12.992497 seconds
=================================================
Iteration number: 3
Disbalance time: 2.092688
Disbalance percentage: 14.944721
----------------------------------------------
		Current proc is: 0
Amount of executed tasks: 4999
Result of calculating is: 9824.320051
Time per iteration: 12.438688 seconds
		Current proc is: 1
Amount of executed tasks: 3037
Result of calculating is: 9342.321567
Time per iteration: 11.910167 seconds
		Current proc is: 2
Amount of executed tasks: 2654
Result of calculating is: 7050.332960
Time per iteration: 14.002855 seconds
		Current proc is: 3
Amount of executed tasks: 2221
Result of calculating is: 6343.299235
Time per iteration: 13.941318 seconds
		Current proc is: 4
Amount of executed tasks: 1653
Result of calculating is: 5711.869107
Time per iteration: 12.026871 seconds
		Current proc is: 5
Amount of executed tasks: 1887
Result of calculating is: 4936.151830
Time per iteration: 12.776611 seconds
		Current proc is: 6
Amount of executed tasks: 1661
Result of calculating is: 3691.719811
Time per iteration: 11.954421 seconds
		Current proc is: 7
Amount of executed tasks: 1088
Result of calculating is: 3541.738864
Time per iteration: 12.874222 seconds
=================================================
Iteration number: 4
Disbalance time: 1.598021
Disbalance percentage: 10.912987
----------------------------------------------
		Current proc is: 0
Amount of executed tasks: 5084
Result of calculating is: 12291.233052
Time per iteration: 13.063522 seconds
		Current proc is: 1
Amount of executed tasks: 2969
Result of calculating is: 11995.364409
Time per iteration: 14.297257 seconds
		Current proc is: 2
Amount of executed tasks: 2555
Result of calculating is: 9012.727131
Time per iteration: 13.045275 seconds
		Current proc is: 3
Amount of executed tasks: 1814
Result of calculating is: 7999.532610
Time per iteration: 14.643296 seconds
		Current proc is: 4
Amount of executed tasks: 1992
Result of calculating is: 7098.786387
Time per iteration: 13.143131 seconds
		Current proc is: 5
Amount of executed tasks: 2234
Result of calculating is: 6229.152743
Time per iteration: 13.068542 seconds
		Current proc is: 6
Amount of executed tasks: 1487
Result of calculating is: 4897.296784
Time per iteration: 13.592245 seconds
		Current proc is: 7
Amount of executed tasks: 1065
Result of calculating is: 4379.944898
Time per iteration: 13.753658 seconds
=======================================
Time for all lists: 69.991226 seconds


mpirun -np 8 ./main_test
=================================================
Iteration number: 0
Disbalance time: 2.181139
Disbalance percentage: 14.416405
----------------------------------------------
All tasks were successfully executed.
		Current proc is: 0
Amount of executed tasks: 5138
Result of calculating is: 2508.530379
Time per iteration: 15.129564 seconds
Active time: 15.127873 seconds
Idle time percentage: 0.011182%
		Current proc is: 1
Amount of executed tasks: 2706
Result of calculating is: 2455.768848
Time per iteration: 13.321523 seconds
Active time: 13.320099 seconds
Idle time percentage: 0.010689%
		Current proc is: 2
Amount of executed tasks: 2607
Result of calculating is: 2007.703701
Time per iteration: 13.532661 seconds
Active time: 13.530833 seconds
Idle time percentage: 0.013509%
		Current proc is: 3
Amount of executed tasks: 2194
Result of calculating is: 1606.117334
Time per iteration: 13.348006 seconds
Active time: 13.346774 seconds
Idle time percentage: 0.009233%
		Current proc is: 4
Amount of executed tasks: 1940
Result of calculating is: 1285.854880
Time per iteration: 13.397862 seconds
Active time: 13.396699 seconds
Idle time percentage: 0.008679%
		Current proc is: 5
Amount of executed tasks: 1749
Result of calculating is: 1098.885895
Time per iteration: 13.404137 seconds
Active time: 13.402670 seconds
Idle time percentage: 0.010947%
		Current proc is: 6
Amount of executed tasks: 1829
Result of calculating is: 986.251954
Time per iteration: 12.948425 seconds
Active time: 12.947409 seconds
Idle time percentage: 0.007845%
		Current proc is: 7
Amount of executed tasks: 1037
Result of calculating is: 792.236925
Time per iteration: 13.351776 seconds
Active time: 13.351384 seconds
Idle time percentage: 0.002941%
=================================================
Iteration number: 1
Disbalance time: 2.091447
Disbalance percentage: 14.280053
----------------------------------------------
All tasks were successfully executed.
		Current proc is: 0
Amount of executed tasks: 5101
Result of calculating is: 5003.303154
Time per iteration: 12.554485 seconds
Active time: 12.553707 seconds
Idle time percentage: 0.006199%
		Current proc is: 1
Amount of executed tasks: 2738
Result of calculating is: 4952.833271
Time per iteration: 13.008388 seconds
Active time: 13.007761 seconds
Idle time percentage: 0.004818%
		Current proc is: 2
Amount of executed tasks: 2251
Result of calculating is: 4082.666802
Time per iteration: 14.645931 seconds
Active time: 14.645394 seconds
Idle time percentage: 0.003671%
		Current proc is: 3
Amount of executed tasks: 2382
Result of calculating is: 3209.276511
Time per iteration: 12.632965 seconds
Active time: 12.632476 seconds
Idle time percentage: 0.003869%
		Current proc is: 4
Amount of executed tasks: 1711
Result of calculating is: 2809.914603
Time per iteration: 13.248676 seconds
Active time: 13.248258 seconds
Idle time percentage: 0.003153%
		Current proc is: 5
Amount of executed tasks: 2223
Result of calculating is: 2473.708568
Time per iteration: 13.497409 seconds
Active time: 13.496927 seconds
Idle time percentage: 0.003569%
		Current proc is: 6
Amount of executed tasks: 1849
Result of calculating is: 2038.435838
Time per iteration: 13.332701 seconds
Active time: 13.332116 seconds
Idle time percentage: 0.004391%
		Current proc is: 7
Amount of executed tasks: 945
Result of calculating is: 1632.486410
Time per iteration: 13.369322 seconds
Active time: 13.368965 seconds
Idle time percentage: 0.002675%
=================================================
Iteration number: 2
Disbalance time: 2.304404
Disbalance percentage: 15.036671
----------------------------------------------
All tasks were successfully executed.
		Current proc is: 0
Amount of executed tasks: 5057
Result of calculating is: 7440.514427
Time per iteration: 13.802108 seconds
Active time: 13.801179 seconds
Idle time percentage: 0.006734%
		Current proc is: 1
Amount of executed tasks: 3096
Result of calculating is: 7725.658447
Time per iteration: 15.325227 seconds
Active time: 15.324465 seconds
Idle time percentage: 0.004971%
		Current proc is: 2
Amount of executed tasks: 2569
Result of calculating is: 6060.066914
Time per iteration: 13.471885 seconds
Active time: 13.471242 seconds
Idle time percentage: 0.004769%
		Current proc is: 3
Amount of executed tasks: 1808
Result of calculating is: 4839.817075
Time per iteration: 13.020823 seconds
Active time: 13.020241 seconds
Idle time percentage: 0.004474%
		Current proc is: 4
Amount of executed tasks: 1806
Result of calculating is: 4126.889189
Time per iteration: 13.378314 seconds
Active time: 13.377851 seconds
Idle time percentage: 0.003465%
		Current proc is: 5
Amount of executed tasks: 2133
Result of calculating is: 3981.441604
Time per iteration: 14.084514 seconds
Active time: 14.084004 seconds
Idle time percentage: 0.003620%
		Current proc is: 6
Amount of executed tasks: 1556
Result of calculating is: 3057.770300
Time per iteration: 13.216112 seconds
Active time: 13.215667 seconds
Idle time percentage: 0.003364%
		Current proc is: 7
Amount of executed tasks: 1175
Result of calculating is: 2523.339674
Time per iteration: 13.656713 seconds
Active time: 13.655840 seconds
Idle time percentage: 0.006386%
=================================================
Iteration number: 3
Disbalance time: 2.860830
Disbalance percentage: 18.899533
----------------------------------------------
All tasks were successfully executed.
		Current proc is: 0
Amount of executed tasks: 5077
Result of calculating is: 9894.218621
Time per iteration: 12.399145 seconds
Active time: 12.398328 seconds
Idle time percentage: 0.006589%
		Current proc is: 1
Amount of executed tasks: 2739
Result of calculating is: 10224.707948
Time per iteration: 12.276209 seconds
Active time: 12.275528 seconds
Idle time percentage: 0.005551%
		Current proc is: 2
Amount of executed tasks: 2581
Result of calculating is: 8043.987078
Time per iteration: 15.015715 seconds
Active time: 15.014985 seconds
Idle time percentage: 0.004863%
		Current proc is: 3
Amount of executed tasks: 2163
Result of calculating is: 6446.192871
Time per iteration: 15.137039 seconds
Active time: 15.136474 seconds
Idle time percentage: 0.003733%
		Current proc is: 4
Amount of executed tasks: 1864
Result of calculating is: 5621.412940
Time per iteration: 12.918112 seconds
Active time: 12.917530 seconds
Idle time percentage: 0.004501%
		Current proc is: 5
Amount of executed tasks: 1957
Result of calculating is: 5362.291149
Time per iteration: 12.950521 seconds
Active time: 12.950026 seconds
Idle time percentage: 0.003826%
		Current proc is: 6
Amount of executed tasks: 1745
Result of calculating is: 4084.367068
Time per iteration: 13.416397 seconds
Active time: 13.415946 seconds
Idle time percentage: 0.003364%
		Current proc is: 7
Amount of executed tasks: 1074
Result of calculating is: 3347.433999
Time per iteration: 13.334253 seconds
Active time: 13.333846 seconds
Idle time percentage: 0.003050%
=================================================
Iteration number: 4
Disbalance time: 6.061990
Disbalance percentage: 33.317794
----------------------------------------------
All tasks were successfully executed.
		Current proc is: 0
Amount of executed tasks: 5076
Result of calculating is: 12347.612429
Time per iteration: 12.132461 seconds
Active time: 12.131516 seconds
Idle time percentage: 0.007794%
		Current proc is: 1
Amount of executed tasks: 2713
Result of calculating is: 12703.034659
Time per iteration: 12.255341 seconds
Active time: 12.254597 seconds
Idle time percentage: 0.006075%
		Current proc is: 2
Amount of executed tasks: 2564
Result of calculating is: 10010.064613
Time per iteration: 15.100372 seconds
Active time: 15.099676 seconds
Idle time percentage: 0.004611%
		Current proc is: 3
Amount of executed tasks: 2411
Result of calculating is: 8278.681859
Time per iteration: 18.194451 seconds
Active time: 18.193252 seconds
Idle time percentage: 0.006592%
		Current proc is: 4
Amount of executed tasks: 1887
Result of calculating is: 7139.894770
Time per iteration: 13.608731 seconds
Active time: 13.608123 seconds
Idle time percentage: 0.004464%
		Current proc is: 5
Amount of executed tasks: 1904
Result of calculating is: 6706.874395
Time per iteration: 13.137003 seconds
Active time: 13.136202 seconds
Idle time percentage: 0.006092%
		Current proc is: 6
Amount of executed tasks: 1236
Result of calculating is: 5028.421025
Time per iteration: 12.171220 seconds
Active time: 12.170808 seconds
Idle time percentage: 0.003392%
		Current proc is: 7
Amount of executed tasks: 1409
Result of calculating is: 4140.674158
Time per iteration: 13.020344 seconds
Active time: 13.019907 seconds
Idle time percentage: 0.003360%
=======================================
Time for all lists: 78.434545 seconds
Final result of all sin sums: 66355.257908



