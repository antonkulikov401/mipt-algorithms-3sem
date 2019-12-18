#!/usr/bin/python3
import sys

operation = sys.argv[1]
if operation == "unary_plus":
    print(int(sys.argv[2]), end="")
if operation == "unary_minus":
    print(-int(sys.argv[2]), end="")
if operation == "less":
    print(int(int(sys.argv[2]) < int(sys.argv[3])), end="")
if operation == "eq":
    print(int(int(sys.argv[2]) == int(sys.argv[3])), end="")
if operation == "addition":
    print(int(sys.argv[2]) + int(sys.argv[3]), end="")
if operation == "substraction":
    print(int(sys.argv[2]) - int(sys.argv[3]), end="")
if operation == "multiplication":
    print(int(sys.argv[2]) * int(sys.argv[3]), end="")
if operation == "division":
    d = int(sys.argv[2]) // int(sys.argv[3])
    print(d if int(sys.argv[2]) / int(sys.argv[3]) > 0 else d + 1, end="")
