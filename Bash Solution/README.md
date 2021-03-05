### For the bash solution you have to run the below command in terminal

```
for /f "tokens=1,2,7,9" %i in ('tasklist /v /fi "PID ne 0" /fi "USERNAME ne NT AUTHORITY\*" /fi "USERNAME ne Font Driver Host\*"') do @echo Process Name = %i,    PID = %j,    Status = %k,    Time = %l
```