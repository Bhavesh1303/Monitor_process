# Monitor Process in Python

-   ``` 
    pip3 install -r requirements.txt 
    ```
- 
    ```
    python3 monitor_process.py
    ```
   
## Examples:
```
>python monitor_process.py
```
**Output:**
```
----------- Get all the Process Name, ID, Status, Time -----------
            Process Name  ProcessID                 Time Status(Start/Stop)
0    System Idle Process          0  1970-01-01 05:30:00              Start
1                 System          4  2021-03-04 18:32:22              Start
2           winlogon.exe          8  2021-03-04 18:32:58              Start
3                                72  2021-03-04 18:31:41               Stop
4               Registry        132  2021-03-04 18:31:41              Start
..                   ...        ...                  ...                ...
220        SearchApp.exe      20208  2021-03-04 22:46:45               Stop
221              cmd.exe      20336  2021-03-04 20:34:01              Start
222          conhost.exe      20344  2021-03-04 20:34:01              Start
223           chrome.exe      20392  2021-03-04 22:00:04              Start
224         vcpkgsrv.exe      20420  2021-03-04 23:11:17              Start

[78 rows x 4 columns]
.
.
.
.
```