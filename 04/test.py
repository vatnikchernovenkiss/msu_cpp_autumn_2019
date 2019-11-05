import subprocess

def check(expr, result):
    check.flag = True
    out = subprocess.run(['./BigInt', *(expr.split())], stdout=subprocess.PIPE)

    if out.stdout.decode('ascii') != result:
        print('error {0}\n != \n{1}'.format(out.stdout.decode('ascii'), result))
    else:
        print('OK')

check('2000000000 2000000000 + + + + + +', '14000000000\n')
check('-1 999 ! +', '-1000\n')
check('999999 1 +','1000000\n')
check('-123456789 -123456789 @ @ @ @ @ @','617283945\n')
check('-2000000000 -2000000000 + 1000000000 + + + + 0 =','1\n0\n')
check('0 0 =','1\n0\n')
check('-1000000000 2 +','-999999998\n')
check('1000 20000 + 20999 >','1\n21000\n')
check('-333333 233 + -333100 >', '0\n-333100\n')
check('-2147483648 -2147483648 + + +', '-8589934592\n')
