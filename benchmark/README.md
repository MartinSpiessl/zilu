# precision problem
In our invariant generation approach, we apply learning algorithm to produce a classifier which will be round off into integer coefficients.
This round off operation depends much on the precision we could bear.
By default, the precidion is set to 2, which means if there are two coefficents a, and b, and a > 100*b, b will be rounded off to 0.
This default setting works for most cases. But for the following cases, you need to specify a new precision value in order to learn an invariant. Otherwise, the learning iteration will continue until reaching the upbound or running overtime.

| benchmark | precision |
| --------- |:---------:|
| 08.cfg    |  1        |
| 15.cfg    |  1        |
| 16.cfg    |  2        |may take many times
| 20.cfg    |  1        |
| 21.cfg    |  ?        |-
| 22.cfg    |  1        |may take many times
| 23.cfg    |  3        |
| 24.cfg    |  1        |failed to reproduce the result for now
| 29.cfg    |  3        |
| 34.cfg    |  1        |
| 40.cfg    |  1        |
| 41.cfg    |  1        |question_mark as positive
| 42.cfg    |  1        |take many times
| 43.cfg    |  3        |
| 44.cfg    |  ?        |-
| 45.cfg    |  ?        |-
| 46.cfg    |  ?        |-
| 52.cfg    |  3        |
| 53.cfg    |  1        |
