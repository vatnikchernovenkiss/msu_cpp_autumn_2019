import subprocess

def check(expr, result):
    check.flag = True
    out = subprocess.run(['./Matrix', *(expr.split())], stdout=subprocess.PIPE)

    if out.stdout.decode('ascii') != result:
        print('error {0}\n != \n{1}'.format(out.stdout.decode('ascii'), result))
    else:
        print('OK')

check('3 4 5 6 cmp', '0\n')
check('3 4 3 4 a-2 cmp','1\n')
check('3 4 3 4 a-3 uncmp','1\n')
check('3 4 3 4 56 uncmp','Bad range!\n')
check('3 4 3 4 a12 10 ', '12\n')
check('6 7 3 4 rows','6\n')
check('7 9 3 4 cols', '9\n')
check('3 4 3 4 const', '0\n')
