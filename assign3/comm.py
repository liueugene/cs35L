#!/usr/bin/python

import sys, locale
from optparse import OptionParser

def main():
    usage_msg = """%prog [OPTIONS] file1 file2

Compares file1 and file2, outputting three columns of text by default:
lines only in file1, lines only in file2, and lines in both

Specify -u when comparing unsorted files"""
    
    parser = OptionParser(usage=usage_msg)
    parser.add_option("-1",
                action="store_false", dest="print_file_1", default=True,
                help="Suppress the column of lines unique to file1")
    parser.add_option("-2",
                action="store_false", dest="print_file_2", default=True,
                help="Suppress the column of lines unique to file2")
    parser.add_option("-3",
                action="store_false", dest="print_both", default=True,
                help="Suppress the column of lines in both file1 and file2")
    parser.add_option("-u",
                action="store_true", dest="unsorted", default=False,
                help="Use if files are not sorted")
    
    global lines1
    global lines2
    global options
    options, args = parser.parse_args()
        
    if len(args) != 2:
        parser.error("Must have 2 input files")
        return 1
    
    filename1 = args[0]
    filename2 = args[1]
    
    try:
        if filename1 == '-':
            file1 = sys.stdin
        else:
            file1 = open(filename1, 'r')
            
        if filename2 == '-':
            file2 = sys.stdin
        else:
            file2 = open(filename2, 'r')
        
        lines1 = file1.read().splitlines()
        lines2 = file2.read().splitlines()
        file1.close()
        file2.close()
        
    except IOError as e:
        parser.error("I/O error({0}): {1}".
                     format(e.errno, e.strerror))
    
    global col2delimiter
    global col3delimiter
    col2delimiter = ""
    col3delimiter = ""
    
    if options.print_file_1:
        col2delimiter += "\t"
        col3delimiter += "\t"
    
    if options.print_file_2:
        col3delimiter += "\t"
    
    if options.unsorted:
        print_unsorted()
    else:
        print_sorted()

def print_sorted():  
    pos1 = 0
    pos2 = 0
    
    # compare lines in each list and print to the right column
    while pos1 < len(lines1) and pos2 < len(lines2):
        if locale.strcoll(lines1[pos1], lines2[pos2]) == 0:
            if options.print_both:
                print(col3delimiter + lines1[pos1])
            pos1 += 1
            pos2 += 1
            
        elif locale.strcoll(lines1[pos1], lines2[pos2]) > 0:
            if options.print_file_2:
                print(col2delimiter + lines2[pos2])
            pos2 += 1
            
        else:
            if options.print_file_1:
                print(lines1[pos1])
            pos1 += 1
            
    # print any remaining lines
    if pos1 < len(lines1) and options.print_file_1:
        for i in range(pos1, len(lines1)):
            print(lines1[i])
    
    if pos2 < len(lines2) and options.print_file_2:
        for i in range(pos2, len(lines2)):
            print(lines2[i])


def print_unsorted():
    for line in lines1:
        if line in lines2:
            lines2.remove(line)            
            if options.print_both:
                print(col3delimiter + line)
                
        elif options.print_file_1:
            print(line)
    
    if options.print_file_2:
        for line in lines2:
            print(col2delimiter + line)
    

if __name__ == "__main__":
    main()
