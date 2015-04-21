# FAST Field Interpretation  Rules #
This doesn't substitute for understanding the FAST standard, but once you understand it, this is a convenient cheat-sheet to help you remember how an encoded field should be processed.

Start with a field operator and read to the right, branching down if necessary.  When you come to App Value column you know what the result of the decoding will be.  Dictionary tells you what value will be saved in the dictionary.

|<b>Field Op</b> |<b>Presence</b> |<b>PMAP</b>|<b>Value in Stream</b>|<b>Null?</b>|<b>App Value</b>|<b>Dictionary</b>|
|:---------------|:---------------|:----------|:---------------------|:-----------|:---------------|:----------------|
|<b>NOP</b>      |Mandatory|None|Value          |N/A  |Value     |N/A      |
|           |Optional |None|Nullable Value |NULL |None      |N/A      |
|           |         |    |               |VALUE|Value     |N/A      |
|<b>Constant</b> |Mandatory|None|No             |N/A  |Constant  |Unchanged|
|           |Optional |0   |No             |N/A  |None      |Unchanged|
|           |         |1   |No             |N/A  |Constant  |Unchanged|
|<b>Default</b>  |Mandatory|0   |No             |N/A  |Default   |Unchanged|
|           |         |1   |Value          |N/A  |Value     |Unchanged|
|           |Optional |0   |No             |N/A  |Default   |Unchanged|
|           |         |1   |Nullable Value |NULL |None      |Unchanged|
|           |         |    |               |VALUE|Value     |Unchanged|
|<b>Copy</b>     |Mandatory|0   |No             |N/A  |Dictionary|Unchanged|
|           |         |1   |Value          |N/A  |Value     |Value    |
|           |Optional |0   |No             |N/A  |Dictionary|Unchanged|
|           |         |1   |Nullable Value |NULL |None      |Empty    |
|           |         |    |               |VALUE|Value     |Value    |
|<b>Delta</b>    |Mandatory|None|Delta          |N/A  |Dictionary + Delta|Dictionary  + Delta|
|           |Optional |None|Nullable Delta |NULL|None       |Unchanged|
|           |         |    |               |DELTA|Dictionary  + Delta|Dictionary  + Delta|
|<b>Increment</b>|Mandatory|0   |No             |N/A  |Dictionary + 1|Dictionary + 1|
|           |         |1   |Value          |N/A  |Value     |Value    |
|           |Optional |0   |No             |N/A  |Dictionary + 1|Dictionary + 1|
|           |         |1   |Nullable Value |NULL |None      |Empty    |
|           |         |    |               |VALUE|Value     |Value    |
|<b>Tail</b>     |Mandatory|0   |No             |N/A  |Dictionary|Unchanged|
|           |         |1   |Tail           |N/A  |Dictionary `*` Tail|Dictionary `*` Tail|
|           |Optional |0   |No             |N/A  |Dictionary|Unchanged|
|           |         |1   |Nullable Tail  |NULL |None      |Empty    |
|           |         |    |               |TAIL |Dictionary `*` Tail|Dictionary `*` Tail|

Special Handling for Decimal Fields with individual operators for Exponent and Mantissa
|                    |<b>Mandatory</b>                 |<b>Optional</b>                 |
|:-------------------|:--------------------------------|:-------------------------------|
|<b>Exponent</b>         |Treat as Mandatory Integer  |Treat as Optional Integer   |
|<b>Mantissa</b>         |Treat as Mandatory Integer  |If Exponent is present then treat as Mandatory Integer; else Absent|
|<b>Presence Map Bits</b>|0, 1 or 2|0, 1, or 2 (may change dynamically)|