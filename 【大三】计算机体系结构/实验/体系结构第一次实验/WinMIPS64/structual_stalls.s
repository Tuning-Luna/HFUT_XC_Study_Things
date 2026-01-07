.data
A: .double 8
B: .double 2
C: .double 0
D: .double 9
E: .double 7
F: .double 0
.text
main:
l.d f0,A(r0)
l.d f1,B(r0)
mul.d f3, f0, f1
mul.d f4, f0, f1
mul.d f5, f0, f1
mul.d f6, f0, f1
s.d f3,C(r0)

l.d f2, D(r1)
l.d f3, E(r1)
mul.d f7, f2, f3
mul.d f8, f2, f3
mul.d f9, f2, f3
mul.d f10, f2, f3
s.d f4, F(r1)

halt