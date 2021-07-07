#!/usr/bin/python3
import time
from threading import Thread

NUMBER_OF_THREADS = 4

def process_line(in_line):
    #swapcase
    out_line = in_line.swapcase()

    #process numbers
    for i in range(0, len(out_line)):
        if out_line[i].isdigit():
            d = int(out_line[i])
            if d<9:
                out_line = out_line[:i] + '{}'.format(d+1) + out_line[i+1:]
            else:
                out_line = out_line[:i] + '0' + out_line[i+1:]
    return out_line


def process_thread(lines, offset, threads_count):
    for i in range(offset, len(lines), threads_count):
        line_in = lines[i]
        line_out = process_line(line_in)
        lines[i] = line_out


if __name__ == '__main__':
    start = time.time()

    try:
        print("Load input data")
        in_file = open("input.data", "r")
        lines = in_file.readlines()
        in_file.close()

        data_load = time.time()

        threads_pool = []

        print("Start {} thread".format(NUMBER_OF_THREADS))
        #start threads
        for i in range(0, NUMBER_OF_THREADS):
            thread = Thread(target=process_thread, args=(lines, i, NUMBER_OF_THREADS))
            thread.start()
            threads_pool.append(thread)

        print("Wait for finish")
        #wiat for finish
        for t in threads_pool:
            t.join()
        
        data_process = time.time()

        print("Save output data")
        out_file = open("output_py.data", "w")
        out_file.writelines(lines)
        out_file.close()

        print("Done.")
    except Exception as e:
        data_process = time.time()
        print(e)

    end = time.time()
    print("Data load time: {}".format(data_load - start))
    print("Data process time: {}".format(data_process - data_load))
    print("Data save time: {}".format(end - data_process))
    print("Total Execution time: {}".format(end - start))



