#!/usr/bin/python3
import random
import string

LINE_LENGTH = 4096
LINE_COUNT = 128 * 1024
choice = string.digits+string.ascii_letters


def output_line():
    line = ""
    for i in range(0, LINE_LENGTH-1):
        line += random.choice(choice)
    return line+"\n"


if __name__ == '__main__':
    try:
        out_file = open("input.data", "w")
        print("Start generating file ({} B)...\n".format(LINE_LENGTH*LINE_COUNT))
        progress = 0
        for j in range(0, LINE_COUNT):
            out_file.write(output_line())
            if (j-progress) > LINE_COUNT/10:
                print("{:.1f}%".format(100 * j / LINE_COUNT))
                progress = j
        out_file.close()
        print("100% Done")
    except Exception as e:
        print(e)


