#!/usr/bin/env python
# coding=utf-8
s1=list('abcdefghijklmnopqrstuvwsyz')
s2=list('ABCDEFGHIJKLMNOPQRSTUVWSYZ')
s=(string)(input())
n=int(input())
for i in range(len(s)):
    if s[i] in s1:
        s[i]=s1[(s1.index(s[i])+n)%26]
    else :
        s[i]=s2[(s2.index(s[i])+n)%26]
print(s)
