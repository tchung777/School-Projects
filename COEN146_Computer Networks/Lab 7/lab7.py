#!/user/bin/python
import sys

def is_prime(a):
    return all(a % i for i in range(2,a))

p = input("Give me a prime:")

if not is_prime(int(p)):
    sys.exit()

g = input("Give me a base:")
a = input("Give me secret number:")
b = input("Give me another secret number:")

p = int(p)
g = int(g)
a = int(a)
b = int(b)

A = g**a % p
B = g**b % p

s = B**a % p
s1 = A**b % p

#print out true if they are the same
print(s == s1)


