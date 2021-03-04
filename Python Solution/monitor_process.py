#Python program to monitor the process in the windows machine
import psutil
import datetime
import pandas as pd
import time
import os

def construct_dataframe(processes):
    # convert to pandas dataframe
    df = pd.DataFrame(processes)
    # set the process id as index of a process
    # df.set_index('ProcessID', inplace=True)
    return df

if __name__ == '__main__':
    print("----------- Get all the Process Name, ID, Status, Time -----------")
    while 1:
        processes = []
        # Iterate over all running process
        for proc in psutil.process_iter():
            try:
                # Get process details from the process object.
                processName = proc.name()
                processID = proc.pid
                if proc.status() == 'running':
                    processStatus='Start'
                elif proc.status() == 'stopped':
                    processStatus='Stop'
                else:
                    processStatus = proc.status()
                processTime = datetime.datetime.fromtimestamp(proc.create_time()).strftime("%Y-%m-%d %H:%M:%S")
                # print(processName , ' ::: ', processID,  ' ::: ', processTime, ' ::: ', processStatus)
                processes.append({ 'Process Name': processName, 'ProcessID': processID, 'Time': processTime, 'Status(Start/Stop)': processStatus })
            except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
                pass
        df = construct_dataframe(processes)
        df = df.drop_duplicates(subset = ["Process Name", "Status(Start/Stop)"], keep = 'last')
        print(df)
        df.to_csv(r'result.txt', header="ProcessID\t\t\tProcess Name\t\t\tTime\t\t\tStatus(Start/Stop)", index=None, sep='\t', mode='w+')
        time.sleep(1)
    