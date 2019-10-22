import subprocess

def check(expr, result):
    check.flag = True
    out = subprocess.run(['./alloc', *(expr.split())], stdout=subprocess.PIPE)

    if out.stdout.decode('ascii') != result:
        print('error {0}\n != \n{1}'.format(out.stdout.decode('ascii'), result))
    else:
        print('OK')

check('3 4', 'Error! Unable to allocate memory of size 4')
check('12 5 6 reset 10','Allocated 5 bytes\n'
			'Allocated 6 bytes\n'
			'Reset!\n'
			'Allocated 10 bytes\n'
			'success!')
check('4000000000 8','Error! Incorrect initial size')
check('20 5 4','Allocated 5 bytes\n'
		'Allocated 4 bytes\n'
		'success!')
check('4 3 reset 5', 'Allocated 3 bytes\n'
		     'Reset!\n'
		     'Error! Unable to allocate memory of size 5')
check('-1 8','Error! Incorrect initial size')
